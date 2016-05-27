#ifndef __TEKDUMP_H
#define __TEKDUMP_H

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h" 

/** \brief  Application for TekDump.
*/
class CTekDumpApp : public CWinApp
{
public:
	CTekDumpApp();

public:
	virtual BOOL InitInstance();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

#endif
