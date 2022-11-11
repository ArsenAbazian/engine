#include "train.h"
#include "n3dsfile.h"

int  AnimatedModel::getAnimatedIndex(a3dModel *model, TCHAR *name) { 
	shString str(name);
	int i;
	
	for( i = 0; i < model->objCount; i++) {
		if(model->object[i].name == str) return i;
	}
	return -1;
}

Train3D::Train3D() {
	this->model = NULL;
	
	this->bigReel1 = -1;
	this->bigReel2 = -1;
	this->smallReel1 = -1;
	this->smallReel2 = -1;
	this->tube1 = -1;
	this->tube2 = -1;
	this->tube3 = -1;
}

bool Train3D::initializeAnimatedIndices() {
	this->bigReel1 = this->getAnimatedIndex(this->model, TEXT("bReel1"));
	this->bigReel2 = this->getAnimatedIndex(this->model, TEXT("bReel2"));
	this->smallReel1 = this->getAnimatedIndex(this->model, TEXT("sReel1"));
	this->smallReel2 = this->getAnimatedIndex(this->model, TEXT("sReel2"));
	this->tube1 = this->getAnimatedIndex(this->model, TEXT("Tube1"));
	this->tube2 = this->getAnimatedIndex(this->model, TEXT("Tube2"));
	this->tube3 = this->getAnimatedIndex(this->model, TEXT("Tube3"));
	this->smokeTube = this->getAnimatedIndex(this->model, TEXT("tube01"));

	return 
		this->bigReel1 != -1 && 
		this->bigReel2 != -1 && 
		this->smallReel1 != -1 &&
		this->smallReel2 != -1 &&
		this->tube1 != -1 && 
		this->tube2 != -1 && 
		this->tube3 != -1;
}

void Train3D::getBasisNormalizedYVector(Matrix4 mat, Vector3 v) {
	
	setVector3(v, this->bigReel1Basis[0][1], this->bigReel1Basis[1][1], this->bigReel1Basis[2][1]);
	normalizeVector(v);

}

bool Train3D::initializeData() { 
	
	copyMatrix4(this->model->object[this->bigReel1].basis, this->bigReel1Basis);
	copyMatrix4(this->model->object[this->bigReel2].basis, this->bigReel2Basis);
	copyMatrix4(this->model->object[this->smallReel1].basis, this->smallReel1Basis);
	copyMatrix4(this->model->object[this->smallReel2].basis, this->smallReel2Basis);
	copyMatrix4(this->model->object[this->tube1].basis, this->tube1Basis);
	copyMatrix4(this->model->object[this->tube2].basis, this->tube2Basis);
	copyMatrix4(this->model->object[this->tube3].basis, this->tube3Basis);
	copyMatrix4(this->getObj(this->smokeTube)->basis, this->smokeTubeBasis);

	Vector3 y = { 0.0f, 1.0f, 0.0f };
	Vector3 v;
	
	// инициализируем углы
	this->getBasisNormalizedYVector(this->bigReel1Basis, v);
	this->bigReelAngle = acos(scalarMulOfVector(v, y));
	
	this->getBasisNormalizedYVector(this->smallReel1Basis, v);
	this->smallReel1Angle = acos(scalarMulOfVector(v,y));

	this->getBasisNormalizedYVector(this->smallReel2Basis, v);
	this->smallReel2Angle = acos(scalarMulOfVector(v,y));

	this->getBasisNormalizedYVector(this->smallReel1Basis, v);
	this->smallReel1Angle = acos(scalarMulOfVector(v,y));

	this->reelsKoeff = (this->model->object[this->bigReel1].bVolume.getWidth()) / (this->model->object[this->smallReel1].bVolume.getWidth());
	
	setVector3(this->distanceFromBigReel1ToTube1, this->tube1Basis[3][0] - this->bigReel1Basis[3][0], this->tube1Basis[3][1] - this->bigReel1Basis[3][1], this->tube1Basis[3][2] - this->bigReel1Basis[3][2]);
	setVector3(this->distanceFromBigReel2ToTube2, this->tube2Basis[3][0] - this->bigReel2Basis[3][0], this->tube2Basis[3][1] - this->bigReel2Basis[3][1], this->tube2Basis[3][2] - this->bigReel2Basis[3][2]);

	// относительно самое сложное 
	// определяем длину вектора до следующего передаточного устройства
	setVector3(this->distanceFromTube2ToTube3, this->tube3Basis[3][0] - this->tube2Basis[3][0], this->tube3Basis[3][1] - this->tube2Basis[3][1], this->tube3Basis[3][2] - this->tube2Basis[3][2]);
	this->lengthFromTube2ToTube3 = normalizeVector(this->distanceFromTube2ToTube3);
	
	// расстояние между колесами
	this->distanceFromBigToSmall = fabs( this->bigReel1Basis[3][2] - this->smallReel2Basis[3][2] );

	return true;
}

void Train3D::move(float deltaBigReelAngle) { 

	this->model->object[this->bigReel1].rotateAroundY(deltaBigReelAngle);
	this->model->object[this->bigReel2].rotateAroundY(deltaBigReelAngle);
	
	// маленькие колеса крутятся гораздо быстрее потому что должны пройти путь за то же время что прошли большие колеса	
	this->model->object[this->smallReel1].rotateAroundY(-deltaBigReelAngle * this->reelsKoeff);
	this->model->object[this->smallReel2].rotateAroundY(-deltaBigReelAngle * this->reelsKoeff );

	Vector3 v = { this->bigReel1Basis[0][1],  this->bigReel1Basis[1][1], this->bigReel1Basis[2][1] };
	
	// поворот вектора дистанции
	rotateVectorAroundVector(this->distanceFromBigReel1ToTube1, v, this->distanceFromBigReel1ToTube1, -deltaBigReelAngle);
	rotateVectorAroundVector(this->distanceFromBigReel2ToTube2, v, this->distanceFromBigReel2ToTube2, -deltaBigReelAngle);
	
	// колеса воздействуют на передаточное устройство 1
	this->model->object[this->tube1].basis[3][0] = this->model->object[this->bigReel1].basis[3][0] + this->distanceFromBigReel1ToTube1[0];
	this->model->object[this->tube1].basis[3][1] = this->model->object[this->bigReel1].basis[3][1] + this->distanceFromBigReel1ToTube1[1];
	this->model->object[this->tube1].basis[3][2] = this->model->object[this->bigReel1].basis[3][2] + this->distanceFromBigReel1ToTube1[2];

	// и передаточное устройство 2
	this->model->object[this->tube2].basis[3][0] = this->model->object[this->bigReel2].basis[3][0] + this->distanceFromBigReel2ToTube2[0];
	this->model->object[this->tube2].basis[3][1] = this->model->object[this->bigReel2].basis[3][1] + this->distanceFromBigReel2ToTube2[1];
	this->model->object[this->tube2].basis[3][2] = this->model->object[this->bigReel2].basis[3][2] + this->distanceFromBigReel2ToTube2[2];

	// теперь определяем угол наклона
	float Yy1	= this->model->object[this->tube3].basis[3][1] - this->model->object[this->tube2].basis[3][1];
	float Z		= sqrt(this->lengthFromTube2ToTube3 * this->lengthFromTube2ToTube3 - Yy1 * Yy1);
	
	this->model->object[this->tube3].basis[3][2] = this->model->object[this->tube2].basis[3][2] + Z;

	Vector3 z = { 0.0f, 0.0f, 1.0f };
	setVector3(v, 0.0f, Yy1, Z);
	normalizeVector(v);
	this->tube2Angle = acos(scalarMulOfVector(v, z));
	if(this->model->object[this->tube2].basis[3][1] > this->model->object[this->tube3].basis[3][1]) 
		this->tube2Angle = -this->tube2Angle;

	this->model->object[this->tube2].basis[0][0] = this->tube2Basis[0][0];
	this->model->object[this->tube2].basis[1][0] = this->tube2Basis[1][0];
	this->model->object[this->tube2].basis[2][0] = this->tube2Basis[2][0];

	this->model->object[this->tube2].basis[0][1] = this->tube2Basis[0][1];
	this->model->object[this->tube2].basis[1][1] = this->tube2Basis[1][1];
	this->model->object[this->tube2].basis[2][1] = this->tube2Basis[2][1];

	this->model->object[this->tube2].basis[0][2] = this->tube2Basis[0][2];
	this->model->object[this->tube2].basis[1][2] = this->tube2Basis[1][2];
	this->model->object[this->tube2].basis[2][2] = this->tube2Basis[2][2];

	this->model->object[this->tube2].rotateAroundY(this->tube2Angle);

	// теперь угол поворота трубы второго передаточного устройства.

	// Базис трубы
	copyMatrix4(this->getObj(this->smokeTube)->basis, this->smokeTubeBasis);
}

HRESULT Train3D::initialize( a3dModel *mdl, Kernel *krn ) { 
	
	this->clear();
	this->model = mdl;
	this->model->setKernel(krn);

	if(!this->initializeAnimatedIndices()) return D3DERR_INVALIDCALL;
	if(!this->initializeData()) return D3DERR_INVALIDCALL;

	return D3D_OK;
}