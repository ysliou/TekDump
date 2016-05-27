/** \file	TekDumpView.cpp
    \brief	Show THS720 screen dump.

	   Date		Description
	----------  ---------------------------------------------
	2005/08/24  Created.
*/

#include "stdafx.h"
#include <vfw.h>

#include "TekDump.h"
#include "TekDumpDoc.h"
#include "TekDumpView.h"
#include "YSL\MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CTekDumpView, CScrollView)

BEGIN_MESSAGE_MAP(CTekDumpView, CScrollView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CTekDumpView	*g_pTekDumpView = NULL;

/** \brief  Constructor.
*/
CTekDumpView::CTekDumpView()
{
	g_pTekDumpView = this;

	m_brushBG.CreateSolidBrush(RGB(160, 160, 255));
}

/** \brief  Destructor.
*/
CTekDumpView::~CTekDumpView()
{
	g_pTekDumpView = NULL;
}

#ifdef _DEBUG
CTekDumpDoc* CTekDumpView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTekDumpDoc)));
	return (CTekDumpDoc*)m_pDocument;
}
#endif

/** \brief  Initial view and open COM port.
*/
void	CTekDumpView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(380, 360));
	ResizeParentToFit(FALSE);

	CRect rcBounds;
	GetClientRect(&rcBounds);
	SetScrollSizes(MM_TEXT, rcBounds.Size());

	m_pDoc = GetDocument();
}

/** \brief  Draw view.
*/
void	CTekDumpView::OnDraw(CDC* pDC)
{
	const int offsetX = 10;
	const int offsetY = 10;

	CRect rcBounds;

	GetClientRect(&rcBounds);
	if ((rcBounds.Width() < offsetX) || (rcBounds.Height() < offsetY))
		return;

	CMemDC0 *pMemDC = new CMemDC0(pDC);

	pMemDC->FillRect(rcBounds, &m_brushBG);
	pMemDC->SelectObject(m_brushBG);
	pMemDC->SetBkMode(TRANSPARENT);

	CImage *pImage = m_pDoc->GetImageToShow();
	if ((pImage != NULL) && !pImage->IsNull())
	{
		if (pImage->Draw(pMemDC->GetSafeHdc(), offsetX, offsetY) == FALSE)
			AfxMessageBox(_T("Draw Image error"));
	}

	delete pMemDC;
}

/** \brief  Erase background myself.
*/
BOOL CTekDumpView::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);

	return FALSE;
}
