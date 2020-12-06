//
//  @file WizardPageFinish.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDPAGEFINISH_H
#define WIZARDPAGEFINISH_H

#include "WizardPageBase.h"
#include "../PackageInfo.h"
#include <FL/Fl_Box.H>

class CWizardPageFinish : public CWizardPageBase
{
public:

	CWizardPageFinish(CWizardSheet *pParent, bool fUninstall = false) : CWizardPageBase(pParent)
	{

		m_fUninstall = fUninstall;

		m_pLabelText = 0;

	}

	~CWizardPageFinish()
	{
		//FLDestroy(m_pLabelText);
	}

	void OnFailed()
	{
		m_pLabelText->label("Unable to finish. Operation has been aborted. Please try again.");
	}

	virtual bool CreatePage(int nPosX, int nPosY, int nWidth, int nHeight)
	{

		EInstallType eType = m_fUninstall ? EInstallType::Uninstall : m_pParent->GetInstallType();

		CStdString sFinishPrompt = CStdStrFormat(GSTR(PleaseLogoffToStartUsing), m_pParent->GetProductName());
		if(eType == EInstallType::Install)
			m_sLabelText = CStdStrFormat(GSTR(InstallationHasBeenFinished), sFinishPrompt.c_str());
		else if(eType == EInstallType::Reinstall)
			m_sLabelText = CStdStrFormat(GSTR(HasBeenReinstalled), m_pParent->GetProductName(), sFinishPrompt.c_str());
		else if(eType == EInstallType::Update)
			m_sLabelText = CStdStrFormat(GSTR(HasBeenUpdated), m_pParent->GetProductName(), sFinishPrompt.c_str());
		else if(eType == EInstallType::Uninstall)
			m_sLabelText = CStdStrFormat(GSTR(HasBeenRemoved), m_pParent->GetProductName());

		m_pLabelText = new Fl_Box(nPosX + UIScale(25), nPosY + UIScale(75), nWidth - UIScale(50), nHeight - UIScale(150), m_sLabelText.c_str());
		m_pLabelText->box(FL_NO_BOX);
		//m_pLabelText->box(FL_DOWN_BOX);
		m_pLabelText->align(FL_ALIGN_LEFT | FL_ALIGN_TOP | FL_ALIGN_INSIDE | FL_ALIGN_WRAP);
		//m_pLabelText->end();

		OnSetActive(false);

		return true;
	}

	virtual void OnSetActive(bool fActive)
	{
		FLSetVisible(m_pLabelText, fActive);
	}

	virtual uint32_t GetButtons()
	{
		return EButtonCancel;
	}

	virtual const char *GetCancelButtonTitle()
	{
		return GSTR(Finish);
	}

	virtual const char *GetTitle()
	{
		return GSTR(Finished);
	}

	virtual const char *GetSubTitle()
	{
		return "";
	}

protected:

	bool m_fUninstall;

	Fl_Box *m_pLabelText;
	CStdString m_sLabelText;

};

#endif

