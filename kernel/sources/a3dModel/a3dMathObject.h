#ifndef A3D_MATH_OBJECT_INC
#define A3D_MATH_OBJECT_INC

#include "serialization.h"
#include "..\\boundBox.h"

class a3dMathObject : public a3dSerializableObject { 
public:
	Matrix4						basis;
	Matrix4						invertBasis;
	float						scaleX, scaleY, scaleZ;
	a3dBoundBox					bVolume;

	a3dMathObject() { this->initializeDefault(); }
	~a3dMathObject() { this->clear(); }

	virtual void initializeDefault();
	virtual void clear();
	virtual void calcBVolume();

	void	position( Vector3 v );
	void	position( float x, float y, float z );
	void	moveX( float f );
	void	moveY( float f );
	void	moveZ( float f );
	void	moveV( Vector3 v );
	void	moveV( float x, float y, float z );

	void    rotateAroundX(float angle);
	void    rotateAroundY(float angle);
	void    rotateAroundZ(float angle);

	void    rotateAroundV( Vector3 v, float angle );
	void    rotateAroundA( Vector3 p, Vector3 a, float angle );

	void	direction( Vector3 newZ, Vector3 newY );
	void	rotateHead( float angle );

	void	scale( float fx, float fy, float fz );
	void    origin();
	virtual void correct();
	void	correct( Matrix4 modBasis );
	void	setBasis3( Matrix4 newbas );

	int		calcSizeCore();
	int		getHeaderCode() { return a3dHeaders::a3dMathObjectHeader; }
	bool	saveToMemoryCore( a3dMemoryFile &file );
	bool	loadFromMemoryCore( a3dMemoryFile &file );

};

#endif