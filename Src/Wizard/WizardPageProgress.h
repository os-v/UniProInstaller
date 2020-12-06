//
//  @file WizardPageProgress.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDPAGEPROGRESS_H
#define WIZARDPAGEPROGRESS_H

#include "WizardPageBase.h"
#include <FL/Fl_Box.H>
#include <FL/Fl_Progress.H>

class CWizardPageProgress : public CWizardPageBase
{
public:

	CWizardPageProgress(CWizardSheet *pParent, bool fUninstall = false) : CWizardPageBase(pParent)
	{

		m_fUninstall = fUninstall;

		m_pLabelStatus = 0;
		m_pProgress = 0;

	}

	~CWizardPageProgress()
	{
		//FLDestroy(m_pLabelStatus);
		//FLDestroy(m_pProgress);
	}

	virtual bool CreatePage(int nPosX, int nPosY, int nWidth, int nHeight)
	{

		int nMargin = UIScale(15);
		int nLabelHeight = UIScale(24);
		int nProgressHeight = UIScale(12);

		m_pLabelStatus = new Fl_Box(nPosX + nMargin, nPosY + nHeight / 2 - nLabelHeight, nWidth - nMargin * 2, nLabelHeight, GSTR(Preparing));
		m_pLabelStatus->box(FL_NO_BOX);
		m_pLabelStatus->align(FL_ALIGN_LEFT | /*FL_ALIGN_TOP | */FL_ALIGN_INSIDE | FL_ALIGN_WRAP);

		m_pProgress = new Fl_Progress(nPosX + nMargin, nPosY + nHeight / 2, nWidth - nMargin * 2, nProgressHeight);
			m_pProgress->box(FL_PLASTIC_DOWN_BOX);
		m_pProgress->minimum(0);
		m_pProgress->maximum(100);
		m_pProgress->value(0);
		m_pProgress->color(UICOLOR_BACKGROUND, UICOLOR_PROGRESS);

		OnSetActive(false);

		return true;
	}

	virtual void OnSetActive(bool fActive)
	{
		FLSetVisible(m_pLabelStatus, fActive);
		FLSetVisible(m_pProgress, fActive);
	}

	virtual uint32_t GetButtons()
	{
		return 0;
	}

	virtual const char *GetTitle()
	{
		if(m_pParent->GetInstallType() == EInstallType::Install)
			return GSTR(Installation);
		else if(m_pParent->GetInstallType() == EInstallType::Reinstall)
			return GSTR(Reinstallation);
		else if(m_pParent->GetInstallType() == EInstallType::Update)
			return GSTR(Updating);
		else if(m_pParent->GetInstallType() == EInstallType::Uninstall)
			return GSTR(Uninstalling);
		return "";
	}

	virtual const char *GetSubTitle()
	{
		if(m_pParent->GetInstallType() == EInstallType::Install)
			m_sSubTitle = CStdStrFormat(GSTR(PleaseWaitInstalling), m_pParent->GetProductName());
		else if(m_pParent->GetInstallType() == EInstallType::Reinstall)
			m_sSubTitle = CStdStrFormat(GSTR(PleaseWaitReinstalling), m_pParent->GetProductName());
		else if(m_pParent->GetInstallType() == EInstallType::Update)
			m_sSubTitle = CStdStrFormat(GSTR(PleaseWaitUpdating), m_pParent->GetProductName());
		else if(m_pParent->GetInstallType() == EInstallType::Uninstall)
			m_sSubTitle = CStdStrFormat(GSTR(PleaseWaitRemoving), m_pParent->GetProductName());
		return m_sSubTitle.c_str();
	}

	void UpdateProgress(int nStep, const CStdString &sText)
	{
		m_sTextStatus = sText;
		Fl::lock();
		m_pLabelStatus->label(m_sTextStatus.c_str());
		m_pProgress->value((float)nStep);
		Fl::unlock();
		Fl::awake();
		//Fl::awake(m_pLabelStatus);
		//Fl::awake(m_pProgress);
	}

protected:

	bool m_fUninstall;

	Fl_Box *m_pLabelStatus;
	Fl_Progress *m_pProgress;

	CStdString m_sTextStatus;
	CStdString m_sSubTitle;

};

#endif

