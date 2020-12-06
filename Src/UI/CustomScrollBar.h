//
//  @file CustomScrollBar.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 27.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef CUSTOMSCROLLBAR_H
#define CUSTOMSCROLLBAR_H

#include <FL/Fl_Scrollbar.H>
#include <FL/fl_draw.H>
#include  "../HelperDefs.h"

class CCustomScrollbar : public Fl_Scrollbar
{
public:

	CCustomScrollbar(int X, int Y, int W, int H, const char *L = 0) : Fl_Scrollbar(X, Y, W, H, L)
	{
	}

private:

	virtual void draw()
	{
		//Fl_Scrollbar.cxx > Fl_Scrollbar::draw()
		Fl::set_box_color(Fl_Color(UICOLOR_BORDER));
		fl_xyline(x(), y(), x() + w());
		fl_xyline(x(), y() + h() - 1, x() + w());
		fl_yxline(x(), y(), y() + h());
		fl_yxline(x() + w() - 1, y(), y() + h());
		//if (damage() & FL_DAMAGE_ALL)
			//draw_box();
		int X = x() + 0;//Fl::box_dx(box());
		int Y = y() + 0;//Fl::box_dy(box());
		int W = w() - 1;//Fl::box_dw(box());
		int H = h() - 1;//Fl::box_dh(box());
		if (horizontal())
		{
			fl_yxline(X + H - 1, Y, Y + H);
			fl_yxline(X + W - H, Y, Y + H);
			if (W < 3 * H)
			{
				//Fl_Slider::draw(X, Y, W, H);
				return;
			}
			//Fl_Slider::draw(X + H, Y, W - 2 * H, H);
			fl_rectf(X + H, Y + 1, W - 2 * H, H - 1, Fl_Color(UICOLOR_EMPTY));
			DrawSlider(X + H, Y + 1, W - 2 * H, H - 1);
			if (damage() & FL_DAMAGE_ALL)
			{
				//draw_box((pushed_==1) ? fl_down(slider()) : slider(), X, Y, H, H, selection_color());
				//draw_box((pushed_==2) ? fl_down(slider()) : slider(), X + W - H, Y, H, H, selection_color());
				fl_rectf(X + 1, Y + 1, H - 2, H - 2, Fl_Color(UICOLOR_BUTTON));
				fl_rectf(X + W - H + 1, Y + 1, H - 2, H - 2, Fl_Color(UICOLOR_BUTTON));
				if (active_r())
					fl_color(labelcolor());
				else
					fl_color(fl_inactive(labelcolor()));
				int w1 = (H - 4) / 3;
				if (w1 < 1)
					w1 = 1;
				int x1 = X + (H - w1 - 1) / 2;
				int yy1 = Y + (H - 2 * w1 - 1) / 2;
				if (Fl::is_scheme("gtk+"))
				{
					fl_polygon(x1, yy1 + w1, x1 + w1, yy1 + 2 * w1, x1 + w1 - 1, yy1 + w1, x1 + w1, yy1);
					x1 += (W - H);
					fl_polygon(x1, yy1, x1 + 1, yy1 + w1, x1, yy1 + 2 * w1, x1 + w1, yy1 + w1);
				}
				else
				{
					fl_polygon(x1, yy1 + w1, x1 + w1, yy1 + 2 * w1, x1 + w1, yy1);
					x1 += (W - H);
					fl_polygon(x1, yy1, x1, yy1 + 2 * w1, x1 + w1, yy1 + w1);
				}
			}
		}
		else
		{
			// vertical
			fl_xyline(X, Y + W - 1, X + W);
			fl_xyline(X, Y + H - W, X + W);
			if (H < 3 * W)
			{
				//Fl_Slider::draw(X, Y, W, H);
				return;
			}
			//Fl_Slider::draw(X, Y + W, W, H - 2 * W);
			fl_rectf(X + 1, Y + W, W - 1, H - W - W, Fl_Color(UICOLOR_EMPTY));
			DrawSlider(X + 1, Y + W, W - 1, H - 2 * W);
			if (damage() & FL_DAMAGE_ALL)
			{
				//draw_box((pushed_ == 1) ? fl_down(slider()) : slider(), X, Y, W, W, selection_color());
				//draw_box((pushed_ == 2) ? fl_down(slider()) : slider(), X, Y + H - W, W, W, selection_color());
				fl_rectf(X + 1, Y + 1, W - 2, W - 2, Fl_Color(UICOLOR_BUTTON));
				fl_rectf(X + 1, Y  + H - W + 1, W - 2, W - 2, Fl_Color(UICOLOR_BUTTON));
				if (active_r())
					fl_color(labelcolor());
				else
					fl_color(fl_inactive(labelcolor()));
				int w1 = (W - 4) / 3;
				if (w1 < 1)
					w1 = 1;
				int x1 = X + (W - 2 * w1 - 1) / 2;
				int yy1 = Y + (W - w1 - 1) / 2;
				if (Fl::is_scheme("gtk+"))
				{
					fl_polygon(x1, yy1 + w1, x1 + w1, yy1 + w1 - 1, x1 + 2 * w1, yy1 + w1, x1 + w1, yy1);
					yy1 += H - W;
					fl_polygon(x1, yy1, x1 + w1, yy1 + 1, x1 + w1, yy1 + w1);
					fl_polygon(x1 + w1, yy1 + 1, x1 + 2 * w1, yy1, x1 + w1, yy1 + w1);
				}
				else
				{
					fl_polygon(x1, yy1 + w1, x1 + 2 * w1, yy1 + w1, x1 + w1, yy1);
					yy1 += H - W;
					fl_polygon(x1, yy1, x1 + w1, yy1 + w1, x1 + 2 * w1, yy1);
				}
			}
		}
	}

	void DrawSlider(int X, int Y, int W, int H)
	{
		//Fl_Slider.cxx > Fl_Slider::draw()
		double val = 0.5;
		if (minimum() != maximum())
		{
			val = (value() - minimum()) / (maximum() - minimum());
			if (val > 1.0)
				val = 1.0;
			else if (val < 0.0)
				val = 0.0;
		}
		int ww = (horizontal() ? W : H);
		int S = int(slider_size() * ww + .5);
		int T = (horizontal() ? H : W) / 2 + 1;
		if (S < T)
			S = T;
		int xx = int(val * (ww - S) + .5);
		if (horizontal())
		{
			fl_rect(X + xx - 1, Y - 1, S + 2, H + 2, Fl_Color(UICOLOR_BORDER));
			fl_rectf(X + xx, Y, S, H, Fl_Color(UICOLOR_BUTTON));
		}
		else
		{
			fl_rect(X - 1, Y + xx - 1, W + 2, S + 2, Fl_Color(UICOLOR_BORDER));
			fl_rectf(X, Y + xx, W, S, Fl_Color(UICOLOR_BUTTON));
		}
	}

};

#endif

