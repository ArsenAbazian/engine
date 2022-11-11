#ifndef A3D_SPLINE_MANAGER
#define A3D_SPLINE_MANAGER

#include "kernel.h"
#include "vecmat.h"

class a3dSplineManager { 
public:
	bool				isClose;			// замкнутая фигура?
	shString			splineFileName;		// имя файла сплайна
	float				scale;				// масштабирование
	Vector3				origin;				// базис
	Vector3				*point;				// массив точек
	int					pointCount;			// число точек

	a3dSplineManager();
	~a3dSplineManager() { this->clear(); }

	void				clear();			// очистка
	bool				initialize( shString fileName, float scale, Vector3 origin, bool isClose );
};

#endif