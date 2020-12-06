//
//  @file WizardPageConfig.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDPAGECONFIG_H
#define WIZARDPAGECONFIG_H

#include "WizardPageBase.h"
#include "../UI/CustomInputText.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>

class CWizardPageConfig : public CWizardPageBase
{
public:

	CWizardPageConfig(CWizardSheet *pParent) : CWizardPageBase(pParent)
	{
		m_pLabelName = 0;
		m_pEditName = 0;
	}

	~CWizardPageConfig()
	{
		//FLDestroy(m_pLabelName);
		//FLDestroy(m_pEditName);
	}

	virtual bool CreatePage(int nPosX, int nPosY, int nWidth, int nHeight)
	{

		int nSpacing = UIScale(2);
		int nLabelHeight = UIScale(22);
		int nEditHeight = UIScale(27);

		m_pLabelName = new Fl_Box(nPosX + UIScale(25), nPosY + UIScale(75), nWidth - UIScale(50), nLabelHeight, GSTR(CompanyURL));
		m_pLabelName->box(FL_NO_BOX);
		m_pLabelName->align(FL_ALIGN_LEFT | FL_ALIGN_TOP | FL_ALIGN_INSIDE | FL_ALIGN_WRAP);

		m_pEditName = new CCustomInputText(m_pLabelName->x(), m_pLabelName->y() + m_pLabelName->h() + nSpacing, m_pLabelName->w(), nEditHeight);
		m_pEditName->value("SomeValue");

		OnSetActive(false);

		return true;
	}

	virtual void OnSetActive(bool fActive)
	{
		FLSetVisible(m_pLabelName, fActive);
		FLSetVisible(m_pEditName, fActive);
		//if(fActive)
			//m_pEditName->set_active();
	}

	virtual const char *GetTitle()
	{
		return GSTR(AdvancedConfigMode);
	}

	virtual const char *GetSubTitle()
	{
		return GSTR(PleaseConfigureSettings);
	}

protected:

	Fl_Box *m_pLabelName;
	CCustomInputText *m_pEditName;

};

#endif

