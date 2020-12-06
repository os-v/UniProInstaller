//
//  @file WizardPageConfigType.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDPAGECONFIGTYPE_H
#define WIZARDPAGECONFIGTYPE_H

#include "WizardPageBase.h"
#include <FL/Fl_Round_Button.H>
#include  "../UI/CustomRadioButton.h"
#include "../../Common/UtilsMisc.h"

class CWizardPageConfigType : public CWizardPageBase
{
public:

	CWizardPageConfigType(CWizardSheet *pParent) : CWizardPageBase(pParent)
	{
		m_pRadioTypical = 0;
		m_pRadioAdvanced = 0;
	}

	~CWizardPageConfigType()
	{
		//FLDestroy(m_pRadioTypical);
		//FLDestroy(m_pRadioAdvanced);
	}

	virtual bool CreatePage(int nPosX, int nPosY, int nWidth, int nHeight)
	{

		int nMargin = UIScale(64);
		int nRadioHeight = UIScale(30);

		m_pRadioTypical = new CCustomRadioButton(nPosX + nMargin, nPosY + nHeight / 3, nWidth - nMargin * 2, nRadioHeight, GSTR(Typical));
		m_pRadioTypical->type(FL_RADIO_BUTTON);
		m_pRadioTypical->set();

		m_pRadioAdvanced = new CCustomRadioButton(nPosX + nMargin, m_pRadioTypical->y() + m_pRadioTypical->h(), nWidth - nMargin * 2, nRadioHeight, GSTR(Advanced));
		m_pRadioAdvanced->type(FL_RADIO_BUTTON);

		OnSetActive(false);

		return true;
	}

	bool IsAdvanced()
	{
		return m_pRadioAdvanced->value();
	}

	virtual void OnSetActive(bool fActive)
	{

		FLSetVisible(m_pRadioTypical, fActive);
		FLSetVisible(m_pRadioAdvanced, fActive);

	}

	virtual const char *GetTitle()
	{
		return GSTR(ConfigurationType);
	}

	virtual const char *GetSubTitle()
	{
		return GSTR(PleaseSelectInstallationType);
	}

protected:

	CCustomRadioButton *m_pRadioTypical;
	CCustomRadioButton *m_pRadioAdvanced;

};

#endif

