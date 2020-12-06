//
//  @file WizardPageWelcome.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDPAGEWELCOME_H
#define WIZARDPAGEWELCOME_H

#include "WizardPageBase.h"
#include <FL/Fl_Box.H>

class CWizardPageWelcome : public CWizardPageBase
{
public:

	CWizardPageWelcome(CWizardSheet *pParent) : CWizardPageBase(pParent)
	{

		m_pLabelText = 0;

		m_sLabelText = "";

		m_fUninstallAllowed = false;

	}

	~CWizardPageWelcome()
	{
		//FLDestroy(m_pLabelText);
	}

	virtual bool CreatePage(int nPosX, int nPosY, int nWidth, int nHeight)
	{

		if (m_pParent->GetInstallType() == EInstallType::Update)
			m_sLabelText = CStdStrFormat(GSTR(InstallerWillUpdate), m_pParent->GetProductName(), m_pParent->GetInstallerVersion(), GSTR(ClickNextToContinue));
		else if (m_pParent->GetInstallType() == EInstallType::Reinstall)
			m_sLabelText = CStdStrFormat(GSTR(InstallerWillReinstall), m_pParent->GetProductName(), m_pParent->GetInstallerVersion(), GSTR(ClickNextToContinue));
		else
			m_sLabelText = CStdStrFormat(GSTR(InstallerWillInstall), m_pParent->GetProductName(), m_pParent->GetInstallerVersion(), GSTR(ClickNextToContinue));

		m_fUninstallAllowed = m_pParent->IsUninstallAllowed();
		if(m_fUninstallAllowed)
			m_sLabelText += CStdStrFormat((CStdString("\n\n\n") + GSTR(OrClickUninstallToContinue)).c_str(), m_pParent->GetProductName(), m_pParent->GetInstalledVersion());

		m_pLabelText = new Fl_Box(nPosX + UIScale(25), nPosY + UIScale(75), nWidth - UIScale(50), nHeight - UIScale(150), m_sLabelText.c_str());
		m_pLabelText->box(FL_NO_BOX);
		m_pLabelText->align(FL_ALIGN_LEFT | FL_ALIGN_TOP | FL_ALIGN_INSIDE | FL_ALIGN_WRAP);

		OnSetActive(false);

		return true;
	}

	virtual void OnSetActive(bool fActive)
	{
		FLSetVisible(m_pLabelText, fActive);
	}

	virtual uint32_t GetButtons()
	{

		return EButtonNext | EButtonCancel | (m_fUninstallAllowed ? EButtonAdvanced : 0);
	}

	virtual const char *GetAdvancedButtonTitle()
	{
		return GSTR(Uninstall);
	}

	virtual const char *GetTitle()
	{
		return GSTR(Welcome);
	}

	virtual const char *GetSubTitle()
	{
		return "";
	}

protected:

	Fl_Box *m_pLabelText;
	CStdString m_sLabelText;

	bool m_fUninstallAllowed;

};

#endif

