#ifndef A3D_SPLINE_MANAGER
#define A3D_SPLINE_MANAGER

#include "kernel.h"
#include "vecmat.h"

class a3dSplineManager { 
public:
	bool				isClose;			// ��������� ������?
	shString			splineFileName;		// ��� ����� �������
	float				scale;				// ���������������
	Vector3				origin;				// �����
	Vector3				*point;				// ������ �����
	int					pointCount;			// ����� �����

	a3dSplineManager();
	~a3dSplineManager() { this->clear(); }

	void				clear();			// �������
	bool				initialize( shString fileName, float scale, Vector3 origin, bool isClose );
};

#endif