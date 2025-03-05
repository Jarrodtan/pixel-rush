/**********************************************************/
/*!
	File:	Camera.h
	Author:
	Email:

	Brief:

	Copyright © 2025 DigiPen, All rights reserved.
*/
/**********************************************************/

#ifndef CAMERA_H
#define CAMERA_H

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include <AEEngine.h>
//-------------------------------------------------------------------------/

/******************************************************************************/
/*!
	Camera Class
*/
/******************************************************************************/
class Camera
{
public:
	void applyCameraTransform(AEMtx33& concatTo, f32 anchorX, f32 anchorY);
	void changeLevelSettings(f32 width, f32 height);
	void changeZoomVal(s32 zoom);
	Camera(f32 levelHeight, f32 levelWidth);
	Camera();
	~Camera();

private:
	f32 levelWidth;
	f32 levelHeight;
	f32 zoomValue;
};
//-------------------------------------------------------------------------/

#endif