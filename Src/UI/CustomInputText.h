//
//  @file CustomInputText.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 27.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef CUSTOMINPUTTEXT_H
#define CUSTOMINPUTTEXT_H

#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include  "../HelperDefs.h"

class CCustomInputText : public Fl_Input
{
public:

	CCustomInputText(int X, int Y, int W, int H, const char *L = 0) : Fl_Input(X, Y, W, H, L)
	{
		m_fCursorVisible = true;
		SetColor(UICOLOR_BACKGROUND, UICOLOR_SELECTION);
	}

	void SetColor(Fl_Color nBackColor, Fl_Color nPressedColor)
	{
		color(nBackColor);
		selection_color(nPressedColor);
	}

private:

	bool m_fCursorVisible;

	CASTSIMPLEHANDLER(CCustomInputText, OnBlinkCursor);

	virtual void draw()
	{
		if (input_type() == FL_HIDDEN_INPUT)
			return;
		Fl_Boxtype b = box();
		if (damage() & FL_DAMAGE_ALL)
			DrawBox(x(), y(), w(), h(), color());
		Fl_Input_::drawtext(x() + Fl::box_dx(b), y() + Fl::box_dy(b), w() - Fl::box_dw(b), h() - Fl::box_dh(b));
	}

	void DrawBox(int x, int y, int w, int h, Fl_Color c)
	{

		Fl::set_box_color(Fl_Color(UICOLOR_BORDER));
		fl_begin_loop();
			fl_vertex(x, y + 2);
			fl_vertex(x + 2, y);
			fl_vertex(x + w - 3, y);
			fl_vertex(x + w - 1, y + 2);
			fl_vertex(x + w - 1, y + h - 3);
			fl_vertex(x + w - 3, y + h - 1);
			fl_vertex(x + 2, y + h - 1);
			fl_vertex(x, y + h - 3);
		fl_end_loop();

		Fl::set_box_color(c);
		fl_xyline(x + 2, y + 1, x + w - 3);
		fl_yxline(x + 1, y + 2, y + h - 3);
		fl_yxline(x + 2, y + h - 2, y + 2, x + w - 2);

		fl_rectf(x + 3, y + 3, w - 5, h - 4);
		fl_yxline(x + w - 2, y + 3, y + h - 3);

	}

	int handle(int event)
	{
		if(event == FL_FOCUS)
		{
			m_fCursorVisible = true;
			Fl::add_timeout(0.5, SOnBlinkCursor, this);
			readonly(false);
		}
		else if(event == FL_UNFOCUS)
		{
			Fl::remove_timeout(SOnBlinkCursor, this);
			redraw();
		}
		else if(event == FL_KEYDOWN || event == FL_KEYUP)
		{
			m_fCursorVisible = true;
			cursor_color(Fl_Color(0));
			readonly(false);
			redraw();
		}
		return Fl_Input::handle(event);
	}

	void OnBlinkCursor()
	{
		m_fCursorVisible = !m_fCursorVisible;
		if(m_fCursorVisible)
		{
			cursor_color(Fl_Color(0));
			readonly(false);
		}
		else
		{
			cursor_color(color());
			readonly(true);
		}
		redraw();
		Fl::repeat_timeout(0.5, SOnBlinkCursor, this);
	}

};

#endif

