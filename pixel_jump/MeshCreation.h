/**********************************************************/
/*!
	File:	MeshCreation.h
	Author:
	Email:

	Brief:

	Copyright © 2025 DigiPen, All rights reserved.
*/
/**********************************************************/

#ifndef MESHCREATION_HEADER
#define MESHCREATION_HEADER

#define ARRAY_COUNT(a) (sizeof(a)/sizeof(*a))

/******************************************************************************/
/*!
	Mesh Class
*/
/******************************************************************************/
class Mesh {
private:
	AEGfxVertexList* squareMesh = 0;
	AEGfxVertexList* circleMesh = 0;
	AEMtx33 squareScale, circleScale;
	AEMtx33 squareTranslate, circleTranslate;
public:
	void CreateSquareMesh();
	void CreateCircleMesh();
	void RenderSquareMesh();
	void RenderCircleMesh();
};
//-------------------------------------------------------------------------/

#endif // MESHCREATION_HEADER