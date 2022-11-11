#ifndef A3D_LIGHTMAP_GRID_INC
#define A3D_LIGHTMAP_GRID_INC

#include "kernel.h"
#include "lightMapCreator.h"
#include "hlslman.h"
#include "renderHelper.h"
#include "Ncamera.h"

class a3dLightMap;
class a3dMathObject;
class a3dLightMapGrid {
	LPDIRECT3DVERTEXBUFFER9		vertexBuffer;
	int							vertexCount;
	
	Kernel						*kernel;
	camera						*currCamera;
	a3dLightMapCreator			*creator;
	a3dMathObject				*objectWithBasis;
	a3dPointHLSLManager			effectManager;

	void						saveClear();
	void						preCalcLightMaps(a3dLightMap **lm, int lmCount);
	int							calcVertexCount(a3dLightMap **lm, int lmCount);
	HRESULT						createRenderBuffer();
	HRESULT						openRenderBuffer(VOID **b);
	HRESULT						closeRenderBuffer();
	void						clearRenderBuffer();
	void						fillRenderBuffer(a3dLightMap **lm, int lmCount, Vector3 *v);
	HRESULT						createEffect();
	void						deleteLightMaps(a3dLightMap **lm, int lmCount);
public:
	float						pointSize;
	Vector3						color;

	a3dLightMapGrid() { this->initializeDefault(); }
	~a3dLightMapGrid() { this->clear(true); }

	void initializeDefault();
	void clear(bool clearHlslManager);

	void	setKernel(Kernel *krn) { this->kernel = krn; }
	void	setCamera(camera *cam) { this->currCamera = cam; }
	HRESULT	setLightMapCreator( a3dLightMapCreator *crt );

	HRESULT render();
};

#endif