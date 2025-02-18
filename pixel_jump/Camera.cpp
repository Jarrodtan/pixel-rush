#include "Camera.h"
#include "AEEngine.h"
#include <iostream>

void Camera::applyCameraTransform(AEMtx33& concatTo, f32 anchorX, f32 anchorY) {
	f32 snapToX{ this->levelWidth - anchorX };
	f32 snapToY{ this->levelHeight - anchorY };
	AEMtx33 camTrans{ 0 }, zoom{ 0 };
	AEMtx33Scale(&zoom, this->zoomValue, this->zoomValue);
	AEMtx33Trans(&camTrans, snapToX, snapToY);
	AEMtx33Concat(&concatTo, &concatTo, &camTrans);
	AEMtx33Concat(&concatTo, &zoom, &concatTo);
}

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


/*void Camera::changeZoomVal(s32 zoom) {

	if (this->zoomValue > 2.0f) {
		this->zoomValue = 2.0f;
	} if (this->zoomValue < 0.5f) {
		this->zoomValue = 0.5f;
	}
	if (this->zoomValue <= 2.0f && this->zoomValue >= 0.5f) {
		this->zoomValue += zoom * 0.03f;
	}
}*/

void Camera::changeLevelSettings(f32 width, f32 height) {
	this->levelWidth = width / 2.0f;
	this->levelHeight = height / 2.0f;
}

Camera::Camera(f32 levelWidth, f32 levelHeight) : levelWidth{ levelWidth / 2 }, levelHeight{ levelHeight / 2 }, zoomValue{ 1 } { }

Camera::Camera() : zoomValue{ 1 }, levelWidth{ 0.0f }, levelHeight{ 0.0f } { }

Camera::~Camera() {}

