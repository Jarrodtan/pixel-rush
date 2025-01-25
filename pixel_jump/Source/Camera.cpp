#include "Camera.h"
#include "AEEngine.h"

void Camera::applyCameraTransform(AEMtx33& concatTo, f32 anchorX, f32 anchorY) {
	f32 snapToX{ this->levelWidth - anchorX };
	f32 snapToY{ this->levelHeight - anchorY };
	AEMtx33 camTrans{ 0 }, zoom{ 0 };
	AEMtx33Scale(&zoom, this->zoomValue, this->zoomValue);
	AEMtx33Trans(&camTrans, snapToX, snapToY);
	AEMtx33Concat(&concatTo, &concatTo, &camTrans);
	AEMtx33Concat(&concatTo, &zoom, &concatTo);

}

void Camera::changeZoomVal(s32 zoom){
	this->zoomValue += zoom * 0.2f;
}

void Camera::changeLevelSettings(f32 width, f32 height) {
	this->levelWidth = width;
	this->levelHeight = height;
}

Camera::Camera(f32 levelWidth, f32 levelHeight) : levelWidth{ levelWidth / 2 }, levelHeight{ levelHeight / 2 }, zoomValue{ 1.0f } { }

Camera::Camera() : zoomValue{ 1.0f }, levelWidth{ 0.0f }, levelHeight{ 0.0f } { }

Camera::~Camera() {}
