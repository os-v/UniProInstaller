//
//  @file WizardPageBase.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDPAGEBASE_H
#define WIZARDPAGEBASE_H

#include "../HelperDefs.h"
#include "../Language.h"

class CWizardSheet;

class CWizardPageBase
{
public:

	enum { EButtonBack = 0x01, EButtonNext = 0x02, EButtonCancel = 0x04, EButtonAdvanced = 0x08 };

	CWizardPageBase(CWizardSheet *pParent)
	{
		m_pParent = pParent;
	}

	virtual ~CWizardPageBase()
	{
	}

	virtual bool CreatePage(int nPosX, int nPosY, int nWidth, int nHeight) = 0;

	virtual uint32_t GetButtons()
	{
		return EButtonBack | EButtonNext | EButtonCancel;
	}

	virtual void OnSetActive(bool fActive) = 0;

	virtual void OnWizardNext()
	{
	}

	virtual const char *GetAdvancedButtonTitle()
	{
		return GSTR(Advanced);
	}

	virtual const char *GetBackButtonTitle()
	{
		return GSTR(Back);
	}

	virtual const char *GetNextButtonTitle()
	{
		return GSTR(Next);
	}

	virtual const char *GetCancelButtonTitle()
	{
		return GSTR(Cancel);
	}

	virtual const char *GetTitle()
	{
		return "Title";
	}

	virtual const char *GetSubTitle()
	{
		return "SubTitle";
	}

protected:

	CWizardSheet *m_pParent;

};

#endif

