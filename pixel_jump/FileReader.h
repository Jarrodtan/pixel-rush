#ifndef FILE_READER_HEADER
#define FILE_READER_HEADER

/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "AEEngine.h"

//-----------------------------------------------------------------------------/

// Reading Map Data
s32 ImportMapDataFromFile(std::string s_FileName, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT, s32**& r_MapData, s32**& r_BinaryCollisionArray);
void FreeMapData(s32& r_BINARY_MAP_WIDTH, s32**& r_MapData, s32**& r_BinaryCollisionArray);

// TODO: Reading defines data



#endif // FILE_READER_HEADER