//
//  @file CustomButton.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 27.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include "../HelperDefs.h"

class CCustomButton : public Fl_Button
{
public:

	CCustomButton(int X, int Y, int W, int H, const char *L = 0) : Fl_Button(X, Y, W, H, L)
	{
		SetColor(Fl_Color(UICOLOR_BUTTON), Fl_Color(UICOLOR_PRESSED));
	}

	void SetColor(Fl_Color nBackColor, Fl_Color nPressedColor)
	{
		color(nBackColor);
		selection_color(nPressedColor);
	}

private:

	virtual void draw()
	{
		if (type() == FL_HIDDEN_BUTTON)
			return;
		Fl_Color col = value() ? selection_color() : color();
		DrawBox(x(), y(), w(), h(), col);
		draw_backdrop();
		if (labeltype() == FL_NORMAL_LABEL && value())
		{
			Fl_Color c = labelcolor();
			labelcolor(fl_contrast(c, col));
			draw_label();
			labelcolor(c);
		}
		else
			draw_label();
		if (Fl::focus() == this)
			draw_focus();
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

};

#endif

