/*******************************************************************/
/*!
	File:		Camera.cpp
	Project:	Pixel Rush
	Author(s):  Mira Tristan Dumlao (90% of Code), Tan Wei Leong (5% of Code), Marco Ng Wei Zhen (5% of Code)
	Brief: The use of Game Camera Feature functions for both players through the zooming effects

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/
/**********************************************************/

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "Camera.h"
#include "main.h"
//-------------------------------------------------------------------------/

// camera transformation matrix
void Camera::applyCameraTransform(AEMtx33& concatTo, f32 anchorX, f32 anchorY) {

	//THE X AND Y COORDINATES TO ANCHOR TO
	f32 snapToX{ this->levelWidth - anchorX };
	f32 snapToY{ this->levelHeight - anchorY };


	AEMtx33 camTrans{ 0 }, zoom{ 0 };
	AEMtx33Scale(&zoom, this->zoomValue, this->zoomValue);
	AEMtx33Trans(&camTrans, snapToX, snapToY);
	AEMtx33Concat(&concatTo, &concatTo, &camTrans);
	AEMtx33Concat(&concatTo, &zoom, &concatTo);

}
// zoom scaling value for camera
void Camera::changeZoomVal(s32 zoom) {
	
	// Clamp zoom range
	if (this->zoomValue > 0.5) {
		this->zoomValue = 0.5;
	}
	if (this->zoomValue < -3) {  // Avoid negative zoom
		this->zoomValue = -3;
	}
	if (this->zoomValue <= 0.5 && this->zoomValue >= -3) {
		this->zoomValue += zoom * 0.015f;
	}
}

// compute level size for camera zooming feature
void Camera::changeLevelSettings(f32 width, f32 height) {
	this->levelWidth = width / 2.0f;
	this->levelHeight = height / 2.0f;
}

Camera::Camera(f32 levelWidth, f32 levelHeight) : levelWidth{ levelWidth / 2 }, levelHeight{ levelHeight / 2 }, zoomValue{ 1 } { }

Camera::Camera() : zoomValue{ 1 }, levelWidth{ 0.0f }, levelHeight{ 0.0f } { }

Camera::~Camera() {}
