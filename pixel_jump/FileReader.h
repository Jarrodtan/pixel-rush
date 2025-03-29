/*******************************************************************/
/*!
	File:		FileReader.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang		(50% of Code)
				Mira Tristan Dumlao (50% of Code)
	
	Brief:		All Functional Declaration to use for FileReader.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef FILE_READER_HEADER
#define FILE_READER_HEADER

/******************************************************************************/
/*!
	namespace File Reader
*/
/******************************************************************************/
namespace FileReader {
	using MetaData = std::pair<Objects::Index, Objects::Index>;
	void readMeta(std::string fileName, std::vector<MetaData>& container);
	s32 ImportMapDataFromFile(std::string s_FileName, s32& r_BINARY_MAP_WIDTH, s32& r_BINARY_MAP_HEIGHT, s32**& r_MapData, s32**& r_BinaryCollisionArray);
	void FreeMapData(s32& r_BINARY_MAP_WIDTH, s32**& r_MapData, s32**& r_BinaryCollisionArray);
} // FileReader

//-----------------------------------------------------------------------------/

#endif // FILE_READER_HEADER