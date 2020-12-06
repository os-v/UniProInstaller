//
//  @file CustomLabel.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 27.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <FL/fl_draw.H>
#include "../HelperDefs.h"

class CCustomLabel : public Fl_Widget
{
public:

	CCustomLabel(int X, int Y, int W, int H, const char *L = 0) : Fl_Widget(X, Y, W, H, L)
	{
		SetColor(Fl_Color(UICOLOR_BACKGROUND), Fl_Color(UICOLOR_TEXT));
	}

	void SetColor(Fl_Color nBackColor, Fl_Color nLabelColor)
	{
		color(nBackColor);
		labelcolor(nLabelColor);
	}

private:

	virtual void draw()
	{

		int nPosX = x(), nPosY = y(), nWidth = w(), nHeight = h();

		int nTextWidth = fl_width(label());
		int nTextHeight = fl_height();

		int nAlign = align();
		if(nAlign & FL_ALIGN_LEFT)
			nPosX = nPosX;
		else if(nAlign & FL_ALIGN_RIGHT)
			nPosX = nPosX + nWidth - nTextWidth;
		else //if(nAlign & FL_ALIGN_CENTER)
			nPosX = nPosX + nWidth / 2 - nTextWidth / 2;

		if(nAlign & FL_ALIGN_TOP)
			nPosY = nPosY;
		else if(nAlign & FL_ALIGN_BOTTOM)
			nPosY = nPosY + nHeight - nTextHeight;
		else if(nAlign & FL_ALIGN_INSIDE)
			nPosY = nPosY + nHeight / 2 - nTextHeight / 2;

		fl_rectf(nPosX, nPosY, nTextWidth, nTextHeight, color());
		fl_color(labelcolor());
		fl_draw(label(), nPosX, nPosY, nTextWidth, nTextHeight, 0);//align());

	}

};

#endif

