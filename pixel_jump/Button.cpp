#include "Button.h"
#include "RenderHelper.h"

namespace {
	s32 IsButtonClicked(f64 d_RectCenterX, f64 d_RectCenterY, f64 d_RectWidth, f64 d_RectHeight, f64 d_MousePosX, f64 d_MousePosY) {
		s32 i_Result = 0;
		if (d_MousePosX > d_RectCenterX - d_RectWidth && d_MousePosX < d_RectCenterX + d_RectWidth &&
			d_MousePosY > d_RectCenterY - d_RectHeight && d_MousePosY < d_RectCenterY + d_RectHeight) {
			i_Result = 1;
		}
		return i_Result;
	}

	const f32 f_MaxColorAlpha = 1.f;
	const f32 f_ButtonBorderWidth = 2.f;
}

Button::Button() :
	state(STATE_IDLE), s_TextToDisplay(std::string{ "" }), s_Data(std::string{ "" }),
	f_PositionX(0.f), f_PositionY(0.f), f_Width(0.f), f_Height(0.f),
	f_RedStroke(0.f), f_GreenStroke(0.f), f_BlueStroke(0.f)
{}

void Button::Init(f32 f_InitPositionX, f32 f_InitPositionY, f32 f_InitWidth, f32 f_InitHeight, std::string s_Text, std::string s_InitData,  f32 f_R, f32 f_G, f32 f_B) {
	f_PositionX = f_InitPositionX;
	f_PositionY = f_InitPositionY;
	f_Width = f_InitWidth;
	f_Height = f_InitHeight;
	s_TextToDisplay = s_Text;
	s_Data = s_InitData;
	f_RedStroke = f_R;
	f_GreenStroke = f_G;
	f_BlueStroke = f_B;
}

bool Button::Update(f32 f_MousePositionX, f32 f_MousePositionY, bool b_MouseTriggered, bool b_MouseReleased) {
	if (state == STATE_PRESSED) {
		if (!IsButtonClicked(f_PositionX, f_PositionY, f_Width * 0.5f, f_Height * 0.5f, f_MousePositionX, f_MousePositionY)) {
			state = STATE_IDLE;
		}
		else if (b_MouseReleased) {
			state = STATE_IDLE;
		}
	}
	else {
		state = IsButtonClicked(f_PositionX, f_PositionY, f_Width * 0.5f, f_Height * 0.5f, f_MousePositionX, f_MousePositionY) ? STATE_HOVER : STATE_IDLE;
		if (state == STATE_HOVER && b_MouseTriggered) {
			state = STATE_PRESSED;
			return true;
		}
	}
	return false;
}

void Button::Render(f32 f_ScreenOffsetX, f32 f_ScreenOffsetY) const {
	f32 f_Rfill = 0.f, f_Gfill = 0.f, f_Bfill = 0.f;
	f32 f_Rstroke = f_RedStroke, f_Gstroke = f_GreenStroke, f_Bstroke = f_BlueStroke;

	switch (state) {
	case STATE_IDLE:
		break;

	case STATE_HOVER:
		f_Rstroke *= 2.f;
		f_Gstroke *= 2.f;
		f_Bstroke *= 2.f;
		break;

	case STATE_PRESSED:
		f_Rfill = f_Rstroke * 0.5f;
		f_Gfill = f_Gstroke * 0.5f;
		f_Bfill = f_Bstroke * 0.5f;
		f_Rstroke = f_Gstroke = f_Bstroke = 0.f;
		break;
	}

	AEMtx33 BorderTrans{
		f_Width + f_ButtonBorderWidth * 2, 0, f_PositionX - f_ScreenOffsetX,
		0, f_Height + f_ButtonBorderWidth * 2, f_PositionY - f_ScreenOffsetY,
		0, 0, 1
	};

	AEMtx33 trans{
		f_Width, 0, f_PositionX - f_ScreenOffsetX,
		0, f_Height, f_PositionY - f_ScreenOffsetY,
		0, 0, 1
	};

	Render::RenderShapes(Render::SHAPES::SQUARE, BorderTrans, f_Rstroke, f_Gstroke, f_Bstroke, f_MaxColorAlpha);
	Render::RenderShapes(Render::SHAPES::SQUARE, trans, f_Rfill, f_Gfill, f_Bfill, f_MaxColorAlpha);

	Render::RenderFont(s_TextToDisplay, 0.8f, f_PositionX, f_PositionY, f_Rstroke, f_Gstroke, f_Bstroke, f_MaxColorAlpha);
}
