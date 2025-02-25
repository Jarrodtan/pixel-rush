#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "hpbar.hpp"
#include <math.h>

int maxhealth = 10;
int currenthealth = 10;
float maxhp{ 1000 };
float currenthp{ 1000 };
float timedelta{ 10 };



//---------------------Use this code in the scene to display the UI health bar by number of hearts--------------//

/*for (int h = 0; h < currenthealth; h++)
{
	

	object[h].pMesh = mesh_create_square();
	mesh_square_transform(x_offset += addX_offset, 300, 60, 50, 0, object[h].scale, object[h].rotate, object[h].translate, object[h].transform, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	// Tell Alpha Engine to draw the mesh with the above settings.
	AEGfxMeshDraw(object[h].pMesh, AE_GFX_MDM_TRIANGLES);
}*/


AEGfxVertexList* mesh_create_square()
{

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFFFFF, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFFFFFFF, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue



	return AEGfxMeshEnd();
}

AEGfxVertexList* mesh_create_hpbar(float x1, float y1, float x2, float y2, float x3, float y3)
{

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	AEGfxTriAdd(
		x3, y3, 0xFFFFFFFF, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFFFFFFF, 1.0f, 1.0f,   // bottom-right: green
		x1, y1, 0xFFFFFFF, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		x2, y2, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue



	return AEGfxMeshEnd();

}

void mesh_square_transform(float posX, float posY, float sizeX, float sizeY, f32 rotation, AEMtx33& scale, AEMtx33& rotate, AEMtx33& translate, AEMtx33& transform, float r, float g, float b, float e, float a)
{
	AEMtx33Scale(&scale, sizeX, sizeY);

	AEMtx33Rot(&rotate,rotation);

	// Create a translation matrix that translates by
	// 200 in the x-axis and 100 in the y-axis

	AEMtx33Trans(&translate, posX, posY);

	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	// Add more red!!
	AEGfxSetColorToAdd(r, g, b, e);

	AEGfxSetTransform(transform.m);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(a);

}

void playerhplock()
{
	if (currenthealth > maxhealth)
	{
		currenthealth = maxhealth;
	}
	if (currenthealth < 0)
	{
		currenthealth = 0;
	}

	if (currenthp > maxhp)
	{
		currenthp = maxhp;
	}
	if (currenthp < 0)
	{
		currenthp = 0;
	}
}

void hpcheck()
{
	if (currenthp >= maxhp)
	{
		currenthealth = 10;
	}
	else if (currenthp < maxhp && currenthp >= 900)
	{
		currenthealth = 9;
	}
	else if (currenthp < 900 && currenthp >= 800)
	{
		currenthealth = 8;
	}
	else if (currenthp < 800 && currenthp >= 700)
	{
		currenthealth = 7;
	}
	else if (currenthp < 700 && currenthp >= 600)
	{
		currenthealth = 6;
	}
	else if (currenthp < 600 && currenthp >= 500)
	{
		currenthealth = 5;
	}
	else if (currenthp < 500 && currenthp >= 400)
	{
		currenthealth = 4;
	}
	else if (currenthp < 400 && currenthp >= 300)
	{
		currenthealth = 3;
	}
	else if (currenthp < 300 && currenthp >= 200)
	{
		currenthealth = 2;
	}
	else if (currenthp < 200 && currenthp >= 100)
	{
		currenthealth = 1;
	}
	else
	{
		currenthealth = 0;
	}
}

int calculateDistance(float src_posX, float src_posY, float dst_posX, float dst_posY, float distance)
{
	return (sqrt(pow(dst_posX - src_posX, 2) + pow(dst_posY - src_posY, 2)) < distance);
}

s32 GameOver()
{
	if (currenthp < 1)
	{
		//game over screen;
	}
	return 0;
}

s32 Winstate()
{
	return 0;
}

