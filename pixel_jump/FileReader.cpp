/******************************************************************************/
/*!
	Include Files
*/
/******************************************************************************/
#include "main.h"

//-----------------------------------------------------------------------------/
namespace FileReader {
	s32 ImportMapDataFromFile(std::string s_FileName, 
							  s32& r_BINARY_MAP_WIDTH, 
							  s32& r_BINARY_MAP_HEIGHT, 
							  s32**& r_MapData, 
							  s32**& r_BinaryCollisionArray) {
		std::fstream file;
		file.open(s_FileName);

		if (!file.is_open()) {
			AE_ASSERT_MESG(0, "Failed to open file");
			return 0;
		}

		// get the width and height of the map
		std::string s_Temp;
		file >> s_Temp >> r_BINARY_MAP_WIDTH;
		file >> s_Temp >> r_BINARY_MAP_HEIGHT;

		// allocate memory for the map data
		r_MapData = new s32 * [r_BINARY_MAP_WIDTH];
		// allocate memory for the binary collision array
		r_BinaryCollisionArray = new s32 * [r_BINARY_MAP_WIDTH];

		for (s32 i{}; i < r_BINARY_MAP_WIDTH; ++i) {
			r_MapData[i] = new s32[r_BINARY_MAP_HEIGHT];
			r_BinaryCollisionArray[i] = new s32[r_BINARY_MAP_HEIGHT];
		}

		s32 temp{};

		// loops through each element in the map data and binary collision array
		for (s32 j{}; j < r_BINARY_MAP_HEIGHT; ++j) {
			for (s32 n{}; n < r_BINARY_MAP_WIDTH; ++n) {
				file >> temp;
				r_MapData[n][j] = temp;

				if (temp == 1) {
					r_BinaryCollisionArray[n][j] = 1;
				}
				else {
					r_BinaryCollisionArray[n][j] = 0;
				}
			}
		}
		file.close();
		return 1;
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
}

