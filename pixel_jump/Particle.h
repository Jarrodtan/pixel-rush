/*******************************************************************/
/*!
	File:		Particle.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang
	Brief: All Functional Declaration to use for Particle.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"

/**************************************************************************/
/*!
	Particle class containing all the data for each particle
*/
/**************************************************************************/
class Particle {
public:
	AEVec2 position,
		   scale,
		   velocity;

	AEMtx33 transform;

	f32 transparency,
		duration;
};

/******************************************************************************/
/*!
	Load meshes and initialize variables
*/
/******************************************************************************/
//void load_particle();
/******************************************************************************/
/*!
	Spawn particle and initialize particle data
*/
/******************************************************************************/
//void Spawn_Particle(AEVec2 Pos);
/******************************************************************************/
/*!
	Update the position, transformation and the transparency of each
	particle
*/
/******************************************************************************/
//void Update_Particle();
/******************************************************************************/
/*!
	Calculate final tansformation and draw each particle
*/
/******************************************************************************/
//void Draw_Particle(AEMtx33 Transformation);
/******************************************************************************/
/*!
	Unloads the mesh
*/
/******************************************************************************/
//void Unload_Particle();


#endif // PARTICLE_HEADER