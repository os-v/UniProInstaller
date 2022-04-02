//
//  @file PromptDialog.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 23.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include "UI/CustomButton.h"
#include "HelperDefs.h"

class CPromptDialog
{
public:

	CPromptDialog()
	{

		m_pWindow = 0;

		m_pLabelText = 0;
		m_pButtonOK = 0;
		m_pButtonCancel = 0;

		m_fResult = false;

	}

	~CPromptDialog()
	{

		FLDestroy(m_pWindow);
		/*FLDestroy(m_pLabelText);
		FLDestroy(m_pButtonOK);
		FLDestroy(m_pButtonCancel);
		FLDestroy(m_pWindow);*/

	}

	bool Create(const char *lpTitle, const char *lpPrompt, const char *lpButtonOK = 0, const char *lpButtonCancel = 0)
	{

		int nWindowWidth = UIScale(320);
		int nWindowHeight = UIScale(120);

		int nButtonWidth = UIScale(80);
		int nButtonHeight = UIScale(30);
		int nButtonSpaceX = UIScale(7);
		int nMargin = UIScale(10);
		int nPromptHeight = UIScale(64);

		int nScreenX, nScreenY, nScreenW, nScreenH;
		Fl::screen_xywh(nScreenX, nScreenY, nScreenW, nScreenH);
		Fl::screen_work_area(nScreenX, nScreenY, nScreenW, nScreenH);

		m_pWindow = new Fl_Double_Window(nScreenX + nScreenW / 2 - nWindowWidth / 2, nScreenY + nScreenH / 2 - nWindowHeight / 2, nWindowWidth, nWindowHeight, lpTitle);
		m_pWindow->set_modal();
		m_pWindow->color(UICOLOR_BACKGROUND);

		m_pLabelText = new Fl_Group(nMargin, nMargin, nWindowWidth - nMargin * 2, nPromptHeight, lpPrompt);
		m_pLabelText->align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE | FL_ALIGN_WRAP);
		m_pLabelText->box(FL_NO_BOX);
		m_pLabelText->end();

		int nButtons = (lpButtonOK ? 1 : 0) + (lpButtonCancel ? 1 : 0);
		int nButtonX = nWindowWidth / 2 - (nButtons * nButtonWidth + (nButtons > 1 ? (nButtons - 1) * nButtonSpaceX : 0)) / 2;

		if(lpButtonOK)
		{
			m_pButtonOK = new CCustomButton(nButtonX, nWindowHeight - nButtonHeight - nMargin, nButtonWidth, nButtonHeight, lpButtonOK, true);
			m_pButtonOK->callback(SOnButtonClicked, this);
			nButtonX += nButtonWidth + nMargin;
		}

		if(lpButtonCancel)
		{
			m_pButtonCancel = new CCustomButton(nButtonX, m_pButtonOK->y(), nButtonWidth, nButtonHeight, lpButtonCancel);
			m_pButtonCancel->callback(SOnButtonClicked, this);
			nButtonX += nButtonWidth + nMargin;
		}

		m_pWindow->end();

		m_pWindow->show();

		return true;
	}

	bool Exec()
	{
		for(; m_pWindow->shown(); )
			Fl::wait();
		return m_fResult;
	}

	static bool MessageBox(const char *lpTitle, const char *lpPrompt, const char *lpButtonOK = 0, const char *lpButtonCancel = 0)
	{
		CPromptDialog dlg;
		dlg.Create(lpTitle, lpPrompt, lpButtonOK, lpButtonCancel);
		return dlg.Exec();
	}

protected:

	bool m_fResult;

	Fl_Double_Window *m_pWindow;

	Fl_Group *m_pLabelText;
	CCustomButton *m_pButtonOK;
	CCustomButton *m_pButtonCancel;

	CASTWIDGETHANDLER(CPromptDialog, OnButtonClicked);

	void OnButtonClicked(Fl_Widget *pWidget)
	{
		if(pWidget == m_pButtonOK)
			m_fResult = true;
		m_pWindow->hide();
	}

};

#endif

