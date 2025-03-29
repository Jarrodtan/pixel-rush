/*******************************************************************/
/*!
	File:		Button.h
	Project:	Pixel Rush
	Author(s):  Lau Xiang Yang

	Brief:		All Functional Declaration to use for Button.cpp file

	All content © 2025 DigiPen Institute of Technology Singapore.
	All rights reserved.
*/
/*******************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#include "main.h"

// Button class for button function
class Button {
	enum STATE {
		STATE_IDLE = 0,
		STATE_HOVER,
		STATE_PRESSED,

		STATE_MAX
	};

	STATE state;
	std::string s_TextToDisplay, s_Data;
	f32 f_PositionX, f_PositionY, f_Width, f_Height, f_RedStroke, f_GreenStroke, f_BlueStroke;

public:
	Button();
	void Init(f32 f_PositionX, f32 f_PositionY, f32 f_Width, f32 f_Height, std::string s_TextToDisplay, std::string data, f32 f_Red = 0.75, f32 f_Green = 0.75f, f32 f_Blue = 0.75);
	bool Update(f32 f_MousePosX, f32 f_MousePosY, bool b_MouseTriggered, bool b_MouseReleased);
	void Render(f32 f_ScreenOffsetX, f32 f_ScreenOffsetY) const;
};

#endif