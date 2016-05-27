#ifndef __MAINFRM_H
#define __MAINFRM_H

#pragma once

#include "VisualFX\VisualFX.h"

/** \brief  Frame window for TekDump.
*/
class CMainFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMainFrame)

protected:
	CMainFrame();
	virtual ~CMainFrame();

	CString GetAppPath();

	TVisualObject *m_pvoSetup;
	TVisualObject *m_pvoDump;

	TVisualFramework    m_Framework;
	CFont	m_font;	    // font style for TVisualFramework

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()
};

#endif
