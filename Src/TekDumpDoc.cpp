/** \file	TekDumpDoc.cpp
    \brief	Document for THS720 screen dump.

	   Date		Description
	----------  ---------------------------------------------
	2005/08/24  Created.
	2013/10/16	Rewrite using CImage.
*/

#include "stdafx.h"
#include <io.h>
#include <atlimage.h>

#include "TekDump.h"
#include "TekDumpDoc.h"
#include "TekDumpView.h"
#include "setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CTekDumpDoc, CDocument)

BEGIN_MESSAGE_MAP(CTekDumpDoc, CDocument)
	ON_COMMAND(ID_FILE_LOADTEST, &CTekDumpDoc::OnFileLoadtest)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_FRAME_FIRST, OnFrameFirst)
	ON_UPDATE_COMMAND_UI(ID_FRAME_FIRST, OnUpdateFrameFirst)
	ON_COMMAND(ID_FRAME_LAST, OnFrameLast)
	ON_UPDATE_COMMAND_UI(ID_FRAME_LAST, OnUpdateFrameLast)
	ON_COMMAND(ID_FRAME_NEXT, OnFrameNext)
	ON_UPDATE_COMMAND_UI(ID_FRAME_NEXT, OnUpdateFrameNext)
	ON_COMMAND(ID_FRAME_NEXT10, OnFrameNext10)
	ON_UPDATE_COMMAND_UI(ID_FRAME_NEXT10, OnUpdateFrameNext10)
	ON_COMMAND(ID_FRAME_PREV, OnFramePrev)
	ON_UPDATE_COMMAND_UI(ID_FRAME_PREV, OnUpdateFramePrev)
	ON_COMMAND(ID_FRAME_PREV10, OnFramePrev10)
	ON_UPDATE_COMMAND_UI(ID_FRAME_PREV10, OnUpdateFramePrev10)
END_MESSAGE_MAP()

CTekDumpDoc *g_pTekDumpDoc;

/** \brief  Constructor.
*/
CTekDumpDoc::CTekDumpDoc()
{
	m_imgNo = 0;
	m_saveIndex = -1;
	m_showIndex = -1;
	m_lpBMIH = NULL;
	m_lpImage = NULL;

	g_pTekDumpDoc = this;
}

/** \brief  Destructor.
*/
CTekDumpDoc::~CTekDumpDoc()
{
	g_pTekDumpDoc = NULL;

	if (m_lpBMIH != NULL)
		delete[] m_lpBMIH;

	if (m_lpImage != NULL)
		delete[] m_lpImage;
}
	
/** \brief  Get image to shown.
*/
CImage	*CTekDumpDoc::GetImageToShow()
{
	if (m_showIndex >= 0)
	{
		int	imgW = m_images[m_showIndex].GetWidth();
		int imgH = m_images[m_showIndex].GetHeight();

	    g_pSetup->ShowData(IDC_DATASET_SIZE, _T("%ld"), m_dataSize[m_showIndex]);
	    g_pSetup->ShowData(IDC_IMAGE_SIZE, _T("%d*%d"), imgW, imgH);
	    g_pSetup->ShowData(IDC_IMAGE_COUNT, _T("%d/%d"), m_showIndex+1, m_imgNo);

	    return &m_images[m_showIndex];
	}
	else
	    return NULL;
}

/** \brief  Store received data to image array.

    \param[in]	*pBuffer    Pointer to input buffer (Bitmap file).
    \param[in]	dataSize    Data length in buffer.

	- The received data is a BMP file that rotated anti-clockwise by 90 degrees.
	- The saved image is rotated clockwise by 90 degrees.
	- The default resolution is 96 DPI, but 72 DPI is need.
*/
CSize	CTekDumpDoc::StoreImage(char *pBuffer, long dataSize)
{
	if (RetrieveBitmap(pBuffer, dataSize) == 0)
	{
		m_saveIndex = (m_saveIndex + 1) % MAX_IMGNO;
		m_dataSize[m_saveIndex] = dataSize;

		CImage *pImage = &m_images[m_saveIndex];

		m_imgH = m_imgOrgW;
		m_imgW = m_imgOrgH;
		pImage->Create(m_imgW, m_imgH, m_nBitCount);	// swap height and width

		RGBQUAD prgbColors[2]; 
		prgbColors[0].rgbBlue = 0;
		prgbColors[0].rgbGreen = 0;
		prgbColors[0].rgbRed = 0;
		prgbColors[0].rgbReserved = 0;
		prgbColors[1].rgbBlue = 255;
		prgbColors[1].rgbGreen = 255;
		prgbColors[1].rgbRed = 255;
		prgbColors[1].rgbReserved = 0;

		pImage->SetColorTable(0, 2, prgbColors);

		for (int y=0; y<m_imgH; y++)
		{
			for (int x=0; x<m_imgW; x++)
			{
				DWORD	pixel = GetPixel(y, m_imgOrgH-1-x);
				pImage->SetPixel(x, y, pixel ? 0xffffff : 0x000000);
			}
		}
		
		m_showIndex = m_saveIndex;
		if (m_imgNo < MAX_IMGNO)
			m_imgNo++;	

		return CSize(m_imgH, m_imgW);
	}

	return CSize(0, 0);
}

/**	\brief	Retrieve image from RS-232 receive buffer.

	\param[in]	pBmp	Pointer to BMP image
	\param[in]	dataSize Data size of the image

	The captured image is rotated by 90 degrees.
	Change resolution from Set image to 72 DPI.
*/
int	CTekDumpDoc::RetrieveBitmap(LPSTR pBmp, long dataSize)
{
	if (dataSize < sizeof(BITMAPFILEHEADER))
		return -1;

	memcpy(&m_bmfh, pBmp, sizeof(BITMAPFILEHEADER));
	pBmp += sizeof(BITMAPFILEHEADER);
	dataSize -= sizeof(BITMAPFILEHEADER);
	if (m_bmfh.bfType != 0x4d42)	// 'BM'
	    return -1;					// invalid bitmap

	int nSize = m_bmfh.bfOffBits - sizeof(BITMAPFILEHEADER);
	if (dataSize < nSize)
		return -1;

	if (m_lpBMIH != NULL)
		delete[] m_lpBMIH;

	m_lpBMIH = (LPBITMAPINFOHEADER) new char[nSize];
	memcpy(m_lpBMIH, pBmp, nSize);	// info hdr & color table
	pBmp += nSize;
	dataSize -= nSize;

	// check if Windows 3.x or Windows NT BMP
	if (m_lpBMIH->biSize!=40)
	    return -1;		// bad format

	// check if compressed
	if ((m_lpBMIH->biCompression==BI_RLE8) || (m_lpBMIH->biCompression==BI_RLE4))
	    return -1;		// compress error

	//	change resolution from 3780 (96 DPI) to 2835 (72 DPI).
	if ((m_lpBMIH->biXPelsPerMeter != 2835) || (m_lpBMIH->biYPelsPerMeter != 2835))
	{
		m_lpBMIH->biXPelsPerMeter = 2835;		// set resolution (pixels/meter)
		m_lpBMIH->biYPelsPerMeter = 2835;
	}

	m_imgOrgW = m_lpBMIH->biWidth;
	m_imgOrgH = m_lpBMIH->biHeight;
	m_nBitCount = m_lpBMIH->biBitCount;
	m_dwSizeImage = m_lpBMIH->biSizeImage;
	m_nByteWidth = ((m_imgOrgW*m_nBitCount + 31) / 32 * 4);

	if (dataSize < (long) m_dwSizeImage) 
		return -1;

	if (m_lpImage != NULL)
		delete[] m_lpImage;

	m_lpImage = (LPBYTE) new char[m_dwSizeImage];
	memcpy(m_lpImage, pBmp, m_dwSizeImage);		// image bits only

	return 0;
}

/**	\brief	Get a pixel from received image buffer.

	\param[in]	x	X coordinate.
	\oaram[in]	y	Y coordinate.
*/
DWORD	CTekDumpDoc::GetPixel(int x, int y) const
{
	register BYTE* adr;

	if ((x>=0) && (x<m_imgOrgW) && (y>=0) && (y<m_imgOrgH))
	    switch(m_lpBMIH->biBitCount) 
	    {
		case 1:
	    	adr = m_lpImage + (m_imgOrgH-y-1) * m_nByteWidth + (x>>3);
		    return (((*adr) & (0x80 >> (x & 0x7))) != 0);

		case 8:
		    adr = m_lpImage + (m_imgOrgH-y-1) * m_nByteWidth + x;
		    return (*adr) & 0xff;

		case 24:
		    adr = m_lpImage + (m_imgOrgH-y-1) * m_nByteWidth + x + x + x;
		    return ((*(DWORD*)adr) & 0xffffff);

		default:
		    AfxMessageBox(_T("GetPixel: only implemented for 1,8 and 24 bit per pixels"));
		    return (DWORD) -1;
	    }

	return (DWORD) -1;
}

/**	\brief	Load a test image to test image processing functions.
*/
void CTekDumpDoc::OnFileLoadtest()
{
	LPCTSTR pszSource[] = {
		_T("TestImg\\CAN.bmp"),
		_T("TestImg\\NPort.bmp"),
		_T("TestImg\\QSP200.bmp"),
		_T("TestImg\\UPort1150_0.bmp"),
		_T("TestImg\\UPort1150_1.bmp"),
		_T("TestImg\\UPort1150_2.bmp"),
		_T("TestImg\\UPort1150_3.bmp"),
		_T("TestImg\\UPort1150_to_NPort.bmp"),
		_T("TestImg\\UPort1150_to_UPort.bmp")};
	static int nImage = 0;

	CFile file;
	CFileException ex;

	char	buffer[BUFSIZE];
	TCHAR szError[1024];

	if (!file.Open(pszSource[nImage], CFile::modeRead, &ex))
	{
		ex.GetErrorMessage(szError, 1024);
	}
	else
	{
		long len = (long) file.GetLength();
		file.Read(buffer, len);
		CSize imgSize = g_pTekDumpDoc->StoreImage(buffer, len);
	    g_pTekDumpView->RedrawWindow();
	}

	if (++nImage == LENGTH(pszSource))
		nImage = 0;
}

/** \brief  Enable 'save' button if image number > 0.
*/
void	CTekDumpDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_imgNo > 0);	
}

/** \brief  Save current shown image to file.
*/
void	CTekDumpDoc::OnFileSave() 
{
	CString pathName;
	
	pathName.Format(_T("Scope%03d"), m_showIndex + g_pSetup->m_seqNo); 

	CImage *pImage = &m_images[m_showIndex];

	TCHAR *dlgTitle = _T("Save Scope Image");
	CFileDialog dlg (FALSE, _T("*.png"), pathName, OFN_HIDEREADONLY,_T("Portable Network Graphics (*.png)|*.png||"));
	dlg.m_ofn.lpstrTitle = dlgTitle;
	if (dlg.DoModal() == IDOK)
	{
		CString filename = dlg.GetPathName();
		if ((_taccess(filename, 0) == -1) ||
			(MessageBox(NULL, filename + _T(" already existed, Overwrite it?"), dlgTitle, MB_YESNO | MB_ICONINFORMATION) == IDYES))
			pImage->Save(dlg.GetPathName());
	}
}

/** \brief  Goto first image.
*/
void	CTekDumpDoc::OnUpdateFrameFirst(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_showIndex > 0);
}

void	CTekDumpDoc::OnFrameFirst() 
{
	m_showIndex = 0;
	UpdateAllViews(NULL);
}

/** \brief  Goto last image.
*/
void	CTekDumpDoc::OnUpdateFrameLast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_showIndex != m_saveIndex);
}

void	CTekDumpDoc::OnFrameLast() 
{
	m_showIndex = m_saveIndex;
	UpdateAllViews(NULL);
}

/** \brief  Goto next image.
*/
void	CTekDumpDoc::OnUpdateFrameNext(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_showIndex < m_saveIndex);
}

void	CTekDumpDoc::OnFrameNext() 
{
	m_showIndex++;
	UpdateAllViews(NULL);
}

/** \brief  Goto previous image.
*/
void	CTekDumpDoc::OnUpdateFramePrev(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_showIndex > 0);
}

void	CTekDumpDoc::OnFramePrev() 
{
	m_showIndex--;
	UpdateAllViews(NULL);
}

/** \brief  Goto next 10  image.
*/
void	CTekDumpDoc::OnUpdateFrameNext10(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_showIndex <= m_saveIndex - 10);
}

void	CTekDumpDoc::OnFrameNext10() 
{
	m_showIndex += 10;
	UpdateAllViews(NULL);
}

/** \brief  Goto previous 10  image.
*/
void CTekDumpDoc::OnUpdateFramePrev10(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_showIndex >= 10);
}

void	CTekDumpDoc::OnFramePrev10() 
{
	m_showIndex -= 10;
	UpdateAllViews(NULL);
}

