#ifndef __TEKDUMPDOC_H
#define __TEKDUMPDOC_H

#include <atlimage.h>

#if _MSC_VER > 1000
#pragma once
#endif

#define	MAX_IMGNO   100

/** \brief  Document for TekDump.
*/
class CTekDumpDoc : public CDocument
{
	friend class CTekDumpView;

	DECLARE_DYNCREATE(CTekDumpDoc)
public:
	CTekDumpDoc();
	virtual ~CTekDumpDoc();

	CImage	*GetImageToShow();
	CSize	StoreImage(char *pBuffer, long dataSize);

protected:
	CImage	m_images[MAX_IMGNO];
	long	m_dataSize[MAX_IMGNO];
	int		m_imgNo;
	int		m_saveIndex;
	int		m_showIndex;

	int		m_imgW;				// image width
	int		m_imgH;				// image height

	//	BITMAP from TH720
	int		RetrieveBitmap(LPSTR pBmp, long size);
	DWORD	GetPixel(int x, int y) const;	// get pixel value (with security)

	BITMAPFILEHEADER m_bmfh;		// bitmap file header
	LPBITMAPINFOHEADER m_lpBMIH;	// pointer to BITMAPINFOHEADER structure
	int		m_imgOrgW;				// original width
	int		m_imgOrgH;				// original height
	int		m_nBitCount;			// bit count per pixel
	DWORD	m_dwSizeImage;			// size of image (include zeropadding)
	int		m_nByteWidth;			// width of image (include zeropadding)
	LPBYTE	m_lpImage;				// pointer to image pixels

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileLoadtest();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnFrameFirst();
	afx_msg void OnUpdateFrameFirst(CCmdUI* pCmdUI);
	afx_msg void OnFrameLast();
	afx_msg void OnUpdateFrameLast(CCmdUI* pCmdUI);
	afx_msg void OnFrameNext();
	afx_msg void OnUpdateFrameNext(CCmdUI* pCmdUI);
	afx_msg void OnFrameNext10();
	afx_msg void OnUpdateFrameNext10(CCmdUI* pCmdUI);
	afx_msg void OnFramePrev();
	afx_msg void OnUpdateFramePrev(CCmdUI* pCmdUI);
	afx_msg void OnFramePrev10();
	afx_msg void OnUpdateFramePrev10(CCmdUI* pCmdUI);
};

extern CTekDumpDoc *g_pTekDumpDoc;

#endif
