// cameraMapper.h

#pragma once

#include "vecMatMapper.h"
#include "kernelMapper.h"

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;
using namespace System::Globalization;
using namespace System::Collections;
using namespace System::Windows::Forms::Design;

namespace classMapper {
	
	public ref class camera_Mapper { 
	internal:	
		float fovy;
		float aspect;
		float nearZ;
		float farZ;
		Vector3_Mapper^ pos;
		camera *cam;

	public:
		camera_Mapper() { 
			this->cam = new camera();
			this->fovy = 45.0f;
			this->aspect = 1.0f;
			this->nearZ = 1.0f;
			this->farZ = 1000.0f;
			this->pos = Vector3_Mapper::Empty;
			this->initialize();
		}
		
		void initialize() { 
			this->initialize( FovY, Aspect, NearZ, FarZ );
		}
		
		void initialize( float fovy, float aspect, float nearZ, float farZ ) { 
			cam->initialize( fovy, aspect, nearZ, farZ );
		} 
		
		void position( Vector3_Mapper^ vec ) {
			cam->position( vec->X, vec->Y, vec->Z );
		}
		
		property float FovY { 
			float get() { return fovy; }
			void set( float value ) {  
				fovy = value;
				this->initialize( FovY, Aspect, NearZ, FarZ );
			}
		} 
	
		property float Aspect { 
			float get() { return aspect; }
			void set( float value ) {  
				aspect = value;
				this->initialize( FovY, Aspect, NearZ, FarZ );
			}
		}

		property float NearZ { 
			float get() { return nearZ; }
			void set( float value ) { 
				nearZ = value;
				this->initialize( FovY, Aspect, NearZ, FarZ );
			}
		}

		property float FarZ { 
			float get() { return farZ; }
			void set( float value ) { 
				farZ = value;
				this->initialize( FovY, Aspect, NearZ, FarZ );
			}
		}
	
		property Vector3_Mapper^ Position { 
			Vector3_Mapper^ get() { return pos; }
			void set( Vector3_Mapper^ value ) { 
				pos = value;
				this->position( pos );
			}
		}

		property Vector3_Mapper ^ AxisX { 
			Vector3_Mapper ^ get() { return gcnew Vector3_Mapper( this->cam->getCameraXAxis(0), this->cam->getCameraXAxis(1), this->cam->getCameraXAxis(2) ); }
		}

		property Vector3_Mapper ^ AxisY { 
			Vector3_Mapper ^ get() { return gcnew Vector3_Mapper( this->cam->getCameraYAxis(0), this->cam->getCameraYAxis(1), this->cam->getCameraYAxis(2) ); }
		}

		property Vector3_Mapper ^ AxisZ { 
			Vector3_Mapper ^ get() { return gcnew Vector3_Mapper( this->cam->getCameraZAxis(0), this->cam->getCameraZAxis(1), this->cam->getCameraZAxis(2) ); }
		}

		void UpdateCamera( Kernel_Mapper^ kernel ) { 
			this->cam->setDevice( kernel->krn->lpDevice );
			this->cam->preapreViewProj();
			this->cam->setState( );
			this->cam->setProjection( );
			this->cam->setScreen();
		}

		property int ViewPortWidth { 
			int get() { return this->cam->viewport.Width; }
			void set(int value) { this->cam->viewport.Width = value; }
		}

		property int ViewPortHeight { 
			int get() { return this->cam->viewport.Height; }
			void set(int value) { this->cam->viewport.Height = value; }
		}
		
		void direction( Vector3_Mapper^ newZDir, Vector3_Mapper^ newYDir ) { 
			this->cam->direction( newZDir->X, newZDir->Y, newZDir->Z, newYDir->X, newYDir->Y, newYDir->Z );    
		}
		
		void direction( float newZDirX, float newZDirY, float newZDirZ, float newYDirX, float newYDirY, float newYDirZ ) { 
			this->cam->direction( newZDirX, newZDirY, newZDirZ, newYDirX, newYDirY, newYDirZ );
		}
		void rotateAroundCameraX( float angle ) { 
			this->cam->rotateAroundCameraX( angle );
		}
		void rotateAroundCameraY( float angle ) { 
			this->cam->rotateAroundCameraY( angle );
		}
		void rotateAroundCameraZ( float angle ) { 
			this->cam->rotateAroundCameraZ( angle );
		}
		void rotateAroundVector( Vector3_Mapper^ vec, float angle ) {
			Vector3 v = { vec->X, vec->Y, vec->Z };
			this->cam->rotateAroundVector( v, angle );
		}
		
		void rotateAroundAxis( Vector3_Mapper^ pos, Vector3_Mapper^ axis, float angle ) { 
			Vector3 p = { pos->X, pos->Y, pos->Z };
			Vector3 a = { axis->X, axis->Y, axis->Z };
		
			this->cam->rotateAroundAxis( p, a, angle );
		}
		
		void moveByCameraX( float distance ) { this->cam->moveByCameraX( distance ); }
		void moveByCameraY( float distance ) { this->cam->moveByCameraY( distance ); }
		void moveByCameraZ( float distance ) { this->cam->moveByCameraZ( distance ); }
		void moveByVector( Vector3_Mapper^ vec ) { 
			Vector3 v = { vec->X, vec->Y, vec->Z };
			this->cam->moveByVector( v );
		}
		void walkFB( float distance ) { this->cam->walkFB( distance ); }
		void strafeLR( float distance ) { this->cam->strafeLR( distance ); }
		void liftUD( float distance ) { this->cam->liftUD( distance ); }

		float getCameraXAxis( int index ){ return this->cam->state[index][0]; }
		float getCameraYAxis( int index ){ return this->cam->state[index][1]; }
		float getCameraZAxis( int index ){ return this->cam->state[index][2]; }
		float getCameraPos( int index ){ return this->cam->pos[index]; }
		float getFrustumApexPos( int index ){ return this->cam->frustumApex[index]; }
	};

}