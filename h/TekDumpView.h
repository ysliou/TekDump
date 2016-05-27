#ifndef __TEKDUMPVIEW_H
#define __TEKDUMPVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif

class CTekDumpDoc;

/** \brief  View for TekDump.
*/
class CTekDumpView : public CScrollView
{
	DECLARE_DYNCREATE(CTekDumpView)
protected:
	CTekDumpView();
	virtual ~CTekDumpView();

	CTekDumpDoc *m_pDoc;

	CBrush	m_brushBG;
private:
	CTekDumpDoc* GetDocument();

public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

protected:
	virtual void OnInitialUpdate(); // called first time after construct

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in TekDumpView.cpp
inline CTekDumpDoc* CTekDumpView::GetDocument()
   { return (CTekDumpDoc*)m_pDocument; }
#endif

extern	CTekDumpView *g_pTekDumpView;

#endif
