/*******************************************************************/
/*!
	File:		FileReader.cpp
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang			(50% of Code)
				Mira Tristan Dumlao		(50% of Code)

	Brief:		Read binary text files and convert it into a level
				design as an output display on the screen, read
				metafiles as well for the position coordinates of the
				pressure plates too.

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#include "main.h"

/******************************************************************************/
/*!
	namespace File Reader
*/
/******************************************************************************/
namespace FileReader {

	s32 ImportMapDataFromFile(std::string s_FileName, 
							  s32& r_BINARY_MAP_WIDTH, 
							  s32& r_BINARY_MAP_HEIGHT, 
							  s32**& r_MapData, 
							  s32**& r_BinaryCollisionArray) {
		std::fstream file;
		file.open(s_FileName);

		if (!file.is_open()) {
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


				switch (temp) {
				case Objects::TYPE_OBJECT_WALL:
					r_BinaryCollisionArray[n][j] = 1;
					break;
				default:
					r_BinaryCollisionArray[n][j] = 0;
					break;
				}
			}
		}
		file.close();
		return 1;
	}

	void readMeta(std::string fileName, std::vector<MetaData> &container) {
		std::fstream input{ fileName, std::ios_base::in };

		std::string buffer{};
		char stagingChar{};

		while (input && !input.eof()) {

			Objects::Index idx1{};
			input >> idx1.x;
			input >> stagingChar;
			input >> idx1.y;

			Objects::Index idx2{};
			input >> idx2.x;
			input >> stagingChar;
			input >> idx2.y;

			container.push_back(MetaData{idx1, idx2});
			

		}
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
//-----------------------------------------------------------------------------/