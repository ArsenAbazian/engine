#ifndef PARTICLES_INC
#define PARTICLES_INC

#include "particleSystemBase.h"
#include "..\\landscape.h"

class a3dSimpleParticleInfo : public a3dParticleInfoBase { 
public:
	bool			firstTime;
	bool			busy;			// ��������� �� �� ������ �� ������� ��� ���
	bool			visible;		// ��������� �� �� ����� �� ������� ��� ���
	float			size;			// ������� ������ ������
	float			creationTime;	// ����� �������� �������
	float			currTime;
	Vector3			beginPos;
	Vector3			currPos;
	Vector3			currSpeed;
	Vector3			beginSpeed;

	a3dSimpleParticleInfo() : a3dParticleInfoBase() { 
		this->firstTime = true;
		this->busy = false;
		this->visible = false;
		this->size = 0.0f;
		this->creationTime = 0.0f;
		this->currTime = 0.0f;
		setVector3( this->beginPos, 0.0f, 0.0f, 0.0f );
		setVector3( this->currPos, 0.0f, 0.0f, 0.0f );
		setVector3( this->currSpeed, 0.0f, 0.0f, 0.0f ); 
		setVector3( this->beginSpeed, 0.0f, 0.0f, 0.0f );
	}
};

typedef enum { sp_state1, sp_state2 } smokeParticleState;

class a3dSmokeParticleInfo : public a3dSimpleParticleInfo {
public:
	float					time1;		// ��� ��������� ��� ���� ��������
	float					time2;		// ��� ��������� ��� ���� ��������
	
	float					size1;
	float					size2;
	float					size3;
	float					dsize1;
	float					dsize2;
	float					alpha;
	float					alpha1;
	float					alpha2;
	float					alpha3;
	float					dalpha1;
	float					dalpha2;
	float					distFromCamera;	// ���������� �� ������ ���������� ��� ��������������
	smokeParticleState		currentState;

	a3dSmokeParticleInfo() : a3dSimpleParticleInfo() { 
		this->time1 = 0.0f;
		this->time2 = 0.0f;
		this->size1 = 0.0f;
		this->size2 = 0.0f;
		this->size3 = 0.0f;
		this->dsize1 = 0.0f;
		this->dsize2 = 0.0f;
		this->alpha = 0.0f;
		this->alpha1 = 0.0f;
		this->alpha2 = 0.0f;
		this->alpha3 = 0.0f;
		this->dalpha1 = 0.0f;
		this->dalpha2 = 0.0f;
		this->currentState = sp_state1;
	}
};

typedef struct {
	Vector3					pos;			// ��������� �������
	float					alpha;			// ����� �����
	Vector2					tex0;			// ���������� ��������
	Vector2					shiftVector;	// ������� + ���� ������ ���� ������� � �������
} smokeParticleVertex;

class a3dSmokeParticleSystem : public particleSystemBase { 
	
	void		quickSortR( int *a, int N ); // ����������� ������� ����������
public:
	float		dirDelta;				// ���������� �����������
	float		smokeSpeed1;			// �������� 1
	float		smokeSpeed1Delta;		// ���������� ��������
	float		smokeTime1;				// ����� ������ ���� �� �����	
	float		smokeTime1Delta;		// ����������
	float		smokeSize1;				// ������ ���� � ������
	float		smokeSize1Delta;		// ���������� ������� ���� � ������
	float		smokeSpeed2;			// �������� 2
	float		smokeSpeed2Delta;		// ���������� �������� 2
	float		smokeTime2;				// ����� �� ������� ��� ������������
	float		smokeTime2Delta;		// ���������� �������
	float		smokeSize2;				// �������� ������ ����
	float		smokeSize2Delta;		// ���������� ��������� ������� ����
	float		smokeSize3;				// �������� ������ ����
	float		smokeSize3Delta;		// ���������� ��������� ������� ����
	float		alpha1;					
	float		alpha1Delta;
	float		alpha2;
	float		alpha2Delta;
	float		alpha3;
	float		alpha3Delta;
	int			particleIndex;			// ��� ���������� ��������

	
	a3dDependencyProperty	*dirDeltaProperty;
	a3dDependencyProperty	*smokeSpeed1Property;
	a3dDependencyProperty	*smokeSpeed1DeltaProperty;
	a3dDependencyProperty	*smokeTime1Property;
	a3dDependencyProperty	*smokeTime1DeltaProperty;
	a3dDependencyProperty	*smokeSize1Property;
	a3dDependencyProperty	*smokeSize1DeltaProperty;
	a3dDependencyProperty	*smokeSpeed2Property;
	a3dDependencyProperty	*smokeSpeed2DeltaProperty;
	a3dDependencyProperty	*smokeTime2Property;
	a3dDependencyProperty	*smokeTime2DeltaProperty;
	a3dDependencyProperty	*smokeSize2Property;
	a3dDependencyProperty	*smokeSize2DeltaProperty;
	a3dDependencyProperty	*smokeSize3Property;
	a3dDependencyProperty	*smokeSize3DeltaProperty;
	a3dDependencyProperty	*alpha1Property;
	a3dDependencyProperty	*alpha1DeltaProperty;
	a3dDependencyProperty	*alpha2Property;
	a3dDependencyProperty	*alpha2DeltaProperty;
	a3dDependencyProperty	*alpha3Property;
	a3dDependencyProperty	*alpha3DeltaProperty;
	
