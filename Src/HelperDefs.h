//
//  @file HelperDefs.h
//  @author Sergii Oryshchenko <sergii.orishchenko@gmail.com>
//  @see https://github.com/os-v/UniProInstaller/
//
//  Created on 15.10.20.
//  Copyright 2020 Sergii Oryshchenko. All rights reserved.
//

#ifndef HELPERDEFS_H
#define HELPERDEFS_H

#include <cstdint>

#define UICOLOR_BACKGROUND		0xFFFFFFFF
#define UICOLOR_PRESSED			0xF0F0F0FF
#define UICOLOR_SELECTION			0xCDE8FF00
#define UICOLOR_PROGRESS			0xA0A0FF00
#define UICOLOR_EMPTY				0xE4E4E4FF
#define UICOLOR_BUTTON			0xFBFBFBFF
#define UICOLOR_BORDER			0xBCBCBCFF
#define UICOLOR_BORDER_DEFAULT		0x00000000
#define UICOLOR_TEXT				0x00000000
#define UICOLOR_HYPERLINK			0x3B719B00
#define UICOLOR_HYPERLINKHOWER		0x2D516F00

#define PTC(w1, w2)				((w1) / 2 - (w2) / 2)
#define PTCOL(x1, w1, w2)		((x1) + (w1) / 2 - (w2) / 2)
#define PTCOR(x2, w1, w2)		((x2) - (w1) / 2 - (w2) / 2)

#define FLSetVisible(p, f)		if(f) p->show(); else p->hide();
#define FLDestroy(p)			if(p) { delete p; p = 0; }

#define CASTWIDGETHANDLER(Class, Name)		static void S##Name(Fl_Widget *pWidget, void *pCtx) { ((Class*)pCtx)->Name(pWidget); }
#define CASTSIMPLEHANDLER(Class, Name)		static void S##Name(void *pCtx) { ((Class*)pCtx)->Name(); }

void UIPrepare();
int UIScale(int nValue);

#endif
