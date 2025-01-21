/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"

//-----------------------------------------------------------------------------/

s32 ImportMapDataFromFile(const char* p_FileName) {

}

void FreeMapData(s32& r_BINARY_MAP_WIDTH, s32**& r_MapData, s32**& r_BinaryCollisionArray) {
	// free the map data
	for (s32 i{}; i < r_BINARY_MAP_WIDTH; ++i) {
		delete[] r_MapData[i];
		delete[] r_BinaryCollisionArray[i];
	}

	delete[] r_MapData;
	delete[] r_BinaryCollisionArray;

	r_MapData = nullptr;
	r_BinaryCollisionArray = nullptr;
}
