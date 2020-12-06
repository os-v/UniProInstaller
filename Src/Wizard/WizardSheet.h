//
//  @file WizardSheet.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDSHEET_H
#define WIZARDSHEET_H

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include "WizardPageBase.h"
#include "../InstallerBase.h"
#include "../../Common/UtilsMisc.h"

#if defined(WIN32)
	#include "../Platforms/Win32/InstallerSysImpl.h"
#elif defined(__APPLE__)
	#include "../Platforms/MacOS/InstallerSysImpl.h"
#elif defined(__FreeBSD__)
	#include "../Platforms/FreeBSD/InstallerSysImpl.h"
#else//if defined(__linux__)
	#include "../Platforms/Linux/InstallerSysImpl.h"
#endif

class CCustomButton;
class CCustomHyperlink;

class CWizardSheet
{
public:

	enum EPage { EPageInvalid = -1, EPageWelcome, EPageLicense, EPageConfigType, EPageConfig, EPageReady, EPageProgress, EPageFinish, EPageUReady, EPageUProgress, EPageUFinish, EPageCount  };

	CWizardSheet(CInstallerBase *pInstaller);
	~CWizardSheet();

	bool Create();
	void Destroy();
	int Exec();

	EInstallType GetInstallType() {
		return m_pInstaller->GetType();
	}

	bool IsUninstallAllowed() {
		return m_pInstaller->IsUninstallAllowed();
	}

	const char *GetInstalledVersion() {
		return m_pInstaller->GetInstalledVersion();
	}

	const char *GetInstallerVersion() {
		return m_pInstaller->GetInstallerVersion();
	}

	const char *GetProductName() {
		return m_pInstaller->GetProductName();
	}

	CPackageInfo *GetBundleInfo() {
		return m_pInstaller->GetBundleInfo();
	}

	void SetActivePage(EPage ePage, bool fSaveCur = false);

protected:

	bool m_fSuccess;

	CInstallerBase *m_pInstaller;

	EPage m_eActivePage;
	CStdArray<CWizardPageBase*> m_pPage;
	CStdArray<EPage> m_pPagePrev;

	Fl_Image *m_pImageBanner;
	Fl_Window *m_pWindow;
	Fl_Box *m_pBoxBanner;
	Fl_Group *m_pGroupCaption;
	Fl_Group *m_pGroupButtons;
	Fl_Group *m_pLabelCaption;
	Fl_Group *m_pLabelSubCaption;
	CCustomHyperlink *m_pLabelUPInstaller;
	CCustomButton *m_pButtonAdvanced;
	CCustomButton *m_pButtonBack;
	CCustomButton *m_pButtonNext;
	CCustomButton *m_pButtonCancel;

	int m_nProgressStep;
	int m_nProgressStepPrev;
	CStdString m_sProgressText;

	void CloseDialog(bool fSuccess);

	void OnButtonAdvanced(Fl_Widget *pWidget);
	void OnButtonBack(Fl_Widget *pWidget);
	void OnButtonNext(Fl_Widget *pWidget);
	void OnButtonCancel(Fl_Widget *pWidget);
	void OnWindowCallback(Fl_Widget *pWidget);

	CASTWIDGETHANDLER(CWizardSheet, OnButtonAdvanced);
	CASTWIDGETHANDLER(CWizardSheet, OnButtonBack);
	CASTWIDGETHANDLER(CWizardSheet, OnButtonNext);
	CASTWIDGETHANDLER(CWizardSheet, OnButtonCancel);
	CASTWIDGETHANDLER(CWizardSheet, OnWindowCallback);

	CASTSIMPLEHANDLER(CWizardSheet, OnProgressUpdate);

	void OnProgressUpdate();
	bool OnProgressUpdate(int nStep, const CStdString &sText);
	static bool SOnProgressUpdate(int nStep, const CStdString &sText, void *pContext) {
		return ((CWizardSheet*)pContext)->OnProgressUpdate(nStep, sText);
	}

};

#endif
