/** \file	TabWnd.cpp
	\brief	Tab window.

	   Date 	Description
	----------	-------------------------------------------------------
	2003/04/23	Created.
*/
 
#include <stdafx.h>

#include "tabwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(oTTabWnd, TTabWnd)

CMainFrame  *oTTabWnd::m_pFrameWnd = NULL;

/** \brief  Constructor.
*/
oTTabWnd::oTTabWnd()
{
	m_pLastPane = NULL;
}

/** \brief  Destructor.
*/
oTTabWnd::~oTTabWnd()
{
}

BEGIN_MESSAGE_MAP(oTTabWnd, TTabWnd)
END_MESSAGE_MAP()

/** \brief  Set active pane.
    
    \param[in]	pOldPane
    \param[in]	pNewPane
*/
void	oTTabWnd::OnSetActivePane(CWnd *pOldPane, CWnd *pNewPane)
{
	UNREFERENCED_PARAMETER(pNewPane);

	m_pLastPane = pOldPane;

//	m_pFrameWnd->ActivePane(pOldPane, pNewPane);
}

/** \brief  Check if a pane can be actived.
    
    \param[in]	pOldPane
    \param[in]	pNewPane
*/
BOOL	oTTabWnd::CanSetActivePane(CWnd *pOldPane, CWnd *pNewPane)
{
	UNREFERENCED_PARAMETER(pOldPane);
	UNREFERENCED_PARAMETER(pNewPane);

	return TRUE;
}

/** \brief  Switch to last pane.
*/
void	oTTabWnd::ShowLastPane()
{
	int nIndex = GetTabIndex(m_pLastPane);
	SetActivePane(nIndex);
}
