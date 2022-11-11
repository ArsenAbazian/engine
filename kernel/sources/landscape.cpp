#include "landscape.h"
#include "m_png.h"
#include <dxerr9.h>
#include "hlslman.h"
#include "a3dModel\\a3dModel.h"
#include "a3dDesc\\a3dDesc.h"
#include "a3dScene.h"
#include "texture.h"
#include "radiosity.h"

//FILE *fp1;

// загрузка карты
bool	loadFloatMap( TCHAR *fileName, float ***map, int *size )
{

	PNG			png;
	char		tmp[ 512 ];		// временный буффер для имени файла
	int			index, index2, index3;

	// конвертируем unicode в ansi
	wcsToStr( fileName, tmp ); 

	// пытаемся открыть файл
	png.openRFile( tmp );	
	if( png.data->status != 0)
		return false;

	png.load();

	if( png.data->type != PNG_T_R8G8B8 && png.data->type != PNG_T_A8R8G8B8 && png.data->type != PNG_T_R16G16B16 && png.data->type != PNG_T_A16R16G16B16 && png.data->type != PNG_T_GRY16 )
		return false;

	*size		= png.data->width;
	
	// выделяем строки
	(*map) = new float*[ *size ];
	
	if( png.data->type == PNG_T_R8G8B8 )
	{
		for( index = 0, index3 = 0; index < *size; index ++ )
		{
			// столбцы
			(*map)[index] = new float[ *size ];
			
			// заполняем данными
			for( index2 = 0; index2 < *size; index2 ++, index3 += 3 )
				(*map)[index][index2] = ((BYTE*)png.data->buf)[ index3 ] * ( 1.0f / 255.0f );
		}
	}
	else if( png.data->type == PNG_T_A8R8G8B8 )
	{
		for( index = 0, index3 = 2; index < *size; index ++ )
		{
			// столбцы
			(*map)[index] = new float[ *size ];
			
			// заполняем данными
			for( index2 = 0; index2 < *size; index2 ++, index3 += 4 )
				(*map)[index][index2] = ((BYTE*)png.data->buf)[ index3 ] * ( 1.0f / 255.0f );
		}	
	}
	if( png.data->type == PNG_T_R16G16B16 )
	{
		for( index = 0, index3 = 0; index < *size; index ++ )
		{
			// столбцы
			(*map)[index] = new float[ *size ];
			
			// заполняем данными
			for( index2 = 0; index2 < *size; index2 ++, index3 += 3 )
				(*map)[index][index2] = ((WORD*)png.data->buf)[ index3 ] * ( 1.0f / 65536.0f );
		}
	}
	if( png.data->type == PNG_T_A16R16G16B16 )
	{
		for( index = 0, index3 = 0; index < *size; index ++ )
		{
			// столбцы
			(*map)[index] = new float[ *size ];
			
			// заполняем данными
			for( index2 = 0; index2 < *size; index2 ++, index3 += 4 )
				(*map)[index][index2] = ((WORD*)png.data->buf)[ index3 ] * ( 1.0f / 65536.0f );
		}
	}
	if( png.data->type == PNG_T_GRY16 )
	{
		for( index = 0, index3 = 0; index < *size; index ++ )
		{
			// столбцы
			(*map)[index] = new float[ *size ];
			
			// заполняем данными
			for( index2 = 0; index2 < *size; index2 ++, index3 ++ )
				(*map)[index][index2] = ((WORD*)png.data->buf)[ index3 ] * ( 1.0f / 65536.0f );
		}
	}

	return true;
}

void landscapePatch::clear()
{
	if( this->variance != NULL )
		delete[] variance;
	this->variance = NULL;

	if( this->error != NULL )
		delete[] error;
	this->error = NULL;

	if( this->prevRowIndicies != NULL )
		delete[] this->prevRowIndicies;
 
	this->clearDeltaMap();
	this->clearVertexRefMap();
	
	this->setNeighbors( NULL, NULL, NULL, NULL );
}

landscapePatch::landscapePatch()
{
	
	this->buffersLocked			= false;
	this->beginRow				= 0;
	this->beginCol				= 0;
	this->rowIndex				= 0;
	this->colIndex				= 0;
	this->detalizationLevel		= -1;	
	this->realDetalizationLevel = -1;
	this->levelUp				= false;
	this->updateValue			= 0.0f;
	this->update				= true;
	this->visible				= false;

	this->land					= NULL;		
	this->variance				= NULL;		
	this->prevRowIndicies		= NULL;
	this->vertexRefMap			= NULL;

	this->vertexBuffer			= NULL;
	this->indexBuffer			= NULL;
	this->vertexBufferSize		= 0;
	this->indexBufferSize		= 0;
	
	this->deltaLeft				= 0x7fffffff;
	this->deltaRight			= 0x7fffffff;
	this->deltaBottom			= 0x7fffffff;
	this->deltaTop				= 0x7fffffff;

	this->passInfo.setPatch(this);
	this->setNeighbors( NULL, NULL, NULL, NULL );
}

landscapePatch::~landscapePatch()
{
	this->clear();
}

int landscapePatch::getCountByLevel(int level) { 
	// 0 - 1 квадрат
	// 1 - 2 квадрата
	// 2 - 4 квадрата
	int quad = 1 << level;
	int nextQuad = quad << 1;

	return quad * (quad + 1) + (nextQuad + 1) * quad;
}

void landscapePatch::initializeVertexRefMap() { 
	int row;
	
	this->vertexRefMap = new int*[4];
	for( row = 0; row < 4; row ++ ) {
		this->vertexRefMap[row] = new int[this->land->quadCountPerPatchSide + 1];
	}
}

void landscapePatch::clearVertexRefMap() { 
	if( this->vertexRefMap == NULL ) return;
	
	int row;
	for( row = 0; row < 4; row ++ )
		delete[] this->vertexRefMap[row];
	delete[] this->vertexRefMap;

	this->vertexRefMap = NULL;
}

void landscapePatch::initializeDeltaMap() { 
	
	int deltaIndex;
	this->deltaMap				= new float*[ land->quadCountPerPatchSide + 1 ];
	for( deltaIndex = 0; deltaIndex < this->land->quadCountPerPatchSide + 1; deltaIndex ++ )
		this->deltaMap[deltaIndex] = new float[ this->land->quadCountPerPatchSide + 1 ];
}

void landscapePatch::clearDeltaMap() { 
	
	if( this->deltaMap == NULL ) return;
	int deltaIndex;
	for( deltaIndex = 0; deltaIndex < this->land->quadCountPerPatchSide + 1; deltaIndex ++ )
		delete[] this->deltaMap[deltaIndex];

	delete[] this->deltaMap;
	this->deltaMap = NULL;
}

void landscapePatch::initialize( a3dLandscape *l, int row, int col, int rowInd, int colInd )
{
	
	this->land				= l;
	this->beginRow			= row;
	this->beginCol			= col;
	this->rowIndex			= rowInd;
	this->colIndex			= colInd;

	// выделяем память под массив значений
	this->variance			= new float[ land->varianceCount ];
	this->error				= new float[ land->varianceCount ];
	
	// массив отклонений
	this->initializeDeltaMap();

	// массив индексов вершин
	this->initializeVertexRefMap();

	// выделяем память под временный буффер индексов вершин
	this->prevRowIndicies	= new int[ land->quadCountPerPatchSide + 1 ];

	return ;
}

// в прицнипе если ландшафт один - то данная функция не нужна, иначе она позволяет сравнивать ландшафты
void landscapePatch::setNeighbors( landscapePatch *top, landscapePatch *left, landscapePatch *bottom, landscapePatch *right )
{

	this->topPatch		= top;
	this->leftPatch		= left;
	this->bottomPatch	= bottom;
	this->rightPatch	= right;
	
	return ;
}

float landscapePatch::getError(int r0, int c0, int r1, int c1) { 
	
	int cr = (r1 + r0) >> 1;
	int cc = (c1 + c0) >> 1;

	return fabs((this->land->heightMap[ r0 ][ c0 ].position[1] + this->land->heightMap[ r1 ][ c1 ].position[1]) * 0.5f - this->land->heightMap[ cr ][ cc ].position[1]);
}

/*
float landscapePatch::calcVariance(int level) { 
	
	int step = this->land->quadCountPerPatchSide >> level;
	int r, c, errorCount = 0;
	float summError = 0.0f;
	int endRow = this->beginRow + this->land->quadCountPerPatchSide;
	int endCol = this->beginCol + this->land->quadCountPerPatchSide;
	
	for( r = this->beginRow; r <= endRow; r += step) { 
		for( c = this->beginCol; c <= endCol; c += step ) { 
			if( r < endRow ) { 
				summError += this->getError(r, c, r + step, c);
				errorCount ++;
			}
			if( c < endCol ) { 
				summError += this->getError(r, c, r, c + step);
				errorCount ++;
			}
			if( r < endRow && c < endCol ) { 
				summError += this->getError(r, c, r + step, c + step);
				summError += this->getError(r, c + step, r + step, c);
				errorCount += 2;
			}
		}
	}
	
	return summError / errorCount;
}

*/

float landscapePatch::calcVariance( int row0, int col0, int row1, int col1, bool shouldProcessLastRow, bool shouldProcessLastCol ) { 

	float summError = 0.0f;
	
	float dRow0, dRow1, dCol, y, y0, y1;
	int row, col, endRow, endCol;

	row0 += this->beginRow;
	col0 += this->beginCol;
	row1 += this->beginRow;
	col1 += this->beginCol;

	landVertex **hMap = this->land->heightMap;
	dRow0 = (hMap[row1][col0].position[1] - hMap[row0][col0].position[1]) / (row1 - row0);
	dRow1 = (hMap[row1][col1].position[1] - hMap[row0][col1].position[1]) / (row1 - row0);

	if( !shouldProcessLastRow ) endRow = row1 - 1;
	else endRow = row1;

	if( !shouldProcessLastCol ) endCol = col1 - 1;
	else endCol = col1;

	y0 = hMap[row0][col0].position[1];
	y1 = hMap[row0][col1].position[1];
	for( row = row0; row <= endRow; row++, y0 += dRow0, y1 += dRow1 ) { 
		dCol = (y1 - y0) / (col1 - col0);
		y = y0;
		for( col = col0; col <= endCol; col ++, y += dCol) {
			summError += fabs(hMap[row][col].position[1] - y);
		}
	}
	return summError;
}

float landscapePatch::calcVariance( int level ) {
	
	int delta = this->land->quadCountPerPatchSide >> level;
	int row, col;
	float summError = 0.0f;

	for( row = 0; row < this->land->quadCountPerPatchSide; row += delta ) 
	{
		for( col = 0; col < this->land->quadCountPerPatchSide; col += delta ) {
			summError += this->calcVariance( row, col, row + delta, col + delta, row == this->land->quadCountPerPatchSide - delta, col == this->land->quadCountPerPatchSide - delta );
		}
	}
	
	return summError / ( ( this->land->quadCountPerPatchSide + 1 ) * ( this->land->quadCountPerPatchSide + 1 ) );
} 

void landscapePatch::calcVariance()
{

	//_tfopen_s( &fp1, TEXT( "log.xml" ), TEXT( "wt" ) );
	//_ftprintf( fp1, TEXT( "<?xml version=\"1.0\" standalone=\"yes\"?>" )  );
	//_ftprintf( fp1, TEXT( "<!-- This is an xml log file for a3dLandscape class.\nCreated by Abazyan Arsen (ars_s) for 3D engine.\nAll rights reserved. Tula 2006.-->" )  );
	//_ftprintf( fp1, TEXT( "<a3dLandscape>" ) );

	int	i;

	for( i = 0; i < this->land->varianceCount; i ++ ) { 
		this->variance[ i ] = this->calcVariance(i);
	}

	//this->recurseCalcVariance( this->beginRow, this->beginCol, this->land->quadCountPerPatchSide, 0 );
	
	//_ftprintf( fp1, TEXT( "</a3dLandscape>" ) );
	//fclose( fp1 );
		
	return ;
}

void landscapePatch::calcDelta() { 
	int row, col;
	
	for( row = 0; row < this->land->quadCountPerPatchSide + 1; row ++ ) { 
		for( col = 0; col < this->land->quadCountPerPatchSide + 1; col ++ ) { 
			this->deltaMap[row][col] = 0.0f;
		} 
	} 
	this->recurseCalcDelta( this->beginRow, this->beginCol, this->land->quadCountPerPatchSide, 0 );
}

void landscapePatch::recurseCalcDelta( int upperLeftRow, int upperLeftCol, int sideSize, int node )
{
	int				centerRow, centerCol;
	float			interpolatedY;

	if( sideSize == 1 )
		return ;
		
	centerRow	= upperLeftRow + ( sideSize >> 1 );
	centerCol	= upperLeftCol + ( sideSize >> 1 );
		
	// вычисляем приближенную z
	interpolatedY		= this->land->heightMap[ upperLeftRow ][ upperLeftCol ].position[1] + land->heightMap[ upperLeftRow + sideSize ][ upperLeftCol + sideSize ].position[1];
	interpolatedY		*= 0.5f;
	this->deltaMap[centerRow - this->beginRow][centerCol - this->beginCol] = this->land->heightMap[ centerRow ][ centerCol ].position[1] - interpolatedY;

	interpolatedY		= this->land->heightMap[ upperLeftRow ][ upperLeftCol ].position[1] + land->heightMap[ upperLeftRow ][ upperLeftCol + sideSize ].position[1];
	interpolatedY		*= 0.5f;
	this->deltaMap[upperLeftRow  - this->beginRow][centerCol - this->beginCol] = this->land->heightMap[ upperLeftRow ][ centerCol ].position[1] - interpolatedY;
	
	interpolatedY		= this->land->heightMap[ upperLeftRow ][ upperLeftCol + sideSize ].position[1] + land->heightMap[ upperLeftRow + sideSize ][ upperLeftCol + sideSize ].position[1];
	interpolatedY		*= 0.5f;
	this->deltaMap[centerRow  - this->beginRow][upperLeftCol + sideSize - this->beginCol] = this->land->heightMap[ centerRow ][ upperLeftCol + sideSize ].position[1] - interpolatedY;

	interpolatedY		= this->land->heightMap[ upperLeftRow + sideSize ][ upperLeftCol + sideSize ].position[1] + land->heightMap[ upperLeftRow + sideSize ][ upperLeftCol ].position[1];
	interpolatedY		*= 0.5f;
	this->deltaMap[upperLeftRow + sideSize - this->beginRow][centerCol - this->beginCol] = this->land->heightMap[ upperLeftRow + sideSize ][ centerCol ].position[1] - interpolatedY;

	interpolatedY		= this->land->heightMap[ upperLeftRow + sideSize ][ upperLeftCol ].position[1] + land->heightMap[ upperLeftRow ][ upperLeftCol ].position[1];
	interpolatedY		*= 0.5f;
	this->deltaMap[centerRow - this->beginRow][upperLeftCol - this->beginCol] = this->land->heightMap[ centerRow ][ upperLeftCol ].position[1] - interpolatedY;

	// здесь мы выбираем максимально отклонение
	// между подсчитанным отклонением и отклонениями потомков
	// левых и правых
	//_ftprintf( fp1, TEXT( "<n_%d>" ), node );
	
	// проходимся по 4 подквадратам
	this->recurseCalcDelta( upperLeftRow, upperLeftCol, sideSize >> 1, node + 1 );
	this->recurseCalcDelta( upperLeftRow, upperLeftCol + ( sideSize >> 1 ), sideSize >> 1, node + 1 );
	this->recurseCalcDelta( upperLeftRow + ( sideSize >> 1 ), upperLeftCol, sideSize >> 1, node + 1 );
	this->recurseCalcDelta( upperLeftRow + ( sideSize >> 1 ), upperLeftCol + ( sideSize >> 1 ), sideSize >> 1, node + 1 );

	//_ftprintf( fp1, TEXT( "%f</n_%d>" ), deltaY, node );
	// теперь сохраняем отклонение
	//this->variance[ node ]		= max( this->variance[ node ], deltaY );
	return ;
}

void landscapePatch::getNearVertex( int *vertexRow, int *vertexCol )
{
	// необходимо определить ближайшую точку на границе ландшафта
	// определяем ближайшую точку камере
	
	// если камера находится сверху
	if( this->land->cameraPatchRow < this->rowIndex )
	{
		*vertexRow = this->beginRow;
		
		// и слева
		if( this->land->cameraPatchCol < this->colIndex )
			*vertexCol = this->beginCol;
		
		// или справа
		else if( this->land->cameraPatchCol > this->colIndex )
			*vertexCol = this->beginCol + this->land->quadCountPerPatchSide;
		
		// камера находится в том же столбце что и кусок
		else
			*vertexCol = (int)(this->land->cameraPos[0] / this->land->sideScale);
	}
	// а если камера находится снизу
	else if( this->land->cameraPatchRow > this->rowIndex )
	{
		*vertexRow = this->beginRow + this->land->quadCountPerPatchSide;
		
		// и слева
		if( this->land->cameraPatchCol < this->colIndex )
			*vertexCol = this->beginCol;
		
		// или справа
		else if( this->land->cameraPatchCol > this->colIndex )
			*vertexCol = this->beginCol + this->land->quadCountPerPatchSide;
		
		// камера находится в том же столбце что и кусок			
		else
			*vertexCol = (int)(this->land->cameraPos[0] / this->land->sideScale);
	}
	// камера находится в той же строке, что и кусок
	else
	{
		*vertexRow = - (int)(this->land->cameraPos[2] / this->land->sideScale);
		
		// если камера находится левее данного куска
		if( this->land->cameraPatchCol < this->colIndex )
			*vertexCol = this->beginCol;
		
		// если камера находится правее данного куска
		else if( this->land->cameraPatchCol > this->colIndex )
			*vertexCol = this->beginCol + this->land->quadCountPerPatchSide;

		else
			*vertexCol = (int)(this->land->cameraPos[0] / this->land->sideScale);
	}

	return ;
}

bool __a3d_fastcall	landscapePatch::shouldCalcDetalizationLevel() { 
	
	return this->land->enableTesselation;	
}

float __a3d_fastcall landscapePatch::calcUpdateValue(int level) { 
	
	if( level == 0 ) return 0.0f;

	float	x  = ( this->variance[level - 1] - this->variance[level] ) * (this->land->pixelError - this->error[level]);
			x /= ( this->error[level - 1] - this->error[level] );
			x += this->variance[level];

	if( !(x < this->variance[level - 1] && x > this->variance[level]) ) {
		x = 0.0f;			
	}
	
	return (x - this->variance[level]) / (this->variance[level - 1] - this->variance[level]);
}

void landscapePatch::calcDetalizationLevel()
{
	
	if(!this->shouldCalcDetalizationLevel()) return;

	int		vertexRow, vertexCol, detLevel;	
	float	len;
	Vector3	dist, pos;
	Vector4 dst;

	this->getNearVertex( &vertexRow, &vertexCol );
	
	bool maxDetail = false;
	// если принудительно ставить высокий уровень месту где находится камера
	if( this->land->forceMaxDetailCurrentPatch && this->land->isCameraInLand() ) { 
		// если камера находится в том же патче
		if( &this->land->patches[ this->land->cameraPatchRow ][ this->land->cameraPatchCol ] == this ) {
			maxDetail = true;
			detLevel = this->land->varianceCount - 1;
			this->error[ detLevel ] = this->land->pixelError  + 1.0f;
		}
		
	}
	
	if( !maxDetail ) {
	
		// теперь вычисляем дистанцию от камеры, до данной вершины
		subVector3( this->land->heightMap[vertexRow][vertexCol].position, this->land->currCamera->frustumApex, dist ); 			
		// вычисляем дистанцию
		len		= ulsqrt( dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2] );
		// вектор
		setVector3(pos, 0.0f, 0.0f, len);

		// теперь проходимся по всем уровням
		for( detLevel = 0; detLevel < this->land->varianceCount - 1; detLevel ++ )
		{
			pos[1] = this->variance[detLevel];
			mulVector3ToMatrix4(pos, this->land->currCamera->proj, dst);
			this->error[detLevel] = dst[1] / dst[3];
			if( this->error[detLevel] <= this->land->pixelError )
				break;
		}

		// еще один уровень подсчитаем
		if(detLevel < this->land->varianceCount - 1) {
			pos[1] = this->variance[detLevel + 1];
			mulVector3ToMatrix4(pos, this->land->currCamera->proj, dst);
			this->error[detLevel + 1] = dst[1] / dst[3];
		}
	}
	
	// переход на уровень не будет скачкообразным
	// здесь высчитываем дополнительный сдвиг
	if(detLevel == this->land->varianceCount - 1 && this->error[detLevel] > this->land->pixelError ) this->updateValue = 0.0f;
	else this->updateValue = this->calcUpdateValue(detLevel);
	if(detLevel == 0 && this->updateValue < 0.7f) detLevel++; 
	else if(detLevel == this->land->varianceCount - 1) { 
		if(this->updateValue < 0.2f) this->updateValue = 0.0f;
	}
	else detLevel ++;
	
	if( this->detalizationLevel != detLevel )
	{
		this->update				= true;
		this->detalizationLevel		= detLevel;
		this->realDetalizationLevel = detLevel;
		this->deltaThis				= 1 << ( this->land->varianceCount - this->realDetalizationLevel - 1 );
	}
		
	return ;
}

void landscapePatch::checkVisibility()
{
	
	Vector3 basis;
	if( land->basis[3][0] == 0.0f && land->basis[3][1] == 0.0f && land->basis[3][2] == 0.0f ) {
		if( this->land->currCamera->isSphereInFrustum( this->boundSphere ) == OUT_FRUSTUM || 
			this->land->currCamera->isBoxInFrustum( this->boundBox ) == OUT_FRUSTUM )
			this->visible = false;
		else	
			this->visible = true;
	}
	else {
		setVector3(basis, land->basis[3][0], land->basis[3][1], land->basis[3][2]);

		if( this->land->currCamera->isSphereInFrustum( this->boundSphere, basis ) == OUT_FRUSTUM || 
			this->land->currCamera->isBoxInFrustum( this->boundBox, basis ) == OUT_FRUSTUM )
			this->visible = false;
		else	
			this->visible = true;
	}
	
	return ;
}

void landscapePatch::calcMinMaxHeight( float *minHeight, float *maxHeight )
{

	int		i, j;
	
	*minHeight = FLT_MAX;
	*maxHeight = FLT_MIN;

	for( i = 0; i <= this->land->quadCountPerPatchSide; i ++ )
	{
		for( j = 0; j <= this->land->quadCountPerPatchSide; j ++ )
		{
			if( *minHeight > this->land->heightMap[ this->beginRow + i ][ this->beginCol + j ].position[1] )
				*minHeight = this->land->heightMap[ this->beginRow + i ][ this->beginCol + j ].position[1];
			
			if( *maxHeight < this->land->heightMap[ this->beginRow + i ][ this->beginCol + j ].position[1] )
				*maxHeight = this->land->heightMap[ this->beginRow + i ][ this->beginCol + j ].position[1];
		}
	}

	return ;
}

void landscapePatch::calcBoundVolume()
{

	float	minHeight, maxHeight;
	Vector3	dist;

	this->calcMinMaxHeight( &minHeight, &maxHeight );
	
	// создаем ограничивающий куб
	copyVector3( this->land->heightMap[ this->beginRow ][ this->beginCol ].position, this->boundBox[0] ); 
	this->boundBox[0][1] = minHeight;
	copyVector3( this->land->heightMap[ this->beginRow ][ this->beginCol + this->land->quadCountPerPatchSide ].position, this->boundBox[1] ); 
	this->boundBox[1][1] = minHeight;
	copyVector3( this->land->heightMap[ this->beginRow + this->land->quadCountPerPatchSide ][ this->beginCol ].position, this->boundBox[2] ); 
	this->boundBox[2][1] = minHeight;
	copyVector3( this->land->heightMap[ this->beginRow + this->land->quadCountPerPatchSide ][ this->beginCol + this->land->quadCountPerPatchSide ].position, this->boundBox[3] ); 
	this->boundBox[3][1] = minHeight;

	copyVector3( this->land->heightMap[ this->beginRow ][ this->beginCol ].position, this->boundBox[4] ); 
	this->boundBox[4][1] = maxHeight;
	copyVector3( this->land->heightMap[ this->beginRow ][ this->beginCol + this->land->quadCountPerPatchSide ].position, this->boundBox[5] ); 
	this->boundBox[5][1] = maxHeight;
	copyVector3( this->land->heightMap[ this->beginRow + this->land->quadCountPerPatchSide ][ this->beginCol ].position, this->boundBox[6] ); 
	this->boundBox[6][1] = maxHeight;
	copyVector3( this->land->heightMap[ this->beginRow + this->land->quadCountPerPatchSide ][ this->beginCol + this->land->quadCountPerPatchSide ].position, this->boundBox[7] ); 
	this->boundBox[7][1] = maxHeight;

	// создаем ограничивающую сферу
	// координаты
	this->boundSphere[0] = ( this->boundBox[0][0] + this->boundBox[1][0] ) * 0.5f;
	this->boundSphere[2] = ( this->boundBox[0][2] + this->boundBox[2][2] ) * 0.5f;
	this->boundSphere[1] = ( minHeight + maxHeight ) * 0.5f;
	
	// вектор расстояния
	dist[0] = this->boundSphere[0] - this->boundBox[0][0];
	dist[1] = this->boundSphere[1] - this->boundBox[0][1];
	dist[2] = this->boundSphere[2] - this->boundBox[0][2];
	
	// вычисляем радиус
	this->boundSphere[3] = sqrt( dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2] );

	return ;
}

HRESULT	landscapePatch::drawBoundSphere( a3dModel &sphere )
{
	/*
	sphere.Origin();
	sphere.Position( this->boundSphere[0] + this->land->basis[3][0], this->boundSphere[1] + this->land->basis[3][1], this->boundSphere[2] + this->land->basis[3][2] );
	sphere.Scale( this->boundSphere[3], this->boundSphere[3], this->boundSphere[3] );
	sphere.Correct();
	return sphere.render();*/
	return D3D_OK;
}

HRESULT	landscapePatch::drawBoundBox( a3dModel &cube )
{
	/*
	cube.Origin();
	cube.Position( this->boundSphere[0] + this->land->basis[3][0], this->boundBox[0][1] + this->land->basis[3][1], this->boundSphere[2] + this->land->basis[3][2] );
	cube.Scale( this->boundBox[1][0] - this->boundBox[0][0], this->boundBox[4][1] - this->boundBox[0][1], - this->boundBox[2][2] + this->boundBox[0][2] );
	cube.Correct();
	return cube.render();
	*/
	return D3D_OK;
}

HRESULT landscapePatch::initializeRenderBuffers( )
{
	HRESULT		rv;
	TCHAR		tempBuffer[512];
	DWORD		maxVSize, maxISize;
	
	// очищаем предыдущие буфферы
	this->clearRenderBuffers();

	if( this->land->lkrn == NULL )
		return D3DERR_INVALIDCALL;
	
	// выясняем размер буффера вершин и индекса
	maxVSize	= this->land->quadCountPerPatchSide + 1;
	maxVSize	*= maxVSize;

	maxISize	= maxVSize + ( this->land->quadCountPerPatchSide + 1 ) * ( this->land->quadCountPerPatchSide - 1 );
	maxISize	= (DWORD)( maxISize * 1.2f ); // 20 процентов на всякие недоразумения

	this->vertexBufferSize	= maxVSize > this->land->lkrn->Caps.MaxPrimitiveCount * 3 ? this->land->lkrn->Caps.MaxPrimitiveCount * 3: maxVSize;
	this->indexBufferSize	= maxISize > this->land->lkrn->Caps.MaxVertexIndex ? this->land->lkrn->Caps.MaxVertexIndex: maxISize;
	
	if( this->vertexBufferSize < maxVSize ||
		this->indexBufferSize < maxISize )
		return D3DERR_INVALIDCALL;
	
	// создаем массив вершин
	rv = this->land->lkrn->lpDevice->CreateVertexBuffer(	sizeof( landVertex ) * this->vertexBufferSize,	// Length
															D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// используется только для записи и должен находиться в AGP памяти  
															FVF_LAND, 
															D3DPOOL_SYSTEMMEM,
															&this->vertexBuffer,
															NULL );
	if( FAILED( rv ) )
	{
		_stprintf_s( tempBuffer, 512, TEXT( "Error: CreateVertexBuffer() = %s" ), DXGetErrorString9( rv ) );
		MessageBox( this->land->lkrn->hWnd, tempBuffer, TEXT( "a3dLandscape" ), MB_ICONERROR );
		return rv;
	}

	// определяем формат и размер ячейки индекса
	this->indexFormat	= this->indexBufferSize > 0xffff ? D3DFMT_INDEX32 : D3DFMT_INDEX16;
	DWORD indexSize		= this->indexBufferSize > 0xffff ? 4 : 2;

	// создаем массив индексов
	rv = this->land->lkrn->lpDevice->CreateIndexBuffer(	indexSize * this->indexBufferSize,	// Length
															D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,			// используется только для записи и должен находиться в AGP памяти  
															this->indexFormat,
															D3DPOOL_SYSTEMMEM,
															&this->indexBuffer,
															NULL );

	if( FAILED( rv ) )
	{
		_stprintf_s( tempBuffer, 512, TEXT( "Error: CreateIndexBuffer() = %s" ), DXGetErrorString9( rv ) );
		MessageBox( this->land->lkrn->hWnd, tempBuffer, TEXT( "a3dLandscape" ), MB_ICONERROR );
		return rv;
	}

	return D3D_OK;
}

void landscapePatch::clearRenderBuffers()
{

	if( this->vertexBuffer )
	{
		this->vertexBuffer->Release();
		this->vertexBuffer		= NULL;
		this->vertexBufferSize	= 0;
	}

	if( this->indexBuffer )
	{
		this->indexBuffer->Release();
		this->indexBuffer		= NULL;
		this->indexBufferSize	= 0;
		this->indexFormat		= D3DFMT_INDEX32;
	}
	
	return ;
}

HRESULT __a3d_fastcall landscapePatch::openRenderBuffers()
{
	HRESULT rv;

	rv = this->openVertexBuffer();
	if( FAILED( rv ) )
		return rv;

	this->buffersLocked = true;
	return this->openIndexBuffer();
}

HRESULT	__a3d_fastcall landscapePatch::closeRenderBuffers()
{
	
	HRESULT	rv;
	
	rv = this->closeVertexBuffer();
	if( FAILED( rv ) )
		return rv;

	this->buffersLocked = false;
	return this->closeIndexBuffer();
}

void landscapePatch::calcUpdateFlag() 
{ 
	if( this->topPatch != NULL && this->topPatch->deltaThis != this->deltaTop )
		this->update = true;
	else if( this->leftPatch != NULL && this->leftPatch->deltaThis != this->deltaLeft )
		this->update = true;
	else if( this->bottomPatch != NULL && this->bottomPatch->deltaThis != this->deltaBottom )
		this->update = true;
	else if( this->rightPatch != NULL && this->rightPatch->deltaThis != this->deltaRight )
		this->update = true;
}

bool __a3d_fastcall landscapePatch::needUpdate()
{
	if(!this->land->enableTesselation) return false;
	return this->update;
}

void landscapePatch::generateSingle16() 
{
	
	int row, col, endRow, endCol, leftTopCol, index, bottomLeft, upperRight;
	
	leftTopCol = this->beginCol + (this->deltaTop < this->deltaThis? this->deltaTop: this->deltaThis);
	// соединяемся с левой частью
	if(this->deltaLeft < this->deltaThis)
	{
		this->addVertex( this->beginRow, leftTopCol); 
		this->addTopVertexRef( leftTopCol );
		this->addTopVertexRef( this->beginCol, this->vertexIndex );
		index = 1;
		endRow = this->beginRow + this->deltaThis;
		for( row = this->beginRow; row <= endRow; row += this->deltaLeft )
		{
			this->addVertex(row, this->beginCol);
			this->addLeftVertexRef(row);
			this->addIndex16(index);
			this->addIndex16(0);
			index ++;
		}
		bottomLeft = index - 1;
	}
	else {
		this->addVertex(this->beginRow + this->deltaThis, this->beginCol);
		this->addLeftVertexRef(this->beginRow + this->deltaThis);
		this->addVertex(this->beginRow, this->beginCol);
		this->addLeftVertexRef(this->beginRow);
		this->addTopVertexRef(this->beginCol);
		this->addIndex16(0);
		this->addIndex16(1);
		bottomLeft = 0;
		index = 2;
	}

	// соединяемся с верхней частью
	endCol = this->beginCol + this->deltaThis;
	for( col = leftTopCol; col <= endCol; col += this->deltaTop )
	{
		this->addVertex(this->beginRow, col);
		this->addTopVertexRef(col);
		this->addIndex16(bottomLeft);
		this->addIndex16(index);
		index ++;
	}

	upperRight = index - 1;
	if(this->deltaRight < this->deltaThis) {
		col = this->beginCol + this->deltaThis;
		if(this->deltaBottom < this->deltaThis) endRow = this->beginRow + this->deltaThis - this->deltaRight;
		else endRow = this->beginRow + this->deltaThis;

		for( row = this->beginRow; row <= endRow; row += this->deltaRight )
		{
			this->addVertex(row, col);
			this->addIndex16(bottomLeft);
			this->addIndex16(index);
			index ++;
		}
		
		if(this->deltaBottom == this->deltaThis) 
			return ;

		upperRight = index - 1;
		endCol = this->beginCol + this->deltaThis;
		row = this->beginRow + this->deltaThis;
		for ( col = this->beginCol; col <= endCol; col += this->deltaBottom ){
			this->addVertex(row, col);
			this->addIndex16(index);
			this->addIndex16(upperRight);
			index ++;
		}
	}
	else {
		endCol = this->beginCol + this->deltaThis;
		row = this->beginRow + this->deltaThis;
		for ( col = this->beginCol; col <= endCol; col += this->deltaBottom ){
			this->addVertex(row, col);
			this->addIndex16(index);
			this->addIndex16(upperRight);
			index ++;
		}
	}
}

void landscapePatch::generateTopLeft16( )
{

	int	row, col, index;

	if( this->deltaLeft < this->deltaThis )
	{
		this->addVertex( this->beginRow + this->deltaThis, this->beginCol + this->deltaThis );		
		
		index = 1;
		for( row = this->beginRow + this->deltaThis; row >= this->beginRow; row -= this->deltaLeft )
		{
			this->addVertex( row, this->beginCol );
			this->addLeftVertexRef( row );
			this->addIndex16( 0 );
			this->addIndex16( index );

			index ++;
		}
		this->addTopVertexRef( this->beginCol );
	
		// предыдущие вершины 2 - это
		this->prevRowIndicies[ 0 ]			= 1;
		this->prevRowIndicies[ deltaThis ]	= 0;
	
		if( this->deltaTop < this->deltaThis )
		{
			index = this->vertexIndex;
			for( col = this->beginCol + deltaTop; col <= this->beginCol + this->deltaThis; col += this->deltaTop )
			{
				this->addVertex( this->beginRow, col );
				this->addTopVertexRef( col );
				this->addIndex16( 0 );
				this->addIndex16( index );
			
				index ++;
			}
		}
		else
		{
			this->addVertex( this->beginRow, this->beginCol + this->deltaThis );
			this->addTopVertexRef( this->beginCol + this->deltaThis );
			this->addIndex16( 0 );
			this->addIndex16( this->vertexIndex - 1 );
		}
	}
	else if( this->deltaTop < this->deltaThis )
	{
		this->addVertex( this->beginRow + this->deltaThis, this->beginCol );			
		this->addLeftVertexRef( this->beginRow + this->deltaThis );
		this->addVertex( this->beginRow, this->beginCol );
		this->addLeftVertexRef( this->beginRow );
		this->addTopVertexRef( this->beginCol );
		this->addVertex( this->beginRow + this->deltaThis, this->beginCol + this->deltaThis );								
	
		this->addIndex16( 0 );
		this->addIndex16( 1 );
		
		index = 3;
		for( col = this->beginCol + this->deltaTop; col <= this->beginCol + this->deltaThis; col += deltaTop )
		{
			this->addVertex( this->beginRow, col );
			this->addTopVertexRef( col );
			this->addIndex16( 2 );
			this->addIndex16( index );

			index ++;
		}

		this->prevRowIndicies[ 0 ] = 0;
		this->prevRowIndicies[ this->deltaThis ] = 2;
	}
	else
	{
		this->addVertex( this->beginRow + this->deltaThis, this->beginCol );			
		this->addLeftVertexRef( this->beginRow + this->deltaThis );
		this->addVertex( this->beginRow, this->beginCol );
		this->addLeftVertexRef( this->beginRow );
		this->addTopVertexRef( this->beginCol );
		this->addVertex( this->beginRow + this->deltaThis, this->beginCol + this->deltaThis );								
		this->addVertex( this->beginRow, this->beginCol + this->deltaThis );
		this->addTopVertexRef( this->beginCol + this->deltaThis );
	
		this->addIndex16( 0 );
		this->addIndex16( 1 );
		this->addIndex16( 2 );
		this->addIndex16( 3 );

		this->prevRowIndicies[ 0 ] = 0;
		this->prevRowIndicies[ this->deltaThis ] = 2;
	}
	
	return ;
}

void landscapePatch::generateTop16( int col1 )
{
	
	int col, index, origIndex;
	
	if( this->deltaTop < this->deltaThis )
	{
		this->addVertex( this->beginRow + this->deltaThis, this->beginCol + col1 );
		index		= this->vertexIndex;
		origIndex	= index - 1;
				
		for( col = this->beginCol + col1 - this->deltaThis + this->deltaTop; col <= this->beginCol + col1; col += deltaTop )
		{
			this->addVertex( this->beginRow, col );
			this->addTopVertexRef( col );
			this->addIndex16( origIndex );
			this->addIndex16( index );
			index ++;
		}
	
		this->prevRowIndicies[ col1 ] = origIndex;
	}
	else
	{
		this->addVertex( this->beginRow + this->deltaThis, this->beginCol + col1 );
		this->addIndex16( this->vertexIndex - 1 );
		this->addVertex( this->beginRow, this->beginCol + col1 );
		this->addTopVertexRef( this->beginCol + col1 );
		this->addIndex16( this->vertexIndex - 1 );

		this->prevRowIndicies[ col1 ] = this->vertexIndex - 2;
	}

	return ;
}

void landscapePatch::generateTopRight16() { 
	
	int col, row, origIndex;
	
	if( this->deltaTop < this->deltaThis ) 
	{ 
		if( this->deltaRight < this->deltaThis ) {
			
			origIndex = this->vertexIndex - 1;
			col = this->beginCol + this->land->quadCountPerPatchSide;
			row = this->beginRow + this->deltaThis;
			this->prevRowIndicies[this->land->quadCountPerPatchSide] = this->vertexIndex;
			for( ; row > this->beginRow ; row -= this->deltaRight ) { 
				this->addVertex(row, col);
				this->addRightVertexRef(row);
				this->addIndex16(this->vertexIndex - 1);
				this->addIndex16(origIndex);
			}
			origIndex = this->vertexIndex - 1;
			for( col += this->deltaTop - this->deltaThis; col <= this->beginCol + this->land->quadCountPerPatchSide; col += this->deltaTop ) { 
				this->addVertex( this->beginRow, col );
				this->addTopVertexRef( col );
				this->addIndex16( origIndex );
				this->addIndex16( this->vertexIndex - 1 );
			}
			this->addRightVertexRef(this->beginRow);
		}
		else { 
			col = this->beginCol + this->land->quadCountPerPatchSide;
			row = this->beginRow + this->deltaThis;
			this->addVertex( row, col );
			this->addRightVertexRef(row);
			origIndex = this->vertexIndex - 1;
			this->prevRowIndicies[this->land->quadCountPerPatchSide] = origIndex;
			for( col += this->deltaTop - this->deltaThis; col <= this->beginCol + this->land->quadCountPerPatchSide; col += this->deltaTop ) { 
				this->addVertex( this->beginRow, col );
				this->addTopVertexRef( col );
				this->addIndex16( origIndex );
				this->addIndex16( this->vertexIndex - 1 );
			}
			this->addRightVertexRef(this->beginRow);
		}
	}
	else 
	{
		if( this->deltaRight < this->deltaThis ) 
		{
			origIndex = this->vertexIndex - 1;
			col = this->beginCol + this->land->quadCountPerPatchSide;
			row = this->beginRow + this->deltaThis;
			this->prevRowIndicies[this->land->quadCountPerPatchSide] = this->vertexIndex;
			for( ; row >= this->beginRow ; row -= this->deltaRight ) { 
				this->addVertex(row, col);
				this->addRightVertexRef(row);
				this->addIndex16(this->vertexIndex - 1);
				this->addIndex16(origIndex);
			}
			this->addTopVertexRef( col );
		}
		else 
		{
			col = this->beginCol + this->land->quadCountPerPatchSide;
			row = this->beginRow + this->deltaThis;
			this->addVertex( row, col );
			this->addRightVertexRef(row);
			this->addIndex16( this->vertexIndex - 1 );
			this->prevRowIndicies[this->land->quadCountPerPatchSide] = this->vertexIndex - 1;
			this->addVertex( this->beginRow, col ); 
			this->addRightVertexRef(this->beginRow);
			this->addTopVertexRef( col );
			this->addIndex16( this->vertexIndex - 1 );
		}
	}
}
 
/*
void landscapePatch::generateTopRight16()
{
	int col, row, index, origIndex;

	if( this->deltaTop < this->deltaThis )
	{
		col = this->land->quadCountPerPatchSide - this->deltaThis;
		origIndex = this->prevRowIndicies[ col ];
		index = this->vertexIndex;

		for( col += this->beginCol + deltaTop; col <= this->beginCol + this->land->quadCountPerPatchSide - deltaTop; col += deltaTop )
		{
			this->addVertex( this->beginRow, col );
			this->addTopVertexRef( col );
			this->addIndex16( origIndex );
			this->addIndex16( index );
			index ++;
		}
		addRightVertexRef(this->beginRow);

		if( this->deltaRight < this->deltaThis )
		{
			index = this->vertexIndex;
			this->addTopVertexRef( col, this->vertexIndex );
			for( row = this->beginRow; row <= this->beginRow + this->deltaThis; row += this->deltaRight )
			{
				this->addVertex( row, col );
				addRightVertexRef(row);
				this->addIndex16( origIndex );
				this->addIndex16( index );
				index ++;
			}
		
			this->prevRowIndicies[ this->land->quadCountPerPatchSide ] = this->vertexIndex - 1;
		}
		else
		{
			this->addVertex(this->beginRow, col);
			this->addTopVertexRef(col);
			addRightVertexRef(this->beginRow);
			this->addIndex16( origIndex );
			this->addIndex16( this->vertexIndex - 1 );
			
			this->addVertex( this->beginRow + this->deltaThis, col );
			addRightVertexRef(this->beginRow + this->deltaThis);
			this->addIndex16( origIndex );
			this->addIndex16( this->vertexIndex - 1 );

			this->prevRowIndicies[ this->land->quadCountPerPatchSide ] = this->vertexIndex - 1;

		}
	}
	else if( this->deltaRight < this->deltaThis )
	{
		index		= this->vertexIndex;
		
		col			= this->land->quadCountPerPatchSide;
		origIndex	= this->prevRowIndicies[ this->land->quadCountPerPatchSide - this->deltaThis ];
		col			+= this->beginCol;

		this->addTopVertexRef(col, this->vertexIndex);
		for( row = this->beginRow; row <= this->beginRow + this->deltaThis; row += this->deltaRight )
		{
			this->addVertex( row, col );
			addRightVertexRef(row);
			this->addIndex16( origIndex );
			this->addIndex16( index );
		
			index ++;
		}
		
		this->prevRowIndicies[ this->land->quadCountPerPatchSide ] = this->vertexIndex - 1;
	}
	else
	{
		this->addVertex( this->beginRow + this->deltaThis, this->beginCol + this->land->quadCountPerPatchSide );
		addRightVertexRef(this->beginRow + this->deltaThis);
		this->addIndex16( this->vertexIndex - 1 );
		this->addVertex( this->beginRow, this->beginCol + this->land->quadCountPerPatchSide );
		addRightVertexRef(this->beginRow);
		this->addTopVertexRef(this->beginCol + this->land->quadCountPerPatchSide);
		this->addIndex16( this->vertexIndex - 1 );		

		this->prevRowIndicies[ this->land->quadCountPerPatchSide ] = this->vertexIndex - 2;
	}
  
	return ;
}

*/



void landscapePatch::generateLeft16( int row1 )
{

	int	row, index, origIndex, origIndex2;

	if( this->deltaLeft < this->deltaThis )
	{
		this->addVertex( this->beginRow + row1, this->beginCol + this->deltaThis );		
		
		index		= this->vertexIndex;
		origIndex	= index - 1;
		origIndex2	= index;

		for( row = this->beginRow + row1; row > this->beginRow + row1 - this->deltaThis; row -= this->deltaLeft )
		{
			this->addVertex( row, this->beginCol );
			this->addLeftVertexRef( row ); 
			this->addIndex16( origIndex );
			this->addIndex16( index );

			index ++;
		}
	
		this->addIndex16( origIndex );
		this->addIndex16( this->prevRowIndicies[ 0 ] );

		this->addIndex16( origIndex );
		this->addIndex16( this->prevRowIndicies[ this->deltaThis ] );
		
		// предыдущие вершины 2 - это
		this->prevRowIndicies[ 0 ]					= origIndex2;
		this->prevRowIndicies[ this->deltaThis ]	= origIndex;
			
	}
	else
	{
		this->addVertex( this->beginRow + row1, this->beginCol );
		this->addLeftVertexRef( this->beginRow + row1 );
		this->addIndex16( this->vertexIndex - 1 );
		this->addIndex16( this->prevRowIndicies[ 0 ] );
		this->addVertex( this->beginRow + row1, this->beginCol + this->deltaThis );
		this->addIndex16( this->vertexIndex - 1 );
		this->addIndex16( this->prevRowIndicies[ this->deltaThis ] );

		this->prevRowIndicies[ 0 ]					= this->vertexIndex - 2;
		this->prevRowIndicies[ this->deltaThis ]	= this->vertexIndex - 1;
	}
	
	return ;
}

void landscapePatch::generateRight16( int row1 )
{

	int row, col, endRow, origIndex;

	if( this->deltaRight < this->deltaThis )
	{
		col = this->beginCol + this->land->quadCountPerPatchSide;
		endRow = this->beginRow + row1 - this->deltaThis;
		origIndex = this->vertexIndex;
		for( row = this->beginRow + row1; row > endRow; row -= this->deltaRight ) { 
			this->addVertex( row, col );
			this->addRightVertexRef(row);
			this->addIndex16( this->vertexIndex - 1 );
			this->addIndex16( this->prevTopCol );
		}
		this->addIndex16( this->prevRowIndicies[ this->land->quadCountPerPatchSide ] );
		this->addIndex16( this->prevTopCol );
		this->prevRowIndicies[ this->land->quadCountPerPatchSide ] = origIndex;
	}
	else
	{
		this->addVertex( this->beginRow + row1, this->beginCol + this->land->quadCountPerPatchSide );
		this->addRightVertexRef( this->beginRow + row1 );
		this->addIndex16( this->vertexIndex - 1 );
		this->addIndex16( this->prevRowIndicies[ this->land->quadCountPerPatchSide ] );

		this->prevRowIndicies[ this->land->quadCountPerPatchSide ] = this->vertexIndex - 1;
	}
	
	return ;
}

void landscapePatch::generateBottomLeft16() { 

	int row, col, origIndex, index, endRow;
	
	if( this->deltaLeft < this->deltaThis ) 
	{ 
		if( this->deltaBottom < this->deltaThis )
		{
			// необходимо корректировать после последнего вызова link16
			// для этого добавляем вершину
			row = this->beginRow + this->land->quadCountPerPatchSide;
			this->addVertex( row, this->beginCol);
			this->addLeftVertexRef( row );
			this->addBottomVertexRef( this->beginCol );
			this->wordIndexPtr[ this->indexIndex - 1 ] = (WORD)this->vertexIndex - 1;
			this->addIndex16( this->vertexIndex - 1 );
			this->addVertex( row - this->deltaLeft, this->beginCol );
			this->addLeftVertexRef( row - this->deltaLeft );
			this->addIndex16(this->vertexIndex - 1);
			origIndex = this->vertexIndex - 1;
			for( col = this->beginCol + deltaBottom; col <= this->beginCol + this->deltaThis; col += this->deltaBottom ) { 
				this->addVertex(row, col);
				this->addBottomVertexRef( col );
				this->addIndex16(this->vertexIndex - 1);
				this->addIndex16(origIndex);
			}
			origIndex = this->vertexIndex - 1;
			endRow = row - this->deltaThis;
			for( row -= (this->deltaLeft << 1); row > endRow; row -= this->deltaLeft ) { 
				this->addIndex16( origIndex );
				this->addVertex(row, this->beginCol);
				this->addLeftVertexRef( row );
				this->addIndex16( this->vertexIndex - 1);
			}
			
			this->addIndex16( origIndex );
			this->addIndex16( this->prevRowIndicies[0] );
			this->addIndex16( origIndex );
			this->addIndex16( this->prevRowIndicies[this->deltaThis] );
			this->addIndex16( origIndex );
			this->addIndex16( this->prevRowIndicies[this->deltaThis] );
		}
		else {
			row = this->beginRow + this->land->quadCountPerPatchSide;
			this->addVertex( row, this->beginCol + this->deltaThis );
			this->addBottomVertexRef( this->beginCol + this->deltaThis );
			origIndex = this->wordIndexPtr[ this->indexIndex - 1 ] = (WORD)this->vertexIndex - 1;
			this->addIndex16( origIndex );
			endRow = row - this->deltaThis;
			this->addBottomVertexRef( this->beginCol, this->vertexIndex );
			for( ; row > endRow; row -= this->deltaLeft) { 
				this->addVertex( row, this->beginCol );
				this->addLeftVertexRef( row );
				this->addIndex16( this->vertexIndex - 1 );
				this->addIndex16( origIndex );
			}
			this->addIndex16( this->prevRowIndicies[0] );
			this->addIndex16( origIndex );
			this->addVertex( this->beginRow + this->land->quadCountPerPatchSide - this->deltaThis, this->beginCol + this->deltaThis );
			this->addIndex16( this->vertexIndex - 1 );
			this->addIndex16( origIndex );
			this->addIndex16( this->vertexIndex - 1 );
		}
	}
	else {
		if( this->deltaBottom < this->deltaThis ) 
		{ 
			row = this->beginRow + this->land->quadCountPerPatchSide;
			col = this->beginCol;
			origIndex = this->prevRowIndicies[ 0 ];

			// необходимо корректирвать после последнего вызова link16
			//this->wordIndexPtr[ this->indexIndex - 1 ] = origIndex;
			
			this->addVertex( row, col );
			this->addLeftVertexRef( row );
			this->addBottomVertexRef( col );
			this->addIndex16( this->vertexIndex - 1 );
			this->addIndex16( origIndex );
			this->prevRowIndicies[ 0 ] = this->vertexIndex - 1;
		
			index = this->vertexIndex;
			for( col = this->beginCol + this->deltaBottom; col < this->beginCol + this->deltaThis; col += this->deltaBottom )
			{
				this->addVertex( row, col );
				this->addBottomVertexRef( col );
				this->addIndex16( index );
				this->addIndex16( origIndex );

				index ++;
			}
		
			this->addVertex( row, col );
			this->addBottomVertexRef( col );
			this->addIndex16( this->vertexIndex - 1 );
			this->addIndex16( this->prevRowIndicies[ this->deltaThis ] );
			
			this->prevRowIndicies[ this->deltaThis ] = this->vertexIndex - 1;		
		}
		else 
		{
			this->addVertex( this->beginRow + this->land->quadCountPerPatchSide, this->beginCol );
			this->addBottomVertexRef( this->beginCol );
			this->addLeftVertexRef( this->beginRow + this->land->quadCountPerPatchSide );
			this->addIndex16( this->vertexIndex - 1 );
			this->addIndex16( this->prevRowIndicies[ 0 ] );
			this->prevRowIndicies[ 0 ] = this->vertexIndex - 1;
			
			this->addVertex( this->beginRow + this->land->quadCountPerPatchSide, this->beginCol + this->deltaThis );
			this->addBottomVertexRef( this->beginCol + this->deltaThis );
			this->addIndex16( this->vertexIndex - 1 );
			this->addIndex16( this->prevRowIndicies[ this->deltaThis ] );
			this->prevRowIndicies[ this->deltaThis ] = this->vertexIndex - 1;
		}
	}
}

