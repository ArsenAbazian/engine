#ifndef A3D_RENDER_HELPER
#define A3D_RENDER_HELPER

#include "kernel.h"
#include "boundBox.h"
#include "hlslman.h"
#include "ncamera.h"
#include "3dtypes.h"

class a3dLineHLSLManager : public HLSLManager { 
public:
	HRESULT createEffect(LPDIRECT3DDEVICE9 lpDevice);
};

class a3dPointHLSLManager : public HLSLManager {
public:
	HRESULT createEffect(LPDIRECT3DDEVICE9 lpDevice);
};

class a3dLambertHLSLManager : public HLSLManager { 
public:
	HRESULT createEffect(LPDIRECT3DDEVICE9 lpDevice);
};

class a3dBoundBoxRender { 
public:
	LPDIRECT3DVERTEXBUFFER9			vertexBuffer;
	Kernel							*hkrn;
	Vector3							color;
	Matrix4							basis;
	a3dLineHLSLManager				effectManager;
	camera							*currCamera;

	a3dBoundBoxRender() { 
		this->hkrn = null;
		this->vertexBuffer = null; 
		setVector3( color, 1.0f, 1.0f, 1.0f );
	}

	a3dBoundBoxRender(Kernel *krn) { 
		this->hkrn = krn;
		this->vertexBuffer = null; 
		setVector3( color, 1.0f, 1.0f, 1.0f );
	}

	~a3dBoundBoxRender() { this->clear(); }

	void		clear(); 
	void		setKernel(Kernel *krn) { this->hkrn = krn; }
	void		setCamera(camera *cam) { this->currCamera = cam; }
	HRESULT		initialize();
	HRESULT		render( a3dBoundBox *bVolume );
};

class a3dModel;
class a3dEffectDesc;

class a3dRendererUsingModel {
public:	
	a3dLambertHLSLManager effectManager;
	a3dEffectDesc *effectDesc;
	Kernel		*kernel;
	camera		*currCamera;
	a3dModel	*model;

	a3dRendererUsingModel() { this->initializeDefault(); }
	~a3dRendererUsingModel() { this->clear(); }

	void				initializeDefault();
	void				clear();

	void				setKernel( Kernel *kern ) { this->kernel = kern; }
	void				setCamera( camera *cam ) { this->currCamera = cam; }

	void				update(Matrix4 bas);
	HRESULT				render();
	HRESULT				initialize();

	bool				loadModelFromMemory();
	virtual BYTE		*getModelBuffer();
	virtual int			getModelBufferSize();
};

class a3dBasisRender : public a3dRendererUsingModel {
public:
	BYTE*				getModelBuffer();
	int					getModelBufferSize();
};

class a3dOmniLightRender : public a3dRendererUsingModel {
public:
	BYTE*				getModelBuffer();
	int					getModelBufferSize();
};

class a3dDirectionalLightRender : public a3dRendererUsingModel {
public:
	BYTE*				getModelBuffer();
	int					getModelBufferSize();
};

class a3dSprayParticleRender : public a3dRendererUsingModel {
public:
	BYTE*				getModelBuffer();
	int					getModelBufferSize();
};

#endif