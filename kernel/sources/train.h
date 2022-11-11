#ifndef TRAIN_INC
#define TRAIN_INC

#include "a3dModel\\a3dModel.h"
#include "a3dModel\\a3dObject.h"
#include "kernel.h"

class AnimatedModel {
protected:
		int getAnimatedIndex(a3dModel *model, TCHAR *name);
};

class Train3D : public AnimatedModel {

public:
	a3dModel  *model;
	
	// индексы движущихся и важных частей паровоза
	int		bigReel1;
	int		bigReel2;
	int		smallReel1;
	int		smallReel2;
	int		tube1;
	int		tube2;
	int		tube3;
	int		smokeTube;
	Matrix4 bigReel1Basis;
	Matrix4 bigReel2Basis;
	Matrix4	smallReel1Basis;
	Matrix4 smallReel2Basis;
	Matrix4 tube1Basis;
	Matrix4	tube2Basis;
	Matrix4 tube3Basis;
	Matrix4 smokeTubeBasis;
	Vector3	distanceFromBigReel1ToTube1;
	Vector3	distanceFromBigReel2ToTube2;
	Vector3 distanceFromTube2ToTube3;
	float	lengthFromTube2ToTube3;
	float	bigReelAngle;
	float	smallReel1Angle;
	float	smallReel2Angle;
	float	tube2Angle;
	float	reelsKoeff;
	float	distanceFromBigToSmall;

	Train3D();
	~Train3D() { this->clear(); }

	void clear() { 
		this->model = NULL;
	}

	HRESULT initialize(a3dModel *mdl, Kernel *krn);
	bool    initializeAnimatedIndices();
	bool	initializeData();

	void	getBasisNormalizedYVector(Matrix4 mat, Vector3 v);
	void	reset();
	void	setPos(Vector3 pos);
	void	move(float deltaBigReelAngle);
	void	moveDeltaLength( float deltaLength ) { this->move( deltaLength / this->model->scaleX / (this->model->object[ this->bigReel1 ].bVolume.getWidth() * 0.5f) ); }
	a3dObject *getObj( int index ) { return &(this->model->object[ index ]); }
};

#endif