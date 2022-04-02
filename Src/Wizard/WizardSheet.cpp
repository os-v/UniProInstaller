//
//  @file WizardSheet.cpp
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#include "WizardSheet.h"
#include "WizardPageWelcome.h"
#include "WizardPageLicense.h"
#include "WizardPageConfigType.h"
#include "WizardPageConfig.h"
#include "WizardPageReady.h"
#include "WizardPageProgress.h"
#include "WizardPageFinish.h"
#include "../InstallerBase.h"
#include "../UI/CustomButton.h"
#include "../UI/CustomHyperlink.h"
#include "../../Common/UtilsMisc.h"
#include "../PromptDialog.h"
#include "../PromptLanguage.h"
#include "../Resources.h"
#include <FL/fl_ask.H>
#include <FL/Fl_PNG_Image.H>

DefLogScope(CWizardSheet)

CWizardSheet::CWizardSheet(CInstallerBase *pInstaller)
{

	m_pInstaller = pInstaller;

	m_fSuccess = false;

	m_eActivePage = EPageInvalid;

	bool fInstall = GetInstallType() != EInstallType::Uninstall;

	m_pPage.push_back(fInstall ? new CWizardPageWelcome(this) : 0);
	m_pPage.push_back(fInstall ? new CWizardPageLicense(this) : 0);
	m_pPage.push_back(fInstall ? new CWizardPageConfigType(this) : 0);
	m_pPage.push_back(fInstall ? new CWizardPageConfig(this) : 0);
	m_pPage.push_back(fInstall ? new CWizardPageReady(this) : 0);
	m_pPage.push_back(fInstall ? new CWizardPageProgress(this) : 0);
	m_pPage.push_back(fInstall ? new CWizardPageFinish(this) : 0);
	m_pPage.push_back(/*fInstall ? 0 : */new CWizardPageReady(this, true));
	m_pPage.push_back(/*fInstall ? 0 : */new CWizardPageProgress(this, true));
	m_pPage.push_back(/*fInstall ? 0 : */new CWizardPageFinish(this, true));

	m_pImageBanner = 0;

	m_pWindow = 0;
	m_pBoxBanner = 0;
	m_pGroupCaption = 0;
	m_pGroupButtons = 0;
	m_pLabelCaption = 0;
	m_pLabelSubCaption = 0;
	m_pLabelUPInstaller = 0;
	m_pButtonAdvanced = 0;
	m_pButtonBack = 0;
	m_pButtonNext = 0;
	m_pButtonCancel = 0;

}

CWizardSheet::~CWizardSheet()
{

	for(uint32_t iPage = 0; iPage < m_pPage.size(); iPage++)
		delete m_pPage[iPage];

	m_pPage.clear();

	//FLDestroy(m_pImageBanner);

	FLDestroy(m_pWindow);
	/*FLDestroy(m_pImageBanner);
	FLDestroy(m_pGroupCaption);
	FLDestroy(m_pGroupButtons);
	FLDestroy(m_pLabelCaption);
	FLDestroy(m_pLabelSubCaption);
	FLDestroy(m_pLabelUPInstaller);
	FLDestroy(m_pButtonAdvanced);
	FLDestroy(m_pButtonBack);
	FLDestroy(m_pButtonNext);
	FLDestroy(m_pButtonCancel);*/

}

bool CWizardSheet::Create()
{

	UIPrepare();

	fl_font(FL_HELVETICA, FL_NORMAL_SIZE);

	Fl_PNG_Image pIcon("Icon.png", ResIconData(), ResIconSize());
	m_pImageBanner = new Fl_PNG_Image("Banner.png", ResBannerData(), ResBannerSize());

	if(!SetLanguage(m_pInstaller->GetBundleInfo()->Language.c_str()))
	{
		int nLanguage = CPromptLanguage::Prompt(&pIcon);
		if(nLanguage == -1)
			return false;
		nLanguage--;
		SetLanguage(nLanguage);
		m_pInstaller->GetBundleInfo()->Language = GetLanguageName(nLanguage, false);
	}

	CStdString sTitle = CStdStrFormat("%s %s", GetProductName(), GetInstallType() != EInstallType::Uninstall ? GSTR(Install) : GSTR(Uninstall));

	int nWindowWidth = UIScale(640);
	int nWindowHeight = UIScale(480);
	int nCaptionPanelHeight = UIScale(80);
	int nButtonsPanelHeight = UIScale(70);
	int nButtonWidth = UIScale(120);
	int nButtonHeight = UIScale(30);
	int nButtonSpaceX = UIScale(7);
	int nButtonMarginLeft = UIScale(15);
	int nButtonMarginRight = UIScale(15);
	int nCaptionHeight = UIScale(24);
	int nCaptionMargin = UIScale(32);
	int nSubCaptionHeight = UIScale(24);
	int nSubCaptionMargin = UIScale(64);

	int nScreenX, nScreenY, nScreenW, nScreenH;
	//Fl::screen_xywh(nScreenX, nScreenY, nScreenW, nScreenH);
	Fl::screen_work_area(nScreenX, nScreenY, nScreenW, nScreenH);

	m_pWindow = new Fl_Window(nScreenX + nScreenW / 2 - nWindowWidth / 2, nScreenY + nScreenH / 2 - nWindowHeight / 2, nWindowWidth, nWindowHeight, sTitle.c_str());

	m_pBoxBanner = new Fl_Box(m_pWindow->w() - m_pImageBanner->w() - nButtonSpaceX - nButtonMarginRight, nCaptionPanelHeight / 2 - m_pImageBanner->h() / 2, m_pImageBanner->w(), m_pImageBanner->h());
	m_pBoxBanner->image(m_pImageBanner);

	m_pGroupCaption = new Fl_Group(UIScale(4), nCaptionPanelHeight, nWindowWidth - UIScale(8), UIScale(2));
	m_pGroupCaption->box(FL_THIN_DOWN_FRAME);
	m_pGroupCaption->end();

	m_pGroupButtons = new Fl_Group(UIScale(4), nWindowHeight - nButtonsPanelHeight, nWindowWidth - UIScale(8), UIScale(2));
	m_pGroupButtons->box(FL_THIN_DOWN_FRAME);
	m_pGroupButtons->end();

	m_pLabelCaption = new Fl_Group(nCaptionMargin / 2, nCaptionPanelHeight / 2 - nCaptionHeight, nWindowWidth - nCaptionMargin, nCaptionHeight, "Caption");
	m_pLabelCaption->labelfont(FL_BOLD);
	m_pLabelCaption->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
	m_pLabelCaption->box(FL_NO_BOX);
	m_pLabelCaption->end();

	m_pLabelSubCaption = new Fl_Group(nSubCaptionMargin / 2, nCaptionPanelHeight / 2, nWindowWidth - nSubCaptionMargin, nSubCaptionHeight, "Sub Caption");
	m_pLabelSubCaption->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
	m_pLabelSubCaption->box(FL_NO_BOX);
	m_pLabelSubCaption->end();

	const char *lpHyperlinkURL = "https://github.com/os-v";
	const char *lpHyperlinkText = " made with UniProInstaller [ github.com/os-v ] ";
	int nHyperlinkHeight = CCustomHyperlink::DefaultHeight();
	int nHyperlinkWidth = CCustomHyperlink::DefaultWidth(lpHyperlinkText);
	m_pLabelUPInstaller = new CCustomHyperlink(m_pGroupCaption->w() - nHyperlinkWidth - nButtonSpaceX - nButtonMarginRight, nWindowHeight - nButtonsPanelHeight - nHyperlinkHeight / 2, nHyperlinkWidth, nHyperlinkHeight, lpHyperlinkText, lpHyperlinkURL);
	m_pLabelUPInstaller->align(FL_ALIGN_INSIDE);// | FL_ALIGN_RIGHT);

	m_pButtonAdvanced = new CCustomButton(nButtonMarginLeft, PTCOR(nWindowHeight, nButtonsPanelHeight, nButtonHeight), nButtonWidth, nButtonHeight, "&Advanced");
	m_pButtonAdvanced->callback(SOnButtonAdvanced, this);
	m_pButtonBack = new CCustomButton(nWindowWidth - (nButtonWidth + nButtonSpaceX) * 3 - nButtonMarginRight, PTCOR(nWindowHeight, nButtonsPanelHeight, nButtonHeight), nButtonWidth, nButtonHeight, "&Prev");
	m_pButtonBack->callback(SOnButtonBack, this);
	m_pButtonNext = new CCustomButton(m_pButtonBack->x() + nButtonWidth + nButtonSpaceX, m_pButtonBack->y(), nButtonWidth, nButtonHeight, "&Next", true);
	m_pButtonNext->callback(SOnButtonNext, this);
	m_pButtonCancel = new CCustomButton(m_pButtonNext->x() + nButtonWidth + nButtonSpaceX, m_pButtonBack->y(), nButtonWidth, nButtonHeight, "&Cancel");
	m_pButtonCancel->callback(SOnButtonCancel, this);

	for(int iPage = 0; iPage < m_pPage.size(); iPage++)
	{
		if(m_pPage[iPage])
			m_pPage[iPage]->CreatePage(m_pGroupCaption->x(), m_pGroupCaption->y() + m_pGroupCaption->h() + UIScale(4), m_pGroupCaption->w(), m_pGroupButtons->y() - m_pGroupCaption->y() - UIScale(10));
	}

	m_pWindow->icon(&pIcon);

	m_pWindow->callback(SOnWindowCallback, this);
	m_pWindow->color(UICOLOR_BACKGROUND);
	m_pWindow->end();
	//m_pWindow->resizable(m_pWindow);
	m_pWindow->show();

	SetActivePage(GetInstallType() != EInstallType::Uninstall ? EPageWelcome : EPageUReady);

	m_pButtonNext->take_focus();

	return true;
}

void CWizardSheet::Destroy()
{

}

int CWizardSheet::Exec()
{
	int nReturn = Fl::run();
	return nReturn;
}

void CWizardSheet::SetActivePage(EPage ePage, bool fSaveCur)
{

	LogMessage("CWizardSheet::SetActivePage(%d)", ePage);

	if(fSaveCur)
		m_pPagePrev.push_back(m_eActivePage);

	if (m_eActivePage == EPageProgress || m_eActivePage == EPageUProgress)
		Fl::remove_idle(SOnProgressUpdate);

	if (m_eActivePage != EPageInvalid)
		m_pPage[m_eActivePage]->OnSetActive(false);

	m_eActivePage = ePage;

	CWizardPageBase *pPage = m_pPage[m_eActivePage];

	pPage->OnSetActive(true);

	uint32_t eButtons = pPage->GetButtons();
	FLSetVisible(m_pButtonAdvanced, eButtons & CWizardPageBase::EButtonAdvanced);
	FLSetVisible(m_pButtonBack, eButtons & CWizardPageBase::EButtonBack);
	FLSetVisible(m_pButtonNext, eButtons & CWizardPageBase::EButtonNext);
	FLSetVisible(m_pButtonCancel, eButtons & CWizardPageBase::EButtonCancel);

	m_pLabelCaption->label(pPage->GetTitle());
	m_pLabelSubCaption->label(pPage->GetSubTitle());

	m_pButtonAdvanced->label(pPage->GetAdvancedButtonTitle());
	m_pButtonBack->label(pPage->GetBackButtonTitle());
	m_pButtonNext->label(pPage->GetNextButtonTitle());
	m_pButtonCancel->label(pPage->GetCancelButtonTitle());

	if (m_eActivePage == EPageProgress || m_eActivePage == EPageUProgress)
	{
		m_nProgressStep = m_nProgressStepPrev = -1;
		Fl::add_idle(SOnProgressUpdate, this);
		m_pInstaller->Create(SOnProgressUpdate, this);
		m_pInstaller->Start(true);
	}
	else if(m_eActivePage == EPageFinish || m_eActivePage == EPageUFinish)
	{
		m_pButtonCancel->SetDefault(true);
	}

}

void CWizardSheet::CloseDialog(bool fSuccess)
{

	m_pWindow->hide();

}

void CWizardSheet::OnButtonAdvanced(Fl_Widget *pWidget)
{

	if(m_eActivePage == EPageWelcome)
	{
		m_pInstaller->SetUninstallType();
		SetActivePage(EPageUReady);
	}

}

void CWizardSheet::OnButtonBack(Fl_Widget *pWidget)
{

	EPage ePrevPage = m_pPagePrev.back();
	m_pPagePrev.pop_back();

	SetActivePage(ePrevPage);

}

void CWizardSheet::OnButtonNext(Fl_Widget *pWidget)
{

	LogMessage("CWizardSheet::OnButtonNext(%d)", m_eActivePage);

	if (m_eActivePage == EPageWelcome && (GetInstallType() == EInstallType::Update || GetInstallType() == EInstallType::Reinstall))
	{
		SetActivePage(EPageReady, true);
		return;
	}

	if (m_eActivePage == EPageFinish || m_eActivePage == EPageUFinish)
	{
		CloseDialog(m_fSuccess);
		return;
	}

	//if (m_eActivePage == EPageProgress || m_eActivePage == EPageUProgress)
		//return;

	if (m_eActivePage == EPageConfigType)
		SetActivePage(((CWizardPageConfigType*)m_pPage[EPageConfigType])->IsAdvanced() ? EPageConfig : EPageReady, true);
	else if(m_eActivePage == EPageLicense && !GetBundleInfo()->InstallExtraConfig)
		SetActivePage(EPageReady, true);
	else
		SetActivePage((EPage)((int)m_eActivePage + 1), true);

}

void CWizardSheet::OnButtonCancel(Fl_Widget *pWidget)
{

	if (m_eActivePage == EPageFinish || m_eActivePage == EPageUFinish)
	{
		CloseDialog(m_fSuccess);
		return;
	}

	if (m_eActivePage == EPageProgress || m_eActivePage == EPageUProgress)
		return;

	if(CPromptDialog::MessageBox(GSTR(CancelConfirmation), GSTR(ReallyAbortInstallation), GSTR(Yes), GSTR(No)))
		CloseDialog(false);

}

void CWizardSheet::OnWindowCallback(Fl_Widget *pWidget)
{

	OnButtonCancel(pWidget);

}

void CWizardSheet::OnProgressUpdate()
{

	if(m_nProgressStep == m_nProgressStepPrev)
		return;

	if(m_eActivePage != EPageProgress && m_eActivePage != EPageUProgress)
		return;

	LogMessage("CWizardSheet::OnProgressUpdate(%d, %d, %d)", m_eActivePage, m_nProgressStep, m_nProgressStepPrev);

	m_nProgressStepPrev = m_nProgressStep;

	if(m_nProgressStep == EStepFailed)
	{
		CPromptDialog::MessageBox("Error", m_sProgressText.c_str(), "OK");
		((CWizardPageFinish*)m_pPage[m_eActivePage == EPageProgress ? EPageFinish : EPageUFinish])->OnFailed();
		OnButtonNext(0);
	}
	else if (m_nProgressStep == EStepSuccess)
		OnButtonNext(0);
	else
		((CWizardPageProgress*)m_pPage[m_eActivePage])->UpdateProgress(m_nProgressStep, m_sProgressText);

	m_pWindow->redraw();

}

bool CWizardSheet::OnProgressUpdate(int nStep, const CStdString &sText)
{

	LogMessage("CWizardSheet::OnProgressUpdate(%d, '%s')", nStep, sText.c_str());

	m_sProgressText = sText;
	m_nProgressStep = nStep;

	//Fl::awake(SOnProgressUpdate, this);
	/*Fl::lock();
	Fl::unlock();*/
	//Fl::awake();
	//Fl::add_timeout(0.010, SOnProgressUpdate, this);

	Sleep(50);

	if(m_nProgressStep == EStepFailed)
		return false;

	return true;
}

