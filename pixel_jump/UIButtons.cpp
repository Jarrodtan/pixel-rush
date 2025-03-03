#include "UIButtons.h"
#include "RenderHelper.h"
#include <AETypes.h>

namespace {
	int IsButtonClicked(f64 d_CenterX, f64 d_CenterY, f64 d_HalfWidth, f64 d_HalfHeight, f64 d_ClickX, f64 d_ClickY) {
		s32 result = 0;
		if (d_ClickX > d_CenterX - d_HalfWidth && d_ClickX < d_CenterX + d_HalfWidth &&
			d_ClickY > d_CenterY - d_HalfHeight && d_ClickY < d_CenterY + d_HalfHeight) {
			result = 1;
		}
		return result;
	}

	const f32 f_MaxColorAlpha = 1.f;
	const f32 f_ButtonBorderWidth = 2.f;
}

Button::Button() :
	state(BUTTON_STATE::UI_BUTTON_IDLE),
	text(std::string{ "" }),
	data(std::string{ "" }),
	x(0.f), y(0.f), f_Width(0.f), f_Height(0.f),
	r(0.f), g(0.f), b(0.f),
	nextFunction(nullptr)
{}

void Button::Init(f32 f_InitX, f32 f_InitY, f32 f_InitWidth, f32 f_InitHeight, std::string s_Text, std::string s_Initdata, void(*func)(std::string), f32 f_InitR, f32 f_InitG, f32 f_InitB) {
	x = f_InitX;
	y = f_InitY;
	f_Width = f_InitWidth;
	f_Height = f_InitHeight;
	text = s_Text;
	data = s_Initdata;
	nextFunction = func;
	r = f_InitR;
	g = f_InitG;
	b = f_InitB;
}

void Button::Update(f32 f_MouseX, f32 f_MouseY, bool b_MouseClicked, bool b_MouseReleased) {
	if (state == UI_BUTTON_PRESSED) {
		if (!IsButtonClicked(x, y, f_Width * 0.5f, f_Height * 0.5f, f_MouseX, f_MouseY)) {
			state = UI_BUTTON_IDLE;
		}
		else if (b_MouseReleased) {
			state = UI_BUTTON_IDLE;
			nextFunction(data);
		}
	}
	else {
		state = IsButtonClicked(x, y, f_Width * 0.5f, f_Height * 0.5f, f_MouseX, f_MouseY) ? UI_BUTTON_HOVER : UI_BUTTON_IDLE;
		if (state == UI_BUTTON_HOVER && b_MouseClicked) {
			state = UI_BUTTON_PRESSED;
		}
	}
}

void Button::Render(f32 f_ScreenOffsetX, f32 f_ScreenOffsetY) {
	f32 f_FillR{}, f_FillG{}, f_FillB{}, f_StrokeR{ r }, f_StrokeG{ g }, f_StrokeB{ b };

	switch (state) {
	case UI_BUTTON_IDLE:
		break;

	case UI_BUTTON_HOVER:
		f_StrokeR *= 2;
		f_StrokeG *= 2;
		f_StrokeB *= 2;
		break;

	case UI_BUTTON_PRESSED:
		f_FillR = f_StrokeR * 0.5f;
		f_FillG = f_StrokeG * 0.5f;
		f_FillB = f_StrokeB * 0.5f;
		f_StrokeR = f_StrokeG = f_StrokeB = 0.f;
		break;
	}

	AEMtx33 BorderTransform{
		f_Width + f_ButtonBorderWidth * 2.f, 0.f, x - f_ScreenOffsetX,
		0, f_Height + f_ButtonBorderWidth * 2.f, y - f_ScreenOffsetY,
		0, 0, 1
	};

	AEMtx33 trans{
		f_Width, 0, x - f_ScreenOffsetX,
		0, f_Height, y - f_ScreenOffsetY,
		0, 0, 1
	};

	Render::RenderShapes(Render::SHAPES::SQUARE, BorderTransform, f_StrokeR, f_StrokeG, f_StrokeB, f_MaxColorAlpha);
	Render::RenderShapes(Render::SHAPES::SQUARE, trans, f_FillR, f_FillG, f_FillB, f_MaxColorAlpha);
	Render::RenderFont(text, f_Height * 0.5f, x, y, f_StrokeR, f_StrokeG, f_StrokeB, f_MaxColorAlpha);
}
