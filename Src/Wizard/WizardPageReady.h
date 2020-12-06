//
//  @file WizardPageReady.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDPAGEREADY_H
#define WIZARDPAGEREADY_H

#include "WizardPageBase.h"
#include <FL/Fl_Box.H>

class CWizardPageReady : public CWizardPageBase
{
public:

	CWizardPageReady(CWizardSheet *pParent, bool fUninstall = false) : CWizardPageBase(pParent)
	{

		m_fUninstall = fUninstall;

		m_pLabelText = 0;

	}

	~CWizardPageReady()
	{
		//FLDestroy(m_pLabelText);
	}

	virtual bool CreatePage(int nPosX, int nPosY, int nWidth, int nHeight)
	{

		EInstallType eType = m_fUninstall ? EInstallType::Uninstall : m_pParent->GetInstallType();
		if(eType == EInstallType::Install)
			m_sLabelText = CStdStrFormat(GSTR(InstallerReadyToInstall), m_pParent->GetProductName(), GSTR(ClickContinueToStart));
		else if(eType == EInstallType::Reinstall)
			m_sLabelText = CStdStrFormat(GSTR(InstallerReadyToReinstall), m_pParent->GetProductName(), GSTR(ClickContinueToStart));
		else if(eType == EInstallType::Update)
			m_sLabelText = CStdStrFormat(GSTR(InstallerReadyToUpdate), m_pParent->GetProductName(), GSTR(ClickContinueToStart));
		else if(eType == EInstallType::Uninstall)
			m_sLabelText = CStdStrFormat(GSTR(InstallerReadyToUninstall), m_pParent->GetProductName(), GSTR(ClickContinueToStart));
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
		if(m_pParent->GetInstallType() == EInstallType::Uninstall)
			return EButtonNext | EButtonCancel;
		return CWizardPageBase::GetButtons();
	}

	virtual const char *GetNextButtonTitle()
	{
		return GSTR(Continue);
	}

	virtual const char *GetTitle()
	{
		if(m_pParent->GetInstallType() == EInstallType::Install)
			m_sTitleText = CStdStrFormat(GSTR(ReadyToStartInstallation), m_pParent->GetProductName());
		else if(m_pParent->GetInstallType() == EInstallType::Reinstall)
			m_sTitleText = CStdStrFormat(GSTR(ReadyToReinstall), m_pParent->GetProductName());
		else if(m_pParent->GetInstallType() == EInstallType::Update)
			m_sTitleText = CStdStrFormat(GSTR(ReadyToUpdate), m_pParent->GetProductName());
		else if(m_pParent->GetInstallType() == EInstallType::Uninstall)
			m_sTitleText = CStdStrFormat(GSTR(ReadyToRemove), m_pParent->GetProductName());
		return m_sTitleText.c_str();
	}

	virtual const char *GetSubTitle()
	{
		return "";
	}

protected:

	bool m_fUninstall;

	Fl_Box *m_pLabelText;
	CStdString m_sLabelText;
	CStdString m_sTitleText;

};

#endif

