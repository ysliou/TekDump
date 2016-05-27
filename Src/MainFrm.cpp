/** \file	MainFrm.cpp
    \brief	Frame window for TekDump.

	   Date         Description
	----------  ---------------------------------------------
	2005/08/24  Created.
*/

#include "stdafx.h"

#include "TekDump.h"
#include "MainFrm.h"
#include "TekDumpView.h"
#include "setup.h"
#include "VisualFX\TabWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

CString	g_iniName;

/**	\brief	Constructor.
*/
CMainFrame::CMainFrame()
{
	CString path = GetAppPath();

	g_iniName = path + _T("\\TekDump.ini");
}

/**	\brief	Destructor.
*/
CMainFrame::~CMainFrame()
{
}

/** \brief  Crean main frame window.
*/
int	CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

/** \brief  Create all views.
*/
BOOL	CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	UNREFERENCED_PARAMETER(lpcs);

	int id = 1;

	TVisualObject *pvoSplitter = new TVisualObject(id++, _T(""), 1, 2, pContext);
	m_Framework.Add(pvoSplitter);

	m_pvoSetup = new TVisualObject(id++, 0, 0, pContext, 
				RUNTIME_CLASS(CSetup), CSize(180, 200));
	m_Framework.Add(pvoSplitter, m_pvoSetup);

	TVisualObject *pvoTabWnd = new TVisualObject(id++, 0, 1, pContext, 
				RUNTIME_CLASS(oTTabWnd), CSize(300, 320));
	m_Framework.Add(pvoSplitter, pvoTabWnd);

	m_pvoDump = new TVisualObject(id++, _T("Dump"), pContext, RUNTIME_CLASS(CTekDumpView));
	m_pvoDump->SetIcon(IDI_MONITOR);
	m_Framework.Add(pvoTabWnd, m_pvoDump);

	m_Framework.Create(this);
	m_font.CreatePointFont(100, _T("Verdana"));
	m_Framework.SetFont(&m_font);
	
	TVisualFrameworkIterator it(m_Framework);

#if 1
	return TRUE;
#else
	// 產生空白文件失敗
	return CFrameWnd::OnCreateClient(lpcs, pContext);
#endif
}

/** \brief  Destroy window.
*/
void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	m_Framework.Destroy();	
}

/** \brief	Get application path.
*/
CString CMainFrame::GetAppPath()
{
	CString path;

	::GetModuleFileName(NULL, path.GetBuffer(MAX_PATH), MAX_PATH);
	path.ReleaseBuffer();

	if (path.Find('\\') != -1)
	{
		for (int i=path.GetLength()-1; i>=0; i--)
		{
			TCHAR ch = path[i];
			path.Delete(i);
			if (ch == '\\')
				break;
		}
	}

	return path;
}

/** \brief  Show window.
*/
void	CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	CenterWindow();

	m_pvoDump->SetActivePane();

	g_pSetup = (CSetup *) m_pvoSetup->GetSafeWnd();
}
