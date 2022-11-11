#ifndef A3D_RAIL_MANAGER
#define A3D_RAIL_MANAGER

#include "boundBox.h"
#include "a3dModel\\a3dModel.h"
#include "ncamera.h"
#include "lightMapCreator.h"

class a3dRailInfo { 
public:
	a3dBoundBox		bBox;
	float			angle;
	float			length;
	float			width;
	float			radius;
	bool			isRound;
	bool			isLeft;
	bool			isVisible;
	Vector3			centerPoint;		// точка центра окружности

	a3dRailInfo() { this->clear(); }
	~a3dRailInfo() { this->clear(); }

	void			clear();
	bool			readFromFile( FILE *fp );
	void			calcBasis();
	void			calcBVolume(a3dBoundBox &box);
	void			getLeftCenterPoint(Vector3 centerPoint);
	void			getRightCenterPoint(Vector3 centerPoint);
	void			getPoint(float len, Vector3 point);
	void			getLinePoint( float len, Vector3 point );
	void			getLeftPoint(float len, Vector3 leftPoint);
	void			getRightPoint(float len, Vector3 rightPoint);
	void			getLeftRoundLeftPoint(Vector3 point, float len, float wdt);
	void			getLeftRoundRightPoint(Vector3 point, float len, float wdt);
	void			getRightRoundLeftPoint(Vector3 point, float len, float wdt);
	void			getRightRoundRightPoint(Vector3 point, float len, float wdt);
};

class a3dModelDesc;
class a3dEffectDesc;
class a3dRailManager : public a3dLightMapCreator { 
public:
	int				railCount;
	a3dRailInfo		*rails;

	Vector3			origin;				// точка

	float			landSideSize;
	float			landHeight;
	float			railLength;
	float			railWidth;
	int				woodCount;
	float			scaleMeters;
	float			length;				

	a3dModelDesc	*lineRailModel;		// модель рельсов
	a3dModelDesc	*roundRailModel;	// детализированная модель рельсов
	a3dEffectDesc	*lineRailEffect;
	a3dEffectDesc	*roundRailEffect;
	a3dEffectDesc	*roundRailEffect2;

	a3dRailManager() { 
		this->railCount = 0;
		this->rails = null;
		this->lineRailModel = null;
		this->roundRailModel = null;
		setVector3( this->origin, 0.0f, 0.0f, 0.0f );
	}

	~a3dRailManager() { this->clear(); }
	
	void			clear();
	bool			loadFromFile( shString &fileName );
	bool			loadFromFile( TCHAR *fileName ) { return this->loadFromFile( shString( fileName ) ); }
	void			setRailModels( a3dModelDesc *lineModel, a3dModelDesc *roundModel );
	void			setRailEffects( a3dEffectDesc *lineEffect, a3dEffectDesc *roundEffect, a3dEffectDesc *roundEffect2 );
	void			setOrigin( Vector3 origin );
	void			setOrigin( float x, float y, float z );
	void			calcLength();
	HRESULT			prepareLineModel( a3dModel *model, a3dRailInfo *info );
	HRESULT			prepareRoundModel( a3dModel *model, a3dRailInfo *info );
	HRESULT			render( camera *cam );

	int				getPositionPoint( float posT, Vector3 pos );
	void			getLinePositionPoints( float posT, float len, Vector3 pos1, Vector3 pos2 );

	// lightmap generation
	a3dLightMap**	getLightMaps();
	int				getLightMapCount();
};

void getLeftPointCore( float radius, Vector3 v, Vector3 dst );
void updateLeftPoint( a3dRailInfo *ri, Vector3 pos );
void getRightPointCore( float radius, Vector3 pos, Vector3 dst );
void updateRightPoint( a3dRailInfo *ri, Vector3 pos );
void parseNormal( Vector3 axis, Vector3 normal, Vector3 nEnd );
void processLeftNormal( a3dRailInfo *ri, Vector3 axis, Vector3 normal );
void processRightNormal( a3dRailInfo *ri, Vector3 axis, Vector3 normal );
void processRailManagerLightPlane( a3dLightMapPlaneInfo *info, void *lpData );


#endif