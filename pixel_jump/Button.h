#ifndef BUTTON_H
#define BUTTON_H

#include <AETypes.h>
#include <string>

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
	void (*p_Function)(std::string);

public:
	Button();
	void Init(f32 f_PositionX, f32 f_PositionY, f32 f_Width, f32 f_Height, std::string s_TextToDisplay, std::string data, void (*p_Function)(std::string), f32 f_Red = 0.75, f32 f_Green = 0.75f, f32 f_Blue = 0.75);
	void Update(f32 f_MousePosX, f32 f_MousePosY, bool b_MouseTriggered, bool b_MouseReleased);
	void Render(f32 f_ScreenOffsetX, f32 f_ScreenOffsetY) const;
};

#endif