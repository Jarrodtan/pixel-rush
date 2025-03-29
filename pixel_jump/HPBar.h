/*******************************************************************/
/*!
	File:		HPBar.h
	Project:	Pixel Rush
	Author(s):  Ng Wei Zhen

	Brief:		All Functional Declaration to use for HPBar.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef HPBAR_H
#define HPBAR_H

// Variables
extern int maxhealth;
extern int currenthealth;

extern float maxhp;
extern float currenthp;
extern float timedelta;


// All mesh and UI creation transform/render functions 

AEGfxVertexList* mesh_create_square(u32 colour);

AEGfxVertexList* mesh_create_hpbar(float x1, float y1, float x2, float y2, float x3, float y3);

void mesh_square_transform(float posX, float posY, float sizeX, float sizeY, f32 rotation, AEMtx33& scale, AEMtx33& rotate, AEMtx33& translate, AEMtx33& transform, float r, float g, float b, float e, float a);

void renderUI_transform(float posX, float posY, float sizeX, float sizeY, f32 rotation, AEMtx33& scale, AEMtx33& rotate, AEMtx33& translate, AEMtx33& transform);

int calculateDistance(float src_posX, float src_posY, float dst_posX, float dst_posY, float distance);

void playerhplock();

void hpcheck();

s32 GameOver(); 

#endif