
// KRACK-Attack.h : main header file for the KRACK-Attack application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKRACKAttackApp:
// See KRACK-Attack.cpp for the implementation of this class
//

class CKRACKAttackApp : public CWinApp
{
public:
	CKRACKAttackApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKRACKAttackApp theApp;