void landscapePatch::generateBottom16( int col1 )
{
	
	int row, col, index, origIndex;

	if( this->deltaBottom < this->deltaThis )
	{
		origIndex = this->wordIndexPtr[ this->indexIndex - 1 ];
	
		row = this->beginRow + this->land->quadCountPerPatchSide;
		index = this->vertexIndex;

		for( col = this->beginCol + col1 - this->deltaThis + this->deltaBottom; col < this->beginCol + col1; col += this->deltaBottom )
		{
			this->addVertex( row, col );
			this->addBottomVertexRef( col );
			this->addIndex16( index );
			this->addIndex16( origIndex );
		
			index ++;
		}
	
		this->addVertex( row, col );
		this->addBottomVertexRef( col );
		this->addIndex16( this->vertexIndex - 1 );
		this->addIndex16( this->prevRowIndicies[ col1 ] );
		
		this->prevRowIndicies[ col1 ] = this->vertexIndex - 1;
	}
	else
	{
		this->addVertex( this->beginRow + this->land->quadCountPerPatchSide, this->beginCol + col1 );
		this->addBottomVertexRef( this->beginCol + col1 );
		this->addIndex16( this->vertexIndex - 1 );
		this->addIndex16( this->prevRowIndicies[ col1 ] );
		this->prevRowIndicies[ col1 ] = this->vertexIndex - 1;
	}
	
	return ;
}

void landscapePatch::generateBottomRight16()
{

	int row, col, index, origIndex;

	origIndex = this->wordIndexPtr[ this->indexIndex - 1 ];
	
	if( this->deltaBottom < this->deltaThis )
	{
		row		= this->beginRow + this->land->quadCountPerPatchSide;
		index	= this->vertexIndex;

		for( col = this->beginCol + this->land->quadCountPerPatchSide - this->deltaThis + this->deltaBottom; col < this->beginCol + this->land->quadCountPerPatchSide; col += this->deltaBottom )
		{
			this->addVertex( row, col );
			this->addBottomVertexRef( col );
			this->addIndex16( index );
			this->addIndex16( origIndex );
		
			index ++;
		}
	
		if( this->deltaRight < this->deltaThis )
		{
			col = this->beginCol + this->land->quadCountPerPatchSide;
			this->addBottomVertexRef(col, this->vertexIndex);
			for( row = this->beginRow + this->land->quadCountPerPatchSide; row > this->beginRow + this->land->quadCountPerPatchSide - this->deltaThis; row -= this->deltaRight )  
			{
				this->addVertex( row, col );
				this->addRightVertexRef( row );
				this->addIndex16( index );
				this->addIndex16( origIndex );
			
				index ++;
			}
		
			this->addIndex16( this->prevRowIndicies[ this->land->quadCountPerPatchSide ] );
			this->addIndex16( origIndex ); 
		}
		else
		{
			this->addVertex( row, col );
			this->addRightVertexRef( row );
			this->addBottomVertexRef( col );
			this->addIndex16( this->vertexIndex - 1 );
			this->addIndex16( this->prevRowIndicies[ this->land->quadCountPerPatchSide ] );
		}
	}
	else if( this->deltaRight < this->deltaThis )
	{
		index = this->vertexIndex;
		
		col = this->beginCol + this->land->quadCountPerPatchSide;
		this->addBottomVertexRef( col, this->vertexIndex );
		for( row = this->beginRow + this->land->quadCountPerPatchSide; row > this->beginRow + this->land->quadCountPerPatchSide - this->deltaThis; row -= this->deltaRight )
		{
			this->addVertex( row, col );
			this->addRightVertexRef( row );
			this->addIndex16( index );
			this->addIndex16( origIndex );
		
			index ++;
		}
	
		this->addIndex16( this->prevRowIndicies[ this->land->quadCountPerPatchSide ] );
		this->addIndex16( origIndex );
	}
	else
	{
		this->addVertex( this->beginRow + this->land->quadCountPerPatchSide, this->beginCol + this->land->quadCountPerPatchSide );
		this->addRightVertexRef( this->beginRow + this->land->quadCountPerPatchSide );
		this->addBottomVertexRef( this->beginCol + this->land->quadCountPerPatchSide );
		this->addIndex16( this->vertexIndex - 1 );
		this->addIndex16( this->prevRowIndicies[ this->land->quadCountPerPatchSide ] );
	}
	
	return ;
}

HRESULT __a3d_fastcall landscapePatch::updateBlendVertByNeighboors() {
	
	bool	shouldUnlockBuffer = false;
	HRESULT rv;

	// todo оптимизировать - некоторые ландшафты необязательно обновлять

	int tmpVertexIndex = this->vertexIndex;
	if( !this->buffersLocked ) { 
		rv = this->openVertexBuffer(false);
		this->vertexIndex = tmpVertexIndex;
		if( FAILED( rv ) )
			return rv;
		shouldUnlockBuffer = true;
	}
	
	int index, *rowThis, *colThis, deltaBig;
	landVertex **vMap = this->land->heightMap;
	landVertex *vRow;
	float uc;
	
	// обновляем верх
	if( this->topPatch != NULL && this->deltaTop <= this->deltaThis/*todo докончить!!!*/) { 
		rowThis	= this->vertexRefMap[TOPVERTEXROW]; 
		vRow = vMap[this->beginRow];
		uc = topPatch->updateValue;
		deltaBig = this->deltaTop << 1;
		for( index = 0; index < this->land->quadCountPerPatchSide; index += deltaBig) {
			this->vertexPtr[rowThis[index]].position[1] =  vRow[index + this->beginCol].position[1];
			this->vertexPtr[rowThis[index + this->deltaTop]].position[1] = vRow[index + this->deltaTop + this->beginCol].position[1] - uc * vRow[index + this->deltaTop + this->beginCol].deltaY;
			this->vertexPtr[rowThis[index]].deltaY = 0.0f;
			this->vertexPtr[rowThis[index + this->deltaTop]].deltaY = 0.0f;
		}
		this->vertexPtr[rowThis[index]].position[1] = vRow[index + this->beginCol].position[1];
		this->vertexPtr[rowThis[index]].deltaY = 0.0f;
	}

	// обновляем лево
	if( this->leftPatch != NULL && this->deltaLeft <= this->deltaThis/*todo докончить!!!*/) { 
		colThis = this->vertexRefMap[LEFTVERTEXROW];
		uc = leftPatch->updateValue;
		deltaBig = this->deltaLeft << 1;
		for( index = 0; index < this->land->quadCountPerPatchSide; index += deltaBig ) {
			this->vertexPtr[colThis[index]].position[1] =  vMap[this->beginRow + index][this->beginCol].position[1];
			this->vertexPtr[colThis[index + this->deltaLeft]].position[1] = vMap[this->beginRow + this->deltaLeft + index][this->beginCol].position[1] - uc * vMap[this->beginRow + this->deltaLeft + index][this->beginCol].deltaY;
			this->vertexPtr[colThis[index]].deltaY = 0.0f;
			this->vertexPtr[colThis[index + this->deltaLeft]].deltaY = 0.0f;
		}
		this->vertexPtr[colThis[index]].position[1] =  vMap[this->beginRow + index][this->beginCol].position[1];
		this->vertexPtr[colThis[index]].deltaY = 0.0f;
	}
	
	// обновляем низ
	if( this->bottomPatch != NULL && this->deltaBottom < this->deltaThis) { 
		rowThis	= this->vertexRefMap[BOTTOMVERTEXROW]; 
		vRow = vMap[this->beginRow + this->land->quadCountPerPatchSide];
		uc = this->bottomPatch->updateValue;
		deltaBig = this->deltaBottom << 1;
		for( index = 0; index < this->land->quadCountPerPatchSide; index += deltaBig) {
			this->vertexPtr[rowThis[index]].position[1] =  vRow[index + this->beginCol].position[1];
			this->vertexPtr[rowThis[index + this->deltaBottom]].position[1] = vRow[index + this->deltaBottom + this->beginCol].position[1] - uc * vRow[index + this->deltaBottom + this->beginCol].deltaY;
			this->vertexPtr[rowThis[index]].deltaY = 0.0f;
			this->vertexPtr[rowThis[index + this->deltaBottom]].deltaY = 0.0f;
		}
		this->vertexPtr[rowThis[index]].position[1] = vRow[index + this->beginCol].position[1];
		this->vertexPtr[rowThis[index]].deltaY = 0.0f;
	}

	// обновляем вправо
	if( this->rightPatch != NULL && this->deltaRight < this->deltaThis/*todo докончить!!!*/) { 
		colThis = this->vertexRefMap[RIGHTVERTEXROW];
		uc = rightPatch->updateValue;
		deltaBig = this->deltaRight << 1;
		for( index = 0; index < this->land->quadCountPerPatchSide; index += deltaBig ) {
			this->vertexPtr[colThis[index]].position[1] =  vMap[this->beginRow + index][this->beginCol + this->land->quadCountPerPatchSide].position[1];
			this->vertexPtr[colThis[index + this->deltaRight]].position[1] = vMap[this->beginRow + this->deltaRight + index][this->beginCol + this->land->quadCountPerPatchSide].position[1] - uc * vMap[this->beginRow + this->deltaRight + index][this->beginCol + this->land->quadCountPerPatchSide].deltaY;
			this->vertexPtr[colThis[index]].deltaY = 0.0f;
			this->vertexPtr[colThis[index + this->deltaRight]].deltaY = 0.0f;
		}
		this->vertexPtr[colThis[index]].position[1] =  vMap[this->beginRow + index][this->beginCol + this->land->quadCountPerPatchSide].position[1];
		this->vertexPtr[colThis[index]].deltaY = 0.0f;
	}

	if( shouldUnlockBuffer ) { 
		rv = this->closeVertexBuffer();
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

void landscapePatch::calcNeighboorDelta() {
	
	this->deltaTop	= this->deltaLeft = this->deltaRight = this->deltaBottom = this->deltaThis;

	if( this->topPatch != NULL )
	{
		this->deltaTop = this->topPatch->deltaThis;
	}
	if( this->leftPatch != NULL )
	{
		this->deltaLeft = this->leftPatch->deltaThis;
	}
	if( this->rightPatch != NULL )
	{
		this->deltaRight = this->rightPatch->deltaThis;
	}
	if( this->bottomPatch != NULL )
	{
		this->deltaBottom = this->bottomPatch->deltaThis;
	}
}

void landscapePatch::clearVertexRefIndicies() { 
	
	for( int i = 0; i < this->land->quadCountPerPatchSide + 1; i ++) {
		this->vertexRefMap[0][i] = -1;
		this->vertexRefMap[1][i] = -1;
		this->vertexRefMap[2][i] = -1;
		this->vertexRefMap[3][i] = -1;
	}
}

void landscapePatch::generate16()
{

	int		row, col;
	
	this->clearVertexRefIndicies();	
	if(this->deltaThis == this->land->quadCountPerPatchSide)
	{
		this->generateSingle16();
#ifdef TEST_PATCH
		this->testPatchSoftwareBlend();
#endif		
		return;
	}
	
	// верхний левый угол
	this->generateTopLeft16();

	// генерируем верхнюю часть
	for( col = ( this->deltaThis << 1 ); col < this->land->quadCountPerPatchSide; col += this->deltaThis )
		this->generateTop16( col );

	// генерируем правый верхний угол
	this->generateTopRight16();
	
	this->link16();

	// по каждой строке
	for( row = ( this->deltaThis << 1 ); row < this->land->quadCountPerPatchSide; row += this->deltaThis )
	{
		// левая часть
		this->generateLeft16( row );
		
		// тело 
		for( col = ( this->deltaThis << 1 ); col < this->land->quadCountPerPatchSide; col += this->deltaThis )
			this->generateMiddle16( row, col );

		// правая часть
		this->generateRight16( row );
		
		// связываем
		this->link16();
	}

	// левая нижняя часть
	this->generateBottomLeft16();
	
	// генерируем нижнюю часть
	for( col = ( this->deltaThis << 1 ); col < this->land->quadCountPerPatchSide; col += this->deltaThis )
		this->generateBottom16( col );
		
	// нижняя правая часть
	this->generateBottomRight16();
	
#ifdef TEST_PATCH
	this->testPatchSoftwareBlend();
#endif

	return ;
}

void landscapePatch::generate32()
{

	deltaThis	= 1 << ( this->land->varianceCount - this->detalizationLevel - 1 );
	deltaTop	= deltaLeft = deltaRight = deltaBottom = deltaThis;

	if( this->topPatch != NULL )
		deltaTop = 1 << ( this->land->varianceCount - this->topPatch->detalizationLevel );
	if( this->leftPatch != NULL )
		deltaLeft = 1 << ( this->land->varianceCount - this->leftPatch->detalizationLevel );
	if( this->rightPatch != NULL )
		deltaRight = 1 << ( this->land->varianceCount - this->rightPatch->detalizationLevel );
	if( this->bottomPatch != NULL )
		deltaBottom = 1 << ( this->land->varianceCount - this->bottomPatch->detalizationLevel );
	
	return ;
}

void landscapePatch::calcBlendParams(int delta) { 
	
	// копируем параметры
	this->copyBlendParams(delta);

	// верхний правый угол
	//this->calcBlendParamsTopRight(delta);

	// генерируем нижний левый
	//this->calcBlendParamsBottomLeft(delta);

}

void landscapePatch::testPatch() { 
	
	DWORD vi;
	for( vi = 0; vi < this->vertexIndex; vi ++ ) {
		if(this->vertexPtr[vi].deltaY > 1000 || this->vertexPtr[vi].deltaY < -1000) { 
			MessageBox(this->land->lkrn->hWnd, TEXT("deltaY < -100"), TEXT(""), MB_OK);
		}
	}
}

void landscapePatch::testPatchSoftwareBlend() { 
	
	// тестируем верх
	int delta = this->deltaThis > this->deltaTop? this->deltaTop: this->deltaThis;
	int i;

	for(i = 0; i <= this->land->quadCountPerPatchSide; i += delta) {
		if( this->vertexRefMap[TOPVERTEXROW][i] < 0 ) {
			MessageBox(this->land->lkrn->hWnd, TEXT("Top vertex row failed"), TEXT(""), MB_ICONERROR);
		}
	}

	delta = this->deltaThis > this->deltaLeft? this->deltaLeft: this->deltaThis;
	for(i = 0; i <= this->land->quadCountPerPatchSide; i += delta) {
		if( this->vertexRefMap[LEFTVERTEXROW][i] < 0 ) {
			MessageBox(this->land->lkrn->hWnd, TEXT("Left vertex row failed"), TEXT(""), MB_ICONERROR);
		}
	}

	delta = this->deltaThis > this->deltaBottom? this->deltaBottom: this->deltaThis;
	for(i = 0; i <= this->land->quadCountPerPatchSide; i += delta) {
		if( this->vertexRefMap[BOTTOMVERTEXROW][i] < 0 ) {
			MessageBox(this->land->lkrn->hWnd, TEXT("Bottom vertex row failed"), TEXT(""), MB_ICONERROR);
		}
	}
	
	delta = this->deltaThis > this->deltaRight? this->deltaRight: this->deltaThis;
	for(i = 0; i <= this->land->quadCountPerPatchSide; i += delta) {
		if( this->vertexRefMap[RIGHTVERTEXROW][i] < 0 ) {
			MessageBox(this->land->lkrn->hWnd, TEXT("Right vertex row failed"), TEXT(""), MB_ICONERROR);
		}
	}
}

void landscapePatch::calcBlendParams() { 
	
	// * -- 0 -- * -- 0 -- *
	// 0 -- 0 -- 0 -- 0 -- 0
	// * -- 0 -- * -- 0 -- *
	// 0 -- 0 -- 0 -- 0 -- 0
	// * -- 0 -- * -- 0 -- *
	
	if(!this->update) return;
	this->calcBlendParams( 1 << (this->land->varianceCount - this->realDetalizationLevel) );

	return ;
}

void landscapePatch::copyBlendParams(int delta) {
	
	int row, col;
	int endRow = this->beginRow + this->land->quadCountPerPatchSide;
	int endCol = this->beginCol + this->land->quadCountPerPatchSide;
	int lessDelta = delta >> 1;

	landVertex	**hMap = this->land->heightMap;
	
	// * -- 0 -- * -- 0 -- *
	// 0 -- 0 -- 0 -- 0 -- 0
	// * -- 0 -- * -- 0 -- *
	// 0 -- 0 -- 0 -- 0 -- 0
	// * -- 0 -- * -- 0 -- *
	
	for(row = this->beginRow; row <= endRow; row += delta ) {
		for( col = this->beginCol; col < endCol; col += delta ) {
			hMap[row][col].deltaY = 0.0f;
			hMap[row][col + lessDelta].deltaY = this->deltaMap[row - this->beginRow][col - this->beginCol + lessDelta];
		}
		hMap[row][col].deltaY = 0.0f;
	}
	for( row = this->beginRow + lessDelta; row < endRow ; row += delta) { 
		for( col = this->beginCol; col <= endCol; col += lessDelta) { 
			hMap[row][col].deltaY = this->deltaMap[row - this->beginRow][col - this->beginCol];
		}
	}
}

void landscapePatch::testVertexRefIndices() { 
	
	

}

HRESULT __a3d_fastcall landscapePatch::tesselate()
{

	// итак.... проверяем сначала чей уровень детализации больше 
	HRESULT	rv;

	// если ни один из них не изменился
	// ничего обновлять не надо
	if( !this->needUpdate() || !this->visible )
		return D3D_OK;
	
	// открываем вертек и индекс буфферы
	rv = this->openRenderBuffers();
	if( FAILED( rv ) )
		return rv;
	
	if( this->indexFormat == D3DFMT_INDEX16 )
		this->generate16();
	else
		this->generate32();

	rv = this->closeRenderBuffers();
	if( FAILED( rv ) )
		return rv;

	// один раз обновили и хорош
	this->update = false;
	
	return D3D_OK;
}

HRESULT __a3d_fastcall landscapePatch::render(int passIndex)
{
	HRESULT	rv;

	rv = this->land->lkrn->lpDevice->SetStreamSource( 0, this->vertexBuffer, 0, sizeof( landVertex ) );
	if( FAILED( rv ) )
		return rv;

	rv = this->land->lkrn->lpDevice->SetFVF( FVF_LAND );
	if( FAILED( rv ) )
		return rv;
	
	rv = this->land->lpEffect->SetFloat( (D3DXHANDLE)"blendCount", -this->updateValue); 
	if( FAILED( rv ) )
		return rv;

	rv = this->land->lkrn->lpDevice->SetIndices( this->indexBuffer );
	if( FAILED( rv ) )
		return rv;
	
	rv = this->land->lkrn->lpDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, this->vertexIndex, 0, this->indexIndex - 2 );
	if( FAILED( rv ) )
		return rv;

	return rv;
}

void a3dLandscape::clear()
{
	
	int	index;
	
	for( int i = 0; i < this->passCount; i++ )
		delete this->passInfo[i];
		
	if( this->heightMap != NULL )
		delete[] this->heightMap;

	this->heightMap = NULL;
	
	if( this->heightLine != NULL )
		delete[] this->heightLine;

	this->heightLine = NULL;
	
	if( this->patches != NULL )
	{
		for( index = 0; index < this->patchCount; index ++ )
		{
			if( this->patches[ index ] != NULL )
				delete[] this->patches[ index ];
		}
	
		delete[] this->patches;
		
		this->patches = NULL;
	}
		
	this->mapSize				= 0;
	this->landSize				= 0.0f;	
	this->patchSize				= 0.0f;
	this->patchCount			= 0;
	this->quadCountPerPatchSide	= 0;
	this->passCount				= 0;
	
	return ;
}

a3dLandscape::a3dLandscape()
{
	this->scene				= null;
	
	this->passCount			= 0;
	this->heightLine		= NULL;
	this->heightMap			= NULL;
	this->patches			= NULL;
	
	this->lkrn				= NULL;

	this->mapSize			= 0;
	
	this->landSize			= 0.0f;	
	this->patchSize			= 0.0f;
	
	this->lpEffect			= NULL;
	this->enableTesselation = true;
	this->forceMaxDetailCurrentPatch = false;

	this->lightMapTextureDesc = null;

	originMatrix4( this->basis );
}

a3dLandscape::~a3dLandscape()
{
	this->clear();
}

void a3dLandscape::tesselate()
{
	
	int i, j;
	
	// считаем видимость и уровень детализации
	for( i = 0; i < this->patchCount; i++ )
	{
		for( j = 0; j < this->patchCount; j ++ )
		{
			// а надо ли рендерить
			this->patches[ i ][ j ].checkVisibility();
			// считаем уровень детализации
			this->patches[ i ][ j ].calcDetalizationLevel();
		}
	}

	// устанавливаем флажок обновления
	for( i = 0; i < this->patchCount; i++ )
	{
		for( j = 0; j < this->patchCount; j ++ )
		{
			this->patches[ i ][ j ].calcUpdateFlag();
		}
	}


	// подсчитываем соседей
	for( i = 0; i < this->patchCount; i++ )
	{
		for( j = 0; j < this->patchCount; j ++ )
		{
			this->patches[ i ][ j ].calcNeighboorDelta();
		}
	}
	
	// а теперь тесселируем
	for( i = 0; i < this->patchCount; i++ )
	{
		for( j = 0; j < this->patchCount; j ++ )
		{
			if( this->patches[ i ][ j ].visible ) { 
				this->patches[ i ][ j ].calcBlendParams();
				this->patches[ i ][ j ].tesselate();
				this->patches[ i ][ j ].updateBlendVertByNeighboors();
			}
		}
	}
		
	/*
	for( i = 0; i < this->patchCount; i++ )
	{
		for( j = 0; j < this->patchCount; j ++ )
		{
			if( (DWORD)this->patches[ i ][ j ].vertexBuffer != (DWORD)this->patches[ i ][ j ].checkBuffer ) { 
				_stprintf_s( this->lkrn->tempBuffer, TEXT( "%d %d" ), i, j );
				this->lkrn->ErrorBox( this->lkrn->tempBuffer );
			}
		}
	}*/

	return ;
}

int landMapPassInfo::getPassIndex() { 
	// if should not render detail return 0;
	// otherwice return 1

	return this->shouldRenderDetail()? 1: 0;
}

HRESULT landMapPassInfo::beginPass() { 
	
	HRESULT rv;
	
	if(this->land == NULL || this->land->lpEffect == NULL) return D3DERR_INVALIDCALL;		

	if( this->mapTextureDesc != null ) {
		rv = this->land->lpEffect->SetTexture( (D3DXHANDLE)"MapTexture", this->mapTextureDesc->texture->lpTexture );
		if( FAILED( rv ) )
			return rv;
	}
		
	if( this->maskTextureDesc != null ) {
		rv = this->land->lpEffect->SetTexture( (D3DXHANDLE)"MaskTexture", this->maskTextureDesc->texture->lpTexture );
		if( FAILED(rv) ) 
			return rv; 
	}

	rv = this->land->lpEffect->SetFloat( (D3DXHANDLE)"mapScaleU", this->mapUSize );
	if(FAILED(rv)) 
		return rv; 
	
	rv = this->land->lpEffect->SetFloat( (D3DXHANDLE)"mapScaleV", this->mapVSize );
	if(FAILED(rv)) 
		return rv; 
	
	if( this->shouldRenderDetail() && this->detailTextureDesc != null ) {
		rv = this->land->lpEffect->SetTexture( (D3DXHANDLE)"DetailTexture", this->detailTextureDesc->texture->lpTexture );
		if( FAILED(rv) )
			return rv;

		rv = this->land->lpEffect->SetFloat( (D3DXHANDLE)"detScaleU", this->detUSize );
		if(FAILED(rv)) 
			return rv; 
	
		rv = this->land->lpEffect->SetFloat( (D3DXHANDLE)"detScaleV", this->detVSize );
		if(FAILED(rv)) 
			return rv; 
	
		rv = this->land->lpEffect->SetFloat( (D3DXHANDLE)"detailAlphaValue", this->currentDetailAlphaValue );
		if(FAILED(rv)) 
			return rv; 
	}
	
#if D3D_SDK_VERSION < 32
	return this->land->lpEffect->Pass(this->getPassIndex());
#else
	return this->land->lpEffect->BeginPass(this->getPassIndex());
#endif

}

HRESULT landMapPassInfo::endPass() { 

#if D3D_SDK_VERSION >= 32
	return this->land->lpEffect->EndPass();
#else
	return D3D_OK; 
#endif

}

HRESULT a3dLandscape::render()
{
	
	UINT		uPasses;
	int			i, j, iPass;
	HRESULT		rv;

	Matrix4		mat;

	if(this->lpEffect != NULL) {
		this->currCamera->preapreViewProj();
		mulMatrix4(this->basis, this->currCamera->viewProj, mat);
	
		rv = lpEffect->SetMatrix( (D3DXHANDLE)"WvpXf", (D3DXMATRIX*)mat);
		if(FAILED(rv))
			return rv;
	
		if( this->lightMapTextureDesc != null ) {
			rv = lpEffect->SetTexture( (D3DXHANDLE)"LightMapTexture", this->lightMapTextureDesc->texture->lpTexture);
			if(FAILED(rv))
				return rv;
		}

		rv = lpEffect->Begin(&uPasses, 0);
		if(FAILED(rv))
			return rv;

		for( iPass = 0; iPass < this->passCount; iPass ++ ) {
			if( !this->passInfo[iPass]->enabled ) continue;
			
			rv = this->passInfo[iPass]->beginPass();
			if( FAILED(rv) )
				return rv;
			
			for( i = 0; i < this->patchCount; i ++ )
			{
				for( j = 0; j < this->patchCount; j ++ )
				{
					if( this->patches[i][j].visible && this->patches[i][j].passInfo.isEnabled(iPass) )
					{
						rv = this->patches[i][j].render(iPass);
						if( FAILED( rv ) )
							return rv;
					}
				}
			}

			rv = this->passInfo[iPass]->endPass();
			if( FAILED(rv) )
				return rv;
		}

		rv = lpEffect->End();
		if( FAILED(rv) )
			return rv;
	}
	
	return D3D_OK;
}

void a3dLandscape::prepareConvValues()
{
	this->distFromCamera[0] = basis[3][0] - this->currCamera->pos[0];
	this->distFromCamera[1] = basis[3][1] - this->currCamera->pos[1];
	this->distFromCamera[2] = basis[3][2] - this->currCamera->pos[2];

	this->cameraH			= this->currCamera->proj[1][1] * this->currCamera->screenHeight;
	
	// узнали позицию камеры в ландшафте
	this->cameraPos[0]		= this->currCamera->pos[0] - basis[3][0];
	this->cameraPos[1]		= this->currCamera->pos[1] - basis[3][1];
	this->cameraPos[2]		= this->currCamera->pos[2] - basis[3][2];

	// узнали в каком регионе находится камера
	this->cameraPatchCol	= (int)(this->cameraPos[0] / this->patchSize); 	
	// потому что ландшафт у нас переверуть по з как координаты экрана
	this->cameraPatchRow	= - (int)(this->cameraPos[2] / this->patchSize);

	// потому что ландшафт у нас переверуть по з как координаты экрана
	this->cameraRow			= - (int)(this->cameraPos[2] / this->landSize * ( this->mapSize - 1 ));
	this->cameraCol			= (int)(this->cameraPos[0] / this->landSize * ( this->mapSize - 1 ));
	
	if( this->cameraPos[0] < 0.0f ) { 
		this->cameraPatchCol --;
		this->cameraCol --;
	}
	
	if( this->cameraPos[2] > 0.0f ) { 
		this->cameraRow --;
		this->cameraPatchRow --;
	}

	// -1 0 1 2 3 ...
	
	return ;
}

HRESULT a3dLandscape::loadHeightMap( TCHAR *heightFile, float sideScale, float heightScale, int patchPerSize )
{
	shString s1;

	s1.initialize( heightFile );
	
	return this->loadHeightMap( s1, sideScale, heightScale, patchPerSize );

}

HRESULT a3dLandscape::loadHeightMap( shString &heightFile, float sScale, float hScale, int patchPerSize )
{
	
	float	**map;
	int		size;

	// загружаем карту
	if( loadFloatMap( heightFile.getCString(), &map, &size ) == false )
		return D3DERR_INVALIDCALL;

	this->heightMapFileName = heightFile;

	// инициализация
	int			index, index2, index3;
	float		du, dv;	

	this->clear();

	// длина ширина
	this->mapSize		= size;
	this->sideScale		= sScale;
	this->heightScale	= hScale;

	// создание массива высот
	this->heightLine = new landVertex[ this->mapSize * this->mapSize ];
	// создание карты высот
	this->heightMap = new landVertex*[ this->mapSize ];
	for( index = 0, index2 = 0; index < this->mapSize; index ++, index2 += this->mapSize )
		this->heightMap[ index ] = &this->heightLine[ index2 ];
	
	du = 1.0f / this->mapSize;
	dv = 1.0f / this->mapSize;
	
	float ds = this->sideScale / (this->mapSize - 1);
	
	// заполнение карты высот
	for( index = 0, index3 = 0; index < this->mapSize; index ++ )
	{
		for( index2 = 0; index2 < this->mapSize; index2 ++, index3 ++ )
		{
			// x
			this->heightMap[ index ][ index2 ].position[0]	= index2 * ds;
			// z
			this->heightMap[ index ][ index2 ].position[2]	= - index * ds;
			// y - высота			
			this->heightMap[ index ][ index2 ].position[1]	= map[ index ][ index2 ] * this->heightScale;
			
			// координаты текстуры
			// u
			this->heightMap[ index ][ index2 ].tex0[0]		= index2 * du;
			// v
			this->heightMap[ index ][ index2 ].tex0[1]		= index * dv;
			
			// дельта
			this->heightMap[ index ][ index2 ].deltaY		= 0.0f;

		}
	}
		
	this->patchCount			= patchPerSize;
	this->quadCountPerPatchSide	= ( this->mapSize - 1 ) / this->patchCount;
	this->landSize				= this->sideScale;
	this->patchSize				= this->landSize / this->patchCount;
	this->varianceCount			= this->calcDetalizationLevelCount();

	// теперь инициализируем куски ландшафта
	this->patches		= new landscapePatch* [ this->patchCount ];
	for( index = 0; index < this->patchCount; index ++ )
		this->patches[ index ] = new landscapePatch[ this->patchCount ];
	
	HRESULT rv;	
	for( index = 0; index < this->patchCount; index ++ )
	{
		for( index2 = 0; index2 < this->patchCount; index2 ++ )
		{
			this->patches[ index ][ index2 ].initialize( this, index * ( this->quadCountPerPatchSide ), index2 * ( this->quadCountPerPatchSide ), index, index2 );
			rv = this->patches[ index ][ index2 ].initializeRenderBuffers();
			if(FAILED(rv))
				return rv;
			this->patches[ index ][ index2 ].calcVariance();
			this->patches[ index ][ index2 ].calcDelta(); 
			this->patches[ index ][ index2 ].calcBoundVolume();
		}
	}

	// связать между собой куски ландшафта
	this->linkPatches();
	
	// инициализировать эффект
	//createEffect();

	// базис по умолчанию
	originMatrix4( this->basis );
	
	return D3D_OK;
}

int a3dLandscape::calcDetalizationLevelCount()
{
	int		detalizationLevelCount	= 1;
	int		iterationLevel			= this->quadCountPerPatchSide;

	while( iterationLevel >>= 1 )
		detalizationLevelCount ++;

	return detalizationLevelCount;
}

void a3dLandscape::linkPatches()
{
	landscapePatch	*top, *left, *bottom, *right;
	int				i, j;
	
	for( i = 0; i < this->patchCount; i ++ )
	{
		for( j = 0; j < this->patchCount; j ++ )
		{
			top = left = bottom = right = NULL;
			
			if( i > 0 )
				top = &this->patches[i - 1][j];
			if( j > 0 )
				left = &this->patches[i][j - 1];
			
			if( i < this->patchCount - 1 )
				bottom = &this->patches[i + 1][j];
			if( j < this->patchCount - 1 )
				right = &this->patches[i][j + 1];
		
			this->patches[i][j].setNeighbors( top, left, bottom, right );
		}
	}

	return ;
}

HRESULT	a3dLandscape::drawBoundBox( a3dModel &cube )
{
	
	HRESULT rv;
	DWORD	fillMode;
	int		i, j;
	
	this->lkrn->lpDevice->GetRenderState( D3DRS_FILLMODE, &fillMode );
	this->lkrn->lpDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	
	this->renderPatchCount	= 0;

	for( i = 0; i < this->patchCount; i ++ )
	{
		for( j = 0; j < this->patchCount; j ++ )
		{
			if( this->patches[i][j].visible )
			{
				this->renderPatchCount ++;
				rv = this->patches[i][j].drawBoundBox( cube );
				if( FAILED( rv ) )
					return rv;
			}
		}
	}
	this->lkrn->lpDevice->SetRenderState( D3DRS_FILLMODE, fillMode );
	
	return D3D_OK;
}

HRESULT	a3dLandscape::drawBoundSphere( a3dModel &sphere )
{

	HRESULT rv;
	DWORD	fillMode;
	int		i, j;
	
	this->lkrn->lpDevice->GetRenderState( D3DRS_FILLMODE, &fillMode );
	this->lkrn->lpDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	
	this->renderPatchCount	= 0;
	
	for( i = 0; i < this->patchCount; i ++ )
	{
		for( j = 0; j < this->patchCount; j ++ )
		{
			if( this->patches[i][j].visible )
			{
				this->renderPatchCount ++;
				rv = this->patches[i][j].drawBoundSphere( sphere );
				if( FAILED( rv ) )
					return rv;
			}
		}
	}
	this->lkrn->lpDevice->SetRenderState( D3DRS_FILLMODE, fillMode );

	return D3D_OK;
}

HRESULT a3dLandscape::createEffect()
{
	HRESULT rv;
	HLSLManager	hlslman;

	if( this->scene != null ) this->effectDesc = this->scene->getEffect( effectFileName );
	if( this->effectDesc != null ) { 
		this->lpEffect = this->effectDesc->lpEffect;
	}
	else {		
	
		rv = hlslman.createEffectFromFile(this->lkrn->lpDevice, this->effectFileName);
		lpEffect = hlslman.lpEffect;
		hlslman.result.saveToFile(shString(TEXT("landscape.log")));
		if(FAILED(rv))
			return rv;
	}

	rv = this->lpEffect->OnResetDevice();
	if(FAILED(rv))
		return rv;

	return D3D_OK;
}

void a3dLandscape::clearEffect() { 
	
	lpEffect->Release();
	lpEffect = NULL;
	 
	return;
}

void a3dLandscape::setTextureCoors() { 
	
	float	du = ( 1.0f - 0.0f ) / this->mapSize,
			dv = ( 1.0f - 0.0f ) / this->mapSize, 
			u = 0.0f, v = 0.0f;
	
	int r, c;
	
	for( r = 0; r < this->mapSize; r ++, v += dv ) { 
		u = 0.0f;
		for( c = 0; c < this->mapSize; c ++, u += du ) {
			this->heightMap[r][c].tex0[0] = u;
			this->heightMap[r][c].tex0[1] = v;
		}
	}
	
	return ;
}

int a3dLandscape::getPassIndex(landMapPassInfo * pass) { 

	for( int i = 0; i < 16; i++ ) {
		if( this->passInfo[i] == pass ) return i;
	}

	return -1;
}

void a3dLandscape::calcPassEnabled(int passIndex) { 
	
	int row, col;
	//this->passInfo[passIndex]->enabled = false;
	for( row = 0; row < this->patchCount; row ++ ) { 
		for( col = 0; col < this->patchCount; col ++ ) {
			this->patches[ row ][ col ].passInfo.calcPassEnabled( passCount, this->passInfo[passIndex] );
		}
	}
}

void a3dLandscape::calcPassEnabled(landMapPassInfo * pass) { 
	
	int passIndex = this->getPassIndex(pass);
	if( passIndex == -1 ) return ;
	
	this->calcPassEnabled(passIndex);

	return ;
}

void a3dLandscape::calcMaskEnabled(landMapPassInfo * pass) { 
	
	int passIndex = this->getPassIndex(pass);
	if( passIndex == -1 ) return ;

	this->calcMaskEnabled(passIndex);

	return ;
}

void a3dLandscape::addPass( landMapPassInfo *pass ) {

	if( this->passCount == 16 ) return;
	
	this->passInfo[passCount] = pass;
	
	this->calcPassEnabled(pass);
	pass->land = this;

	this->passCount ++;
}

void a3dLandscape::calcMaskEnabled(int index) { 

	int row, col;
	for( row = 0; row < this->patchCount; row++ ) {
		for( col = 0; col < this->patchCount; col++ ) {
			this->patches[row][col].passInfo.calcMaskEnabled(index, this->passInfo[index]);
			this->patches[row][col].passInfo.calcPassEnabled(index, this->passInfo[index]);
		}
	}
	
	return;
}

void a3dLandscape::insertPass( int index, landMapPassInfo *pass ) {

	if( this->passCount >= 16 )
		return;

	pass->land = this;
	if( index >= passCount - 1 ) {
		this->addPass( pass );
		return ;
	}
	
	int i;
	for( i = passCount - 1; i >= index; i-- )
		this->passInfo[i + 1] = this->passInfo[i]; 
	
	this->passInfo[index] = pass;
	this->calcPassEnabled(pass);
	pass->land = this;

	return;
}

void a3dLandscape::removePass( int index ) {
	
	if( index >= this->passCount ) return;

	delete this->passInfo[index];
	int i;
	for( i = index; i < this->passCount - 1; i++ ) { 
		this->passInfo[i] = this->passInfo[i+1];
	}
	
	return;
}

HRESULT landMapPassFlags::calcMaskEnabled(int passIndex, landMapPassInfo *info) { 

	if( info->getMaskTexture()->texture == null ) 
		return D3D_OK;

	D3DLOCKED_RECT lockRect;
	D3DSURFACE_DESC	desc;
	HRESULT rv;
	RECT rect;

	rv = info->getMaskTexture()->texture->lpTexture->GetLevelDesc(0, &desc);
	if(FAILED( rv ))
		return rv;

	// подготавливаем rect
	rect.top = this->patch->rowIndex * desc.Height / this->patch->land->patchCount;
	rect.left = this->patch->colIndex * desc.Width / this->patch->land->patchCount;
	rect.bottom = (this->patch->rowIndex + 1) * desc.Height / this->patch->land->patchCount;
	rect.right = (this->patch->colIndex + 1) * desc.Width / this->patch->land->patchCount;

	// открываем текстуру и смотрим
	rv = info->getMaskTexture()->texture->lpTexture->LockRect( 0, &lockRect, NULL, 0 );
	if( FAILED( rv ) )
		return rv;

	Color8888	*rgba;
	BYTE		*a;
	int			row, col, index;

	bool		found = false;
	// таак теперь
	if( desc.Format == D3DFMT_A8R8G8B8 || desc.Format == D3DFMT_A8B8G8R8 ) { 
		rgba = (Color8888*)lockRect.pBits;
		index = rect.top * desc.Width;
		for( row = rect.top; row < rect.bottom; row++ ) {
			for( col = rect.left; col < rect.right; col ++ ) {
				if( rgba[index + col].a > 0 ) { 
					found = true;
					break;
				}
			}
			if(found) break;
			index += desc.Width;
		}
	}
	else if( desc.Format == D3DFMT_A8 || desc.Format == D3DFMT_L8 ) { 
		a = (BYTE*)lockRect.pBits;
		index = rect.top * desc.Width;
		for( row = rect.top; row < rect.bottom; row++ ) {
			for( col = rect.left; col < rect.right; col ++ ) {
				if( a[index + col] > 0 ) { 
					found = true;
					break;
				}
			}
			if(found) break;
			index += desc.Width;
		}
	}

	this->maskCalculated[passIndex] = true;
	this->maskEnabled[passIndex] = found;

	// освобождаем
	info->getMaskTexture()->texture->lpTexture->UnlockRect(0);
	
	return D3D_OK;
}

HRESULT landMapPassFlags::calcPassEnabled(int passIndex, landMapPassInfo *info) { 
	
	this->enabled[passIndex] = info->enabled;
	if( this->enabled[passIndex] == false ) 
		return D3D_OK;

	// по умолчанию false
	this->enabled[passIndex] = false;
	
	// если анимируемый то всегда виден
	if(info->animated) { 
		this->enabled[passIndex] = true;
		return D3D_OK;
	}
	
	HRESULT rv;
	if( !this->maskCalculated[passIndex] ) {
		rv = this->calcMaskEnabled(passIndex, info);
		if( FAILED(rv) )
			return rv;
	}

	this->enabled[passIndex] = this->maskEnabled[passIndex];
		
	return D3D_OK;
}

HRESULT landMapPassFlags::calcPassEnabled() {
	
	if( this->patch == NULL ) return D3DERR_INVALIDCALL;
	HRESULT rv;

	for( int i = 0; i < this->patch->land->passCount; i ++ ) { 
		rv = this->calcPassEnabled( i, this->patch->land->passInfo[i] );
		if( FAILED( rv ) )
			return rv;
	}

	return D3D_OK;
}

HRESULT landMapPassFlags::initialize(landscapePatch *patch) { 
	
	this->patch = patch;
	return this->calcPassEnabled();

}

void a3dLandscape::offsetCameraByHeight( float deltaHeight ) { 
	
	// если камера не попала в квадрат
	if( this->cameraRow < 0 || this->cameraRow >= this->mapSize - 1 ) return;
	if( this->cameraCol < 0 || this->cameraCol >= this->mapSize - 1 ) return;
	
	float dz = this->cameraPos[2] - this->heightMap[ this->cameraRow ][ this->cameraCol ].position[2];
	float dx = this->cameraPos[0] - this->heightMap[ this->cameraRow ][ this->cameraCol ].position[0];
	float height;

	// мы находимся во втором треугольнике
	if( dx > - dz )
		height = camera::getIntersectYFromPlane( this->cameraPos, this->heightMap[ this->cameraRow ][ this->cameraCol ].position, this->heightMap[ this->cameraRow ][ this->cameraCol + 1].position, this->heightMap[ this->cameraRow + 1 ][ this->cameraCol + 1].position );
	else
		height = camera::getIntersectYFromPlane( this->cameraPos, this->heightMap[ this->cameraRow ][ this->cameraCol ].position, this->heightMap[ this->cameraRow + 1] [ this->cameraCol ].position, this->heightMap[ this->cameraRow + 1 ][ this->cameraCol + 1].position );
	
	// двигаем камеру
	this->currCamera->position( this->currCamera->pos[0], height + deltaHeight, this->currCamera->pos[2] );
	// заново переподсчитаем
	this->prepareConvValues();
}

void a3dLandscape::getIntersectYFromPlane( Vector3 v, Vector3 p0, Vector3 p1, Vector3 p2 ) { 
	
	Vector3 p, p0p1, p0p2;
	
	subVector3( v, p0, p);
	subVector3( p1, p0, p0p1 );
	subVector3( p2, p0, p0p2 );

	// p[0]			  y - p0[1]     p[2]
	// p0p1[0]         p0p1[1]      p0p1[2]
	// p0p2[0]         p0p2[1]      p0p2[2]

	v[1] = (p[0] * ( p0p1[1] * p0p2[2] - p0p2[1] * p0p1[2] ) + p[2] * ( p0p1[0] * p0p2[1] - p0p2[0] * p0p1[1] )) / ( p0p1[0] * p0p2[2] - p0p2[0] * p0p1[2] ) + p0[1];
	
	return ;
}

landscapePatch* a3dLandscape::getPatch( float x, float z ) { 

	Vector3 cv;
	
	cv[0] = x - basis[3][0];
	cv[1] = - basis[3][1];
	cv[2] = z - basis[3][2];

	int patchRow = - (int) (cv[2] / this->patchSize);
	int patchCol = (int)(cv[0] / this->patchSize);

	if( cv[0] < 0.0f ) patchCol --;
	if( cv[2] > 0.0f ) patchRow --;
	
	if( patchCol < 0 || patchCol >= this->patchCount || patchRow < 0 || patchRow >= this->patchCount ) return null;
	return &(this->patches[ patchRow ][ patchCol ]);
}

void a3dLandscape::updateObjectYByLand( Vector3 v ) { 

	Vector3 cv;
	
	cv[0] = v[0] - basis[3][0];
	cv[1] = v[1] - basis[3][1];
	cv[2] = v[2] - basis[3][2];

	// потому что ландшафт у нас переверуть по з как координаты экрана
	int row = - (int)(cv[2] / this->landSize * ( this->mapSize - 1 ));
	int col	= (int)(cv[0] / this->landSize * ( this->mapSize - 1 ));
	
	if( cv[0] < 0.0f ) col --;
	if( cv[2] > 0.0f ) row --;

	if( row < 0 || col < 0 || row > this->mapSize - 1 || col > this->mapSize - 1 ) return;
	
	float dz = cv[2] - this->heightMap[ row ][ col ].position[2];
	float dx = cv[0] - this->heightMap[ row ][ col ].position[0];

	// мы находимся во втором треугольнике
	if( dx > - dz )
		this->getIntersectYFromPlane( cv, this->heightMap[ row ][ col ].position, this->heightMap[row ][ col + 1].position, this->heightMap[ row + 1 ][ col + 1].position );
	else
		this->getIntersectYFromPlane( cv, this->heightMap[ row ][ col ].position, this->heightMap[row + 1][ col ].position, this->heightMap[ row + 1 ][ col + 1].position );
	
	v[1] = cv[1] + basis[3][1];

	return ;
}

void landMapPassInfo::setLandMapPassInfo( a3dLandscape *land, a3dTextureDesc *mapTexDesc, a3dTextureDesc *maskTexDesc, a3dTextureDesc *detTexDesc, float mapUSize, float mapVSize, float detUSize, float detVSize, float minDetDist, float maxDetDist, float maxDetAlpha ) { 
	
	this->mapTextureDesc = mapTexDesc;
	this->maskTextureDesc = maskTexDesc;
	this->detailTextureDesc = detTexDesc;
	this->mapUSize = mapUSize;
	this->mapVSize = mapVSize;
	this->detUSize = detUSize;
	this->detVSize = detVSize;
	this->enabled = false;
	this->animated = false;
	this->minDetailDistance = minDetDist;
	this->maxDetailDistance = maxDetDist;
	this->maxDetailAlphaValue = maxDetAlpha;
	this->currentDetailAlphaValue = 0.0f;
}

void landMapPassInfo::setMaskTexture( a3dTextureDesc *desc ) { 
	
	this->maskTextureDesc = desc;
	this->land->calcMaskEnabled( this );
	
	return ;
} 

// one lightmap for whole landscape
int a3dLandscape::getLightMapCount() { return 1; }

a3dLightMap** a3dLandscape::getLightMaps() {  

	
	a3dLightMap **lmList = new a3dLightMap*[ this->getLightMapCount() ];
	
	lmList[0] = new a3dLightMap( this->lightMapWidth, this->lightMapHeight, this->getLightMapPlaneColl(), this->lightMapTextureDesc->fullPathName );
	
	return lmList;
}

int a3dLandscape::getPlaneCount() { return ( this->mapSize - 1 ) * ( this->mapSize - 1 ) * 2; }

void a3dLandscape::fillLightMapPlaneColl( a3dLightMapPlaneCollection *coll ) { 
	
	int row, col, pi = 0;
	for( row = 0; row < this->mapSize - 1; row++ ) { 
		for( col = 0; col < this->mapSize - 1; col++ ) {
			copyVector3( this->heightMap[row][col].position, coll->plane[pi].a );
			copyVector3( this->heightMap[row][col+1].position, coll->plane[pi].b );
			copyVector3( this->heightMap[row+1][col].position, coll->plane[pi].c );
			
			this->calcNormal( row, col, coll->plane[pi].na );
			this->calcNormal( row, col+1, coll->plane[pi].nb );
			this->calcNormal( row+1, col, coll->plane[pi].nc );
			coll->plane[pi].translate( this->basis, false );
			pi++;

			copyVector3( this->heightMap[row][col+1].position, coll->plane[pi].a );
			copyVector3( this->heightMap[row+1][col+1].position, coll->plane[pi].b );
			copyVector3( this->heightMap[row+1][col].position, coll->plane[pi].c );
			
			this->calcNormal( row, col+1, coll->plane[pi].na );
			this->calcNormal( row+1, col+1, coll->plane[pi].nb );
			this->calcNormal( row+1, col, coll->plane[pi].nc );
			coll->plane[pi].translate( this->basis, false );
			pi++;
		}
	}

	return ;
}

void a3dLandscape::calcNormal(int row, int col, Vector3 v) { 

	Vector3 v1, v2, v3;

	clearVector3( v );
	
	if( row > 0 ) { 
		if( col > 0 ) { 
			subVector3( this->heightMap[row][col].position, this->heightMap[row-1][col].position, v1 );
			subVector3( this->heightMap[row][col-1].position, this->heightMap[row-1][col].position, v2 );
			vectorMulOfVector( v1, v2, v3 );
			normalizeVector( v3 );
			addVector3( v, v3, v );
		}
		if( col < this->mapSize - 1 ) {
			subVector3( this->heightMap[row-1][col+1].position, this->heightMap[row-1][col].position, v1 );
			subVector3( this->heightMap[row][col].position, this->heightMap[row-1][col].position, v2 );
			vectorMulOfVector( v1, v2, v3 );
			normalizeVector( v3 );
			addVector3( v, v3, v );

			subVector3( this->heightMap[row][col+1].position, this->heightMap[row-1][col+1].position, v1 );
			subVector3( this->heightMap[row][col].position, this->heightMap[row-1][col+1].position, v2 );
			vectorMulOfVector( v1, v2, v3 );
			normalizeVector( v3 );
			addVector3( v, v3, v );
		}
	}
	
	if( row < this->mapSize - 1 ) { 
		if( col > 0 ) { 
			subVector3( this->heightMap[row][col].position, this->heightMap[row][col-1].position, v1 );
			subVector3( this->heightMap[row+1][col-1].position, this->heightMap[row][col-1].position, v2 );
			vectorMulOfVector( v1, v2, v3 );
			normalizeVector( v3 );
			addVector3( v, v3, v );
		
			subVector3( this->heightMap[row+1][col].position, this->heightMap[row][col].position, v1 );
			subVector3( this->heightMap[row+1][col-1].position, this->heightMap[row][col].position, v2 );
			vectorMulOfVector( v1, v2, v3 );
			normalizeVector( v3 );
			addVector3( v, v3, v );
		}
		if( col < this->mapSize - 1 ) { 
			subVector3( this->heightMap[row][col+1].position, this->heightMap[row][col].position, v1 );
			subVector3( this->heightMap[row+1][col].position, this->heightMap[row][col].position, v2 );
			vectorMulOfVector( v1, v2, v3 );
			normalizeVector( v3 );
			addVector3( v, v3, v );
		} 
	}

	normalizeVector( v );

	return ;
}

a3dLightMapPlaneCollection* a3dLandscape::getLightMapPlaneColl() { 

	a3dLightMapPlaneCollection *coll = new a3dLightMapPlaneCollection( this->getPlaneCount() );
	
	this->fillLightMapPlaneColl( coll );	
	
	return coll;
}