
// addition3.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Caddition3App:
// See addition3.cpp for the implementation of this class
//

class Caddition3App : public CWinApp
{
public:
	Caddition3App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Caddition3App theApp;