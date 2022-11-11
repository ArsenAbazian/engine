#ifndef A3D_GRID_INC
#define A3D_GRID_INC

#include "kernel.h"
#include "ncamera.h"
#include "renderHelper.h"

class a3dGrid {

	LPDIRECT3DVERTEXBUFFER9		vertexBuffer;
	int							getLinesCount();
	int							getBoldLineCount();
	int							getSimpleLineCount();
	void						setLine( Vector3 *line, float x0, float z0, float x1, float z1 );
	void						fillBoldLines( Vector3 *data );
	void						fillLines( Vector3 *data );

	HRESULT						drawLines( Vector3 color, int startIndex, int count );

public:
	a3dLineHLSLManager	effectManager;
	Kernel		*kernel;
	camera		*currCamera;

	float		sideSize;
	float		smallStep;
	int			boldLineSteps;

	Vector3		axisColor;
	Vector3		lineColor;
	Vector3		boldLineColor;

	a3dGrid() { this->initializeDefault(); }
	~a3dGrid() { this->clear(); }

	void		initializeDefault();
	void		setKernel( Kernel *kernel ) { this->kernel = kernel; }
	void		setCamera( camera *cam ) { this->currCamera = cam; }
	void		clearBuffers();
	HRESULT		initialize( float sideSize, float smallStep, int boldLineStep );
	void		clear();
	HRESULT		render();
};

#endif