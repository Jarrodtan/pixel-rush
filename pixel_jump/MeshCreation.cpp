/*******************************************************************/
/*!
	File:		MeshCreation.cpp
	Project:	Pixel Rush
	Author(s):	Tan Wei Leong

	Brief:		Mesh Creation for game objects used for rendering
				their graphics and art on screen.

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"
//-------------------------------------------------------------------------/

/******************************************************************************/
/*!
	Create Square Function 
*/
/******************************************************************************/
void Mesh::CreateSquareMesh() {
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f);

	Mesh::squareMesh = AEGfxMeshEnd();
}
//-------------------------------------------------------------------------/

/******************************************************************************/
/*!
	Create Circle Function
*/
/******************************************************************************/
void Mesh::CreateCircleMesh() {
	AEGfxMeshStart();

	int slices = 32; // Set the number of slices for the circle approximation
	for (int i = 0; i < slices; ++i) {
		AEGfxTriAdd(
			0.0f, 0.0f, 0x00000000, 0.5f, 0.5f,
			(f32)cosf((f32)i / slices * 2.0f * PI), (f32)sinf((f32)i / slices * 2.0f * PI), 0x00000000, 0.5f + 0.5f * (f32)cosf((f32)i / slices * 2.0f * PI), 0.5f + 0.5f * (f32)sinf((f32)i / slices * 2.0f * PI),
			(f32)cosf((f32)(i + 1) / slices * 2.0f * PI), (f32)sinf((f32)(i + 1) / slices * 2.0f * PI), 0x00000000, 0.5f + 0.5f * (f32)cosf((f32)(i + 1) / slices * 2.0f * PI), 0.5f + 0.5f * (f32)sinf((f32)(i + 1) / slices * 2.0f * PI)
		);

	}

	Mesh::circleMesh = AEGfxMeshEnd();
}
//-------------------------------------------------------------------------/

/******************************************************************************/
/*!
	Render Square Function
*/
/******************************************************************************/
void Mesh::RenderSquareMesh() {
	AEMtx33 squareTransforms[1] {0};

	// Scale: Size
	AEMtx33Scale(&squareScale, 100.0f, 100.0f);

	// Translate: Position
	AEMtx33Trans(&squareTranslate, 0.0f, 0.0f);

	// Concatenate: Merge
	AEMtx33Concat(&squareTransforms[0], &squareScale, &squareTranslate);

	// Begin Rendering
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

	for (int i = 0; i < ARRAY_COUNT(squareTransforms); ++i) {

		if (i == 0) {
			AEGfxSetColorToAdd(0.3f, 1.0f, 0.3f, 0.0f);
		}

		// Transform - Rendering
		AEGfxSetTransform(squareTransforms[i].m);

		// Draw Mesh
		AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
	}
}
//-------------------------------------------------------------------------/

/******************************************************************************/
/*!
	Render Circle Function
*/
/******************************************************************************/
void Mesh::RenderCircleMesh() {
	AEMtx33 circleTransforms[1]{ 0 };

	// Scale: Size
	AEMtx33Scale(&circleScale, 100.0f, 100.0f);

	// Translate: Position
	AEMtx33Trans(&circleTranslate, 0.0f, 0.0f);

	// Concatenate: Merge
	AEMtx33Concat(&circleTransforms[0], &circleScale, &circleTranslate);

	// Begin Rendering
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

	for (int i = 0; i < ARRAY_COUNT(circleTransforms); ++i) {

		if (i == 0) {
			AEGfxSetColorToAdd(0.3f, 1.0f, 0.3f, 0.0f);
		}

		// Transform - Rendering
		AEGfxSetTransform(circleTransforms[i].m);

		// Draw Mesh
		AEGfxMeshDraw(circleMesh, AE_GFX_MDM_TRIANGLES);
	}
}
//-------------------------------------------------------------------------/