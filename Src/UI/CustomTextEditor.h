//
//  @file CustomTextEditor.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 27.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef CUSTOMTEXTEDITOR_H
#define CUSTOMTEXTEDITOR_H

#include <FL/Fl_Text_Editor.H>
#include <FL/fl_draw.H>
#include "CustomScrollBar.h"
#include  "../HelperDefs.h"

class CCustomTextEditor : public Fl_Text_Editor
{
public:

	CCustomTextEditor(int X, int Y, int W, int H, const char* l = 0) : Fl_Text_Editor(X, Y, W, H, l)
	{

		m_fReadOnly = false;

		begin();
		CCustomScrollbar *pVert = new CCustomScrollbar(0, 0, 1, 1);
		CCustomScrollbar *pHorz = new CCustomScrollbar(0, 0, 1, 1);
		SetScrollBars(pVert, pHorz);
		end();

	}

	void SetScrollBars(Fl_Scrollbar *pVert, Fl_Scrollbar *pHorz)
	{
		Fl_Scrollbar *pVertPrev = mVScrollBar;
		Fl_Scrollbar *pHorzPrev = mHScrollBar;
		pVert->resize(mVScrollBar->x(), mVScrollBar->y(), mVScrollBar->w(), mVScrollBar->h());
		pVert->callback((Fl_Callback*)v_scrollbar_cb, this);
		pHorz->resize(mHScrollBar->x(), mHScrollBar->y(), mHScrollBar->w(), mHScrollBar->h());
		pHorz->callback((Fl_Callback*)h_scrollbar_cb, this);
		pHorz->type(FL_HORIZONTAL);
		mVScrollBar = pVert;
		mHScrollBar = pHorz;
		scrollbar_width(Fl::scrollbar_size());
		scrollbar_align(FL_ALIGN_BOTTOM_RIGHT);
		delete pVertPrev;
		delete pHorzPrev;
	}

	Fl_Scrollbar *HScrollBar() {
		return mHScrollBar;
	}

	Fl_Scrollbar *VScrollBar() {
		return mVScrollBar;
	}

	void SetReadOnly(bool fReadOnly) {
		m_fReadOnly = fReadOnly;
	}

private:

	bool m_fReadOnly;

	virtual int handle(int e)
	{
		if(m_fReadOnly)
			return Fl_Text_Display::handle(e);
		return Fl_Text_Editor::handle(e);
	}

};

#endif

