/** \file
    Tab window.
*/

#pragma once

#include "VisualFx.h"

/** \ingroup	gui
    \brief	Tab Window.
*/
class oTTabWnd : public TTabWnd 
{
	friend	class	CMainFrame;

	DECLARE_DYNCREATE(oTTabWnd)

public:
	oTTabWnd();
	virtual ~oTTabWnd();
	void	ShowLastPane();

	virtual void OnSetActivePane(CWnd *pOldPane, CWnd *pNewPane);
	virtual BOOL CanSetActivePane(CWnd *pOldPane, CWnd *pNewPane);
private:
static	CMainFrame  *m_pFrameWnd;
	CWnd	*m_pLastPane;

	DECLARE_MESSAGE_MAP()
};
