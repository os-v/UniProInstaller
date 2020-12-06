//
//  @file WizardPageLicense.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef WIZARDPAGELICENSE_H
#define WIZARDPAGELICENSE_H

#include <FL/Fl_Text_Editor.H>
#include "WizardPageBase.h"
#include "../UI/CustomTextEditor.h"
#include "../Resources.h"
#include "../../Common/UtilsMisc.h"

class CWizardPageLicense : public CWizardPageBase
{
public:

	CWizardPageLicense(CWizardSheet *pParent) : CWizardPageBase(pParent)
	{
		m_pEditLicense = 0;
		m_pTextLicense = 0;
	}

	~CWizardPageLicense()
	{
		//FLDestroy(m_pEditLicense);
		//FLDestroy(m_pTextLicense);
	}

	virtual bool CreatePage(int nPosX, int nPosY, int nWidth, int nHeight)
	{

#ifndef DEFUNINSTALL
		CStdString sLicense((char*)ResLicenseData(), ResLicenseSize());
#else
		CStdString sLicense;
#endif
		for(size_t iPos = 0; (iPos = sLicense.find("\r", iPos)) != -1; )
			sLicense.replace(iPos, 1, "");

		m_pTextLicense = new Fl_Text_Buffer();
		m_pTextLicense->text(sLicense.c_str());

		int nMargin = UIScale(5);

		m_pEditLicense = new CCustomTextEditor(nPosX + nMargin, nPosY + nMargin, nWidth - nMargin * 2, nHeight - nMargin * 2);
		m_pEditLicense->box(FL_NO_BOX);
		m_pEditLicense->selection_color(UICOLOR_SELECTION);
		m_pEditLicense->buffer(m_pTextLicense);
		m_pEditLicense->wrap_mode(Fl_Text_Editor::WRAP_AT_BOUNDS, 0);
		m_pEditLicense->hide_cursor();
		m_pEditLicense->SetReadOnly(true);

		OnSetActive(false);

		return true;
	}

	virtual void OnSetActive(bool fActive)
	{
		FLSetVisible(m_pEditLicense, fActive);
	}

	virtual const char *GetNextButtonTitle()
	{
		return GSTR(Accept);
	}

	virtual const char *GetTitle()
	{
		return GSTR(LicenseAgreement);
	}

	virtual const char *GetSubTitle()
	{
		return GSTR(PleaseReadLicense);
	}

protected:

	CCustomTextEditor *m_pEditLicense;
	Fl_Text_Buffer *m_pTextLicense;

};

#endif