	a3dSmokeParticleSystem() : particleSystemBase() { }

	DWORD __a3d_fastcall	getParticleFVF();
	int __a3d_fastcall		getVertexSize();

	void					registerDependencyProperties();
	
	a3dParticleInfoBase*	createParticles( int count ) { return new a3dSmokeParticleInfo[count]; }
	void					updateParticles( DWORD dwTime );
	void					generateCurrentSmokeParticle( a3dSmokeParticleInfo *p );
	void					fillBuffer( BYTE * ptr );
	bool					checkParams();
	void					calcParticleVisibilityParams( a3dSmokeParticleInfo *p );
	void					sortParticlesByDistance();

	bool					loadParticleDataFromMemoryInternal( a3dMemoryFile &file );
	bool					saveParticleDataToMemoryInternal( a3dMemoryFile &file );

	int						getHeaderCode() { return a3dHeaders::a3dSmokeParticleSystemHeader; }
	int						calcParticleSizeCore();
	void					initializeDefault();
};

class a3dStaticParticleSystem : public particleSystemBase { 
	
public:
	a3dStaticParticleSystem() : particleSystemBase() { }

};

class landscapePatch;
class a3dTreeParticleInfo : public a3dParticleInfoBase { 
public:
	float				distFromCamera;	// ���������� �� ������ ���������� ��� ��������������
	float				size;
	landscapePatch		*ownerPatch;		// ����� ��������� � ������� ��������� ������
	bool				visible;		

	a3dTreeParticleInfo() : a3dParticleInfoBase() { 
		this->distFromCamera = 0.0f;
		this->size = 1.0f;
		this->ownerPatch = null;
		this->visible = false;
	}
};

typedef struct {
	Vector3					pos;			// ��������� �������
	Vector2					tex0;			// ���������� ��������
	Vector2					shiftVector;	// ������� + ���� ������ ���� ������� � �������
} treeParticleVertex;

class a3dLandscape;
class a3dParticleInfoBase;
class a3dTreeParticleSystem : public a3dStaticParticleSystem { 
	void		quickSortR( int *a, int N ); // ����������� ������� ����������
public:
	a3dLandscape	*land;				// ��������
	shString		grassFileName;		// ��� ����� ������� �����
	Color888		treeColor;			// ���� ������
	shString		treeName;			// ��� �������
	
	float			offsetX;			// �������� �� x
	float			offsetZ;			// �������� �� y

	float			width;				// ������ �������
	float			height;				// ������ �������

	int				mapWidth;			
	int				mapHeight;	

	float			minSize;			// ����������� ������ �������
	float			maxSize;			// ������������ ������ �������

	a3dTreeParticleSystem() : a3dStaticParticleSystem() { 
		
		this->land = null;
		this->offsetX = 0.0f;
		this->offsetZ = 0.0f;
		this->width = 1.0f;
		this->height = 1.0f;
		this->minSize = 1.0f;
		this->maxSize = 1.0f;
		this->mapWidth = 0;
		this->mapHeight = 0;
	}
	
	void					setLandscape( a3dLandscape* land ) { this->land = land; }
	
	int						fillBuffer( int beginIndex, BYTE * ptr );
	a3dParticleInfoBase*	createParticles( int count ) { return new a3dTreeParticleInfo[count]; }
	HRESULT	__a3d_fastcall		initialize( shString &fileName );
	void					updateParticles( DWORD dwTime );
	void					updateHeightByLand( a3dLandscape *land );
	HRESULT __a3d_fastcall		updateBuffer();
	HRESULT __a3d_fastcall		renderCore();
	DWORD __a3d_fastcall		getParticleFVF();
	int __a3d_fastcall			getVertexSize();
	void					sortParticlesByDistance();
	void					calcParticleVisibilityParams( a3dTreeParticleInfo *p );
};

class a3dSprayParticleInfo : public a3dSimpleParticleInfo { 
public:
	Vector3	velocity;

	a3dSprayParticleInfo() : a3dSimpleParticleInfo() { 
		clearVector3( this->velocity );
	}
};

class a3dSprayParticleSystem : public particleSystemBase { 
public:
	int						maxBirthCount;
};

#endif