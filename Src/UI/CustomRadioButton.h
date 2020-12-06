//
//  @file CustomRadioButton.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 27.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef CUSTOMRADIOBUTTON_H
#define CUSTOMRADIOBUTTON_H

#include <FL/Fl_Light_Button.H>
#include <FL/fl_draw.H>
#include  "../HelperDefs.h"

class CCustomRadioButton : public Fl_Light_Button
{
public:

	CCustomRadioButton(int x, int y, int w, int h, const char *l = 0) : Fl_Light_Button(x, y, w, h, l)
	{
		color(UICOLOR_BORDER);
		selection_color(UICOLOR_TEXT);
		box(FL_NO_BOX);
		down_box(FL_ROUND_DOWN_BOX);
		align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
		type(FL_RADIO_BUTTON);
	}

private:

	virtual void draw()
	{

		//Fl_Light_Button.cxx > draw()

		int W  = labelsize();
		int bx = 1;//Fl::box_dx(box());	// box frame width
		int dx = bx + 2;		// relative position of check mark etc.
		int dy = (h() - W) / 2;	// neg. offset o.k. for vertical centering
		int lx = 0;			// relative label position (STR #3237)

		fl_color(color());
		fl_arc(x() + dx + 1, y() + dy + 1, W - 2, W - 2, 0.0, 360.0);
		if (value())
		{
			int tW = (W - Fl::box_dw(down_box())) / 2 + 1;
			if ((W - tW) & 1)
				tW++; // Make sure difference is even to center
			int tdx = dx + (W - tW) / 2;
			int tdy = dy + (W - tW) / 2;
			fl_color(selection_color());
			tW --;
			fl_pie(x() + tdx - 1, y() + tdy - 1, tW + 3, tW + 3, 0.0, 360.0);
		}

		lx = dx + W + 2;

		draw_label(x() + lx, y(), w() - lx - bx, h());
		if (Fl::focus() == this)
			draw_focus();

	}

};

#endif

