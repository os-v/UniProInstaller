//
//  @file CustomHyperlink.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 27.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef CUSTOMHYPERLINK_H
#define CUSTOMHYPERLINK_H

#include <FL/fl_draw.H>
#include "../HelperDefs.h"
#include <stdio.h>

class CCustomHyperlink : public Fl_Widget
{
	DefLogScope(CCustomHyperlink)
public:

	CCustomHyperlink(int X, int Y, int W, int H, const char *lpText, const char *lpURL) : Fl_Widget(X, Y, W, H, lpText)
	{
		m_sURL = lpURL;
		m_fMouseEnter = false;
		SetColor(Fl_Color(UICOLOR_BACKGROUND), Fl_Color(UICOLOR_HYPERLINK), Fl_Color(UICOLOR_HYPERLINKHOWER));
		labelfont(FL_HELVETICA);
		labelsize(FL_NORMAL_SIZE);
	}

	void SetColor(Fl_Color nBackColor, Fl_Color nLabelColor, Fl_Color nSelectedColor)
	{
		color(nBackColor);
		labelcolor(nLabelColor);
		selection_color(nSelectedColor);
	}

	static int DefaultHeight()
	{
		fl_font(FL_HELVETICA, FL_NORMAL_SIZE);
		return fl_height();
	}

	static int DefaultWidth(const char *lpText)
	{
		fl_font(FL_HELVETICA, FL_NORMAL_SIZE);
		return (int)fl_width(lpText);
	}

private:

	bool m_fMouseEnter;

	CStdString m_sURL;

	virtual int handle(int event)
	{
		if(event == FL_ENTER)
		{
			m_fMouseEnter = true;
			fl_cursor(FL_CURSOR_HAND);
			redraw();
			return true;
		}
		else if(event == FL_LEAVE)
		{
			m_fMouseEnter = false;
			fl_cursor(FL_CURSOR_DEFAULT);
			redraw();
			return true;
		}
		else if(event == FL_RELEASE)//FL_PUSH)
		{
			//fl_open_uri(m_sURL.c_str(), 0, 0);
			CInstallerSysImpl::OpenURL(m_sURL.c_str());
			return true;
		}
		return false;//Fl_Widget::handle(event);
	}

	virtual void draw()
	{

		int nPosX = x(), nPosY = y(), nWidth = w(), nHeight = h();

		int nTextWidth = (int)fl_width(label());
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
		fl_color(m_fMouseEnter ? selection_color() : labelcolor());

		fl_font(labelfont(), labelsize());
		fl_draw(label(), nPosX, nPosY, nTextWidth, nTextHeight, 0);//align());

	}

};

#endif

