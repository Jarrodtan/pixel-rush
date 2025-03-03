#ifndef UI_BUTTONS_H
#define UI_BUTTONS_H

#include <AETypes.h>
#include <string>

class Button {
	enum BUTTON_STATE {
		UI_BUTTON_IDLE = 0,
		UI_BUTTON_HOVER,
		UI_BUTTON_PRESSED,
		
		UI_BUTTON_MAX
	};

public:
	Button();
	void Init(f32 x, f32 y, f32 f_Width, f32 f_Height, std::string text, std::string data, void(*func)(std::string), f32 r = 0.75f, f32 g = 0.75f, f32 b = 0.75f);
	void Update(f32 f_MouseX, f32 f_MouseY, bool b_MouseClicked, bool b_MouseReleased);
	void Render(f32 f_ScreenOffsetX, f32 f_ScreenOffsetY);

private:
	BUTTON_STATE state;
	std::string text, data;
	f32 x, y, f_Width, f_Height, r, g, b;
	void (*nextFunction)(std::string);
};

#endif
