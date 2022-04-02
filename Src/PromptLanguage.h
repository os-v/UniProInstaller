//
//  @file PromptLanguage.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 23.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef PROMPTLANGUAGE_H
#define PROMPTLANGUAGE_H

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Select_Browser.H>
#include <FL/Fl_PNG_Image.H>
#include "HelperDefs.h"
#include "Language.h"
#include "UI/CustomButton.h"
#include "../Common/UtilsMisc.h"

class CPromptLanguage
{
public:

	class CListBox : public Fl_Select_Browser
	{
	public:

		CListBox(int X, int Y, int W, int H, const char *L = 0) : Fl_Select_Browser(X, Y, W, H, L)
		{
		}

		int GetFullWidth() const
		{
			return Fl_Select_Browser::full_width();
		}

		int GetFullHeight() const
		{
			return Fl_Select_Browser::full_height();
		}

	};

	CPromptLanguage()
	{

		m_pWindow = 0;

		m_pList = 0;
		m_pButtonOK = 0;
		m_pButtonCancel = 0;

		m_nResult = -1;

	}

	~CPromptLanguage()
	{

		FLDestroy(m_pWindow);
		/*FLDestroy(m_pButtonOK);
		FLDestroy(m_pButtonCancel);
		FLDestroy(m_pWindow);*/

	}

	bool Create(Fl_PNG_Image *pIcon)
	{

		const char *lpTitle = GSTR(SelectLanguage);

		int nWindowWidth = UIScale(380);
		int nWindowHeight = UIScale(270);

		int nButtonWidth = UIScale(120);
		int nButtonHeight = UIScale(30);
		int nButtonSpaceX = UIScale(7);
		int nMargin = UIScale(10);
		int nPromptHeight = UIScale(215);

		int nScreenX, nScreenY, nScreenW, nScreenH;
		Fl::screen_work_area(nScreenX, nScreenY, nScreenW, nScreenH);

		m_pWindow = new Fl_Double_Window(nScreenX + nScreenW / 2 - nWindowWidth / 2, nScreenY + nScreenH / 2 - nWindowHeight / 2, nWindowWidth, nWindowHeight, lpTitle);
		m_pWindow->set_modal();
		m_pWindow->color(UICOLOR_BACKGROUND);
		if(pIcon)
			m_pWindow->icon(pIcon);

		m_pList = new CListBox(nMargin, nMargin, nWindowWidth - nMargin * 2, nPromptHeight, 0);
		m_pList->box(FL_BORDER_BOX);
		m_pList->callback(SOnLanguageList, this);
		m_pList->type(FL_HOLD_BROWSER);
		for(int iLanguage = 0; iLanguage < GetLanguageCount(); iLanguage++)
			m_pList->add(CStdStrFormat("%s (%s)", GetLanguageName(iLanguage, false), GetLanguageName(iLanguage, true)).c_str());
		m_pList->position(0);
		m_pList->value(1);
		m_pList->color(UICOLOR_BACKGROUND, UICOLOR_SELECTION);
		m_pList->resize(m_pList->x(), m_pList->y(), m_pList->w(), m_pList->GetFullHeight() + 10);
		m_pWindow->resize(m_pWindow->x(), m_pWindow->y(), m_pWindow->w(), nMargin + m_pList->h() + nButtonHeight + 2 * nMargin);

		int nButtons = 2;
		int nButtonX = nWindowWidth / 2 - (nButtons * nButtonWidth + (nButtons > 1 ? (nButtons - 1) * nButtonSpaceX : 0)) / 2;

		m_pButtonOK = new CCustomButton(nButtonX, m_pWindow->h() - nButtonHeight - nMargin, nButtonWidth, nButtonHeight, GSTR(OK), true);
		m_pButtonOK->callback(SOnButtonClicked, this);
		nButtonX += nButtonWidth + nMargin;

		m_pButtonCancel = new CCustomButton(nButtonX, m_pButtonOK->y(), nButtonWidth, nButtonHeight, GSTR(Cancel));
		m_pButtonCancel->callback(SOnButtonClicked, this);
		nButtonX += nButtonWidth + nMargin;

		m_pWindow->end();

		m_pWindow->show();

		return true;
	}

	int Exec()
	{
		for(; m_pWindow->shown(); )
			Fl::wait();
		return m_nResult;
	}

	static int Prompt(Fl_PNG_Image *pIcon)
	{
		CPromptLanguage dlg;
		dlg.Create(pIcon);
		return dlg.Exec();
	}

protected:

	int m_nResult;

	Fl_Double_Window *m_pWindow;

	CListBox *m_pList;
	CCustomButton *m_pButtonOK;
	CCustomButton *m_pButtonCancel;

	CASTWIDGETHANDLER(CPromptLanguage, OnButtonClicked);
	CASTWIDGETHANDLER(CPromptLanguage, OnLanguageList);

	void OnButtonClicked(Fl_Widget *pWidget)
	{
		if(pWidget == m_pButtonOK)
			m_nResult = m_pList->value();
		m_pWindow->hide();
	}

	void OnLanguageList(Fl_Widget *pWidget)
	{
		if(Fl::event_is_click() && Fl::event_clicks())
		{
			m_nResult = m_pList->value();
			m_pWindow->hide();
		}
	}

};

#endif

