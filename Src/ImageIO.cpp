/** \file	ImageIO.cpp
    \brief	Image file I/O.

	   Date		Description
	----------  ---------------------------------------------
	2004/11/02  Created.
*/

#include "stdafx.h"

#include "ximage.h"
#include "ximagif.h"
#include "ximatif.h"
#include "xfile.h"
#include "ImageIO.h"
#include "dibkernel\dibkernel.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

DocType doctypes[CMAX_IMAGE_FORMATS] =
{
	{ -1, TRUE, TRUE, "Supported files", "*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;*.ppm;*.ras" },
#if CXIMAGE_SUPPORT_BMP
	{ CXIMAGE_FORMAT_BMP, TRUE, TRUE, "BMP files", "*.bmp" },
#endif
#if CXIMAGE_SUPPORT_GIF
	{ CXIMAGE_FORMAT_GIF, TRUE, TRUE, "GIF files", "*.gif" },
#endif
#if CXIMAGE_SUPPORT_JPG
	{ CXIMAGE_FORMAT_JPG, TRUE, TRUE, "JPG files", "*.jpg;*.jpeg" },
#endif
#if CXIMAGE_SUPPORT_PNG
	{ CXIMAGE_FORMAT_PNG, TRUE, TRUE, "PNG files", "*.png" },
#endif
#if CXIMAGE_SUPPORT_MNG
	{ CXIMAGE_FORMAT_MNG, TRUE, TRUE, "MNG files", "*.mng;*.jng;*.png" },
#endif
#if CXIMAGE_SUPPORT_ICO
	{ CXIMAGE_FORMAT_ICO, TRUE, TRUE, "ICO CUR files", "*.ico;*.cur" },
#endif
#if CXIMAGE_SUPPORT_TIF
	{ CXIMAGE_FORMAT_TIF, TRUE, TRUE, "TIF files", "*.tif;*.tiff" },
#endif
#if CXIMAGE_SUPPORT_TGA
	{ CXIMAGE_FORMAT_TGA, TRUE, TRUE, "TGA files", "*.tga" },
#endif
#if CXIMAGE_SUPPORT_PCX
	{ CXIMAGE_FORMAT_PCX, TRUE, TRUE, "PCX files", "*.pcx" },
#endif
#if CXIMAGE_SUPPORT_WBMP
	{ CXIMAGE_FORMAT_WBMP, TRUE, TRUE, "WBMP files", "*.wbmp" },
#endif
#if CXIMAGE_SUPPORT_WMF
	{ CXIMAGE_FORMAT_WMF, TRUE, FALSE, "WMF EMF files", "*.wmf;*.emf" },
#endif
#if CXIMAGE_SUPPORT_J2K
	{ CXIMAGE_FORMAT_J2K, TRUE, TRUE, "J2K files", "*.j2k;*.jp2" },
#endif
#if CXIMAGE_SUPPORT_JBG
	{ CXIMAGE_FORMAT_JBG, TRUE, TRUE, "JBG files", "*.jbg" },
#endif
#if CXIMAGE_SUPPORT_JP2
	{ CXIMAGE_FORMAT_JP2, TRUE, TRUE, "JP2 files", "*.j2k;*.jp2" },
#endif
#if CXIMAGE_SUPPORT_JPC
	{ CXIMAGE_FORMAT_JPC, TRUE, TRUE, "JPC files", "*.j2c;*.jpc" },
#endif
#if CXIMAGE_SUPPORT_PGX
	{ CXIMAGE_FORMAT_PGX, TRUE, TRUE, "PGX files", "*.pgx" },
#endif
#if CXIMAGE_SUPPORT_RAS
	{ CXIMAGE_FORMAT_RAS, TRUE, TRUE, "RAS files", "*.ras" },
#endif
#if CXIMAGE_SUPPORT_PNM
	{ CXIMAGE_FORMAT_PNM, TRUE, TRUE, "PNM files", "*.pnm;*.pgm;*.ppm" }
#endif
};

/**
    Get file extension name.

    \parm[in]	name	Filename.
*/
CString FindExtension(const CString& name)
{
	int len = name.GetLength();

	for (int i=len-1; i>=0; i--)
	{
	    if (name[i] == '.')
	    {
		return name.Mid(i+1);
	    }
	}

	return CString("");
}

/**
    Give file extension name, return corresponding file type.
*/
int	FindType(const CString& ext)
{
	int type = 0;

	if (ext == "bmp")			type = CXIMAGE_FORMAT_BMP;
#if CXIMAGE_SUPPORT_JPG
	else if (ext=="jpg"||ext=="jpeg")	type = CXIMAGE_FORMAT_JPG;
#endif
#if CXIMAGE_SUPPORT_GIF
	else if (ext == "gif")			type = CXIMAGE_FORMAT_GIF;
#endif
#if CXIMAGE_SUPPORT_PNG
	else if (ext == "png")			type = CXIMAGE_FORMAT_PNG;
#endif
#if CXIMAGE_SUPPORT_MNG
	else if (ext=="mng"||ext=="jng")	type = CXIMAGE_FORMAT_MNG;
#endif
#if CXIMAGE_SUPPORT_ICO
	else if (ext == "ico")			type = CXIMAGE_FORMAT_ICO;
#endif
#if CXIMAGE_SUPPORT_TIF
	else if (ext=="tiff"||ext=="tif")	type = CXIMAGE_FORMAT_TIF;
#endif
#if CXIMAGE_SUPPORT_TGA
	else if (ext=="tga")			type = CXIMAGE_FORMAT_TGA;
#endif
#if CXIMAGE_SUPPORT_PCX
	else if (ext=="pcx")			type = CXIMAGE_FORMAT_PCX;
#endif
#if CXIMAGE_SUPPORT_WBMP
	else if (ext=="wbmp")			type = CXIMAGE_FORMAT_WBMP;
#endif
#if CXIMAGE_SUPPORT_WMF
	else if (ext=="wmf"||ext=="emf")	type = CXIMAGE_FORMAT_WMF;
#endif
#if CXIMAGE_SUPPORT_J2K
	else if (ext=="j2k"||ext=="jp2")	type = CXIMAGE_FORMAT_J2K;
#endif
#if CXIMAGE_SUPPORT_JBG
	else if (ext=="jbg")			type = CXIMAGE_FORMAT_JBG;
#endif
#if CXIMAGE_SUPPORT_JP2
	else if (ext=="jp2"||ext=="j2k")	type = CXIMAGE_FORMAT_JP2;
#endif
#if CXIMAGE_SUPPORT_JPC
	else if (ext=="jpc"||ext=="j2c")	type = CXIMAGE_FORMAT_JPC;
#endif
#if CXIMAGE_SUPPORT_PGX
	else if (ext=="pgx")			type = CXIMAGE_FORMAT_PGX;
#endif
#if CXIMAGE_SUPPORT_RAS
	else if (ext=="ras")			type = CXIMAGE_FORMAT_RAS;
#endif
#if CXIMAGE_SUPPORT_PNM
	else if (ext=="pnm"||ext=="pgm"||ext=="ppm") type = CXIMAGE_FORMAT_PNM;
#endif
	else type = CXIMAGE_FORMAT_UNKNOWN;

	return type;
}

/**
    Give index in doctypes[], return corresponding file type.
*/
int	GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
	int nCnt = 0;

	for (int i=0; i<CMAX_IMAGE_FORMATS; i++)
	{
	    if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite)
	    {
		if (nCnt == nIndex)
		    return doctypes[i].nID;
		nCnt++;
	    }
	}

	ASSERT(FALSE);
	return -1;
}

/**
    Give file type, return index in doctypes[].
*/
int	GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
	int nCnt = 0;

	for (int i=0; i<CMAX_IMAGE_FORMATS; i++)
	{
	    if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite)
	    {
		if (doctypes[i].nID == nDocType)
		    return nCnt;
		nCnt++;
	    }
	}

	return -1;
}

/**
    Give file type, return corresponding file extension names.
*/
CString GetExtFromType(int nDocType)
{
	for (int i=0; i<CMAX_IMAGE_FORMATS; i++)
	{
	    if (doctypes[i].nID == nDocType)
		return doctypes[i].ext;
	}
	return CString("");
}

/**
    Give file type, return corresponding file description.
*/
CString GetDescFromType(int nDocType)
{
	for (int i=0; i<CMAX_IMAGE_FORMATS; i++)
	{
	    if (doctypes[i].nID == nDocType)
		return doctypes[i].description;
	}
	return CString("");
}

/**
    Return file extensions names for read/write operation.
*/
CString GetFileTypes(BOOL bOpenFileDialog)
{
	CString str;

	for (int i=0; i<CMAX_IMAGE_FORMATS; i++)
	{
	    if (bOpenFileDialog && doctypes[i].bRead)
	    {
		str += doctypes[i].description;
		str += (TCHAR)NULL;
		str += doctypes[i].ext;
		str += (TCHAR)NULL;
	    } 
	    else if (!bOpenFileDialog && doctypes[i].bWrite) 
	    {
		str += doctypes[i].description;
		str += (TCHAR)NULL;
		str += doctypes[i].ext;
		str += (TCHAR)NULL;
	    }
	}
	str += (TCHAR)NULL;	// YSL, 2004/11/06

	return str;
}

/**
    Give file type, return writable flag.
*/
BOOL	GetWritableType(int nDocType)
{
	for (int i=0; i<CMAX_IMAGE_FORMATS; i++)
	{
	    if (doctypes[i].nID == nDocType)
		return doctypes[i].bWrite;
	}
	return FALSE;
}

/**
    Store image DIB.
*/
BOOL	StoreImage(CDib *dib, LPSTR image)
{
	LPBITMAPINFOHEADER bih = (LPBITMAPINFOHEADER) image;
	if (bih->biSizeImage >= 1)
	{
   	    // set image size
	    CSize imgSize(bih->biWidth, bih->biHeight);

	    // allocate and copy image bits
	    *dib = CDib(imgSize, bih->biBitCount);
	    memcpy(dib->m_lpImage, image+dib->GetSizeHeader(), 
	                                 dib->GetSizeImage());
	    if (bih->biBitCount == 8)
	    {
		DWORD *palette = (DWORD *) (image + sizeof(BITMAPINFOHEADER));
		for (int i=0; i<256; i++)
		    dib->SetPaletteEntry((BYTE) i, *palette++);
	    }
	}

	return (bih->biSizeImage >= 1);
}

/**
    prompt for file name - used for open and save as static function
*/
BOOL	PromptForFileName(CString& fileName, UINT nIDSTitle, 
		DWORD dwFlags, BOOL bOpenFileDialog, int *pType)
{
	UNREFERENCED_PARAMETER(nIDSTitle);

	CString title;
	if (bOpenFileDialog) 
	    title = "Open image file"; 
	else 
	    title = "Save image file";

	//  Get file type
	int nDocType = (pType != NULL) ? *pType : CXIMAGE_FORMAT_BMP;

	if (!fileName.IsEmpty())
	{
	    CString extName(FindExtension(fileName));
	    extName.MakeLower();
	    int type = FindType(extName);
	    if (type != CXIMAGE_FORMAT_UNKNOWN)
		nDocType = type;
	}

	if (nDocType < -1 || nDocType >= CMAX_IMAGE_FORMATS) 
	    nDocType = CXIMAGE_FORMAT_BMP;

	int nIndex = GetIndexFromType(nDocType, bOpenFileDialog);
	if (nIndex == -1) 
	    nIndex = 0;

	//  Initial dialog
	CFileDialog dlgFile(bOpenFileDialog);
		
	CString strFilter = GetFileTypes(bOpenFileDialog);
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.nFilterIndex = nIndex + 1;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.Flags |= dwFlags;

	// strDefExt is necessary to hold onto the memory from GetExtFromType
	CString strDefExt = GetExtFromType(nDocType).Mid(2,3);
	dlgFile.m_ofn.lpstrDefExt = strDefExt;

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	if (bRet)
	{
	    if (pType != NULL)
	    {
		int nIndex = (int) dlgFile.m_ofn.nFilterIndex - 1;
		ASSERT(nIndex >= 0);
		*pType = GetTypeFromIndex(nIndex, bOpenFileDialog);
	    }
	}

	return bRet;
}

/**
    Read an image file.
*/
BOOL	ReadImageFile(CDib *pDib, CString pathName)
{
	if (pathName.IsEmpty())
	{
	    int nDocType = -1;	// all support types
	    if (!PromptForFileName(pathName, AFX_IDS_OPENFILE,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, &nDocType))
		return FALSE;
	}

	//  get extension name
	CString fileName;
	CString extName;
	
	int dotPos = pathName.ReverseFind('.');
	if (dotPos != -1)
	    extName = pathName.Mid(dotPos+1);
	else
	    return FALSE;

	//  get base file name
	dotPos = pathName.ReverseFind('\\');
	if (dotPos == -1)
	    fileName = pathName;
	else
	    fileName = pathName.Mid(dotPos+1);

	//  read image file
	extName.MakeLower();
	int nDocType = FindType(extName);

	CxImage *image = new CxImage(pathName, nDocType);
	if (!image->IsValid())
	{
	    AfxMessageBox(image->GetLastError());
	    delete image;
	    image = NULL;
	    return FALSE;
	}
	
	CSize imgSize(image->GetWidth(), image->GetHeight());
	*pDib = CDib(imgSize, image->GetBpp(), FALSE);
	pDib->m_FileInfo.FileName = fileName; 
	memcpy(pDib->m_lpBMIH, image->GetDIB(), sizeof(BITMAPINFOHEADER));
	DWORD paletteSize = image->GetPaletteSize();
	if (paletteSize > 0)
	{
	    RGBQUAD *srcPalette = image->GetPalette();
	    if (srcPalette != NULL)
	    {
		BYTE *dstPalette = ((BYTE *) pDib->m_lpBMIH) + sizeof(BITMAPINFOHEADER);
		memcpy(dstPalette, srcPalette, paletteSize);
	    }
	    else
	    {
		for (int i=0; i<256; i++)
		    pDib->SetPaletteEntry((BYTE) i, RGB(i, i, i));
	    }
	}
	memcpy(pDib->m_lpImage, image->GetBits(), pDib->GetSizeImage());
	delete image;
	return(TRUE);
}

/**
    Save image file.
*/
BOOL	SaveImageFile(CDib *pDib, CString pathName, int nDocType, BOOL bReplace /*=TRUE*/)
{
	UNREFERENCED_PARAMETER(bReplace);

	if (pDib == NULL || !pDib->DibLoaded())
	    return FALSE;

	if (pathName.IsEmpty())
	    return FALSE;

	CString extName(FindExtension(pathName));
	if (extName != "")
	{
	    extName.MakeLower();
	    nDocType = FindType(extName);
	}
	else
	{
	    extName = GetExtFromType(nDocType);
	    pathName += extName.Mid(extName.ReverseFind('.'));
	}

	if (nDocType <= CXIMAGE_FORMAT_UNKNOWN || nDocType >= CMAX_IMAGE_FORMATS)
	    return FALSE;

        if (!GetWritableType(nDocType))	
	    return FALSE;

	int nBitCount = pDib->m_lpBMIH->biBitCount;
	if (nDocType == CXIMAGE_FORMAT_GIF && nBitCount>8)
	{
	    AfxMessageBox(_T("The image will be saved as a true color GIF!\n")
					_T("This is ok for CxImage, but not for many other programs.\n")
					_T("For better compatibility, please use DecreaseBpp to 8 bits or less."),
		MB_ICONINFORMATION);
	}

	CSize imgSize = pDib->GetDimensions();
	CxImage *image = new CxImage(imgSize.cx, imgSize.cy, nBitCount, nDocType);
	DWORD paletteSize = image->GetPaletteSize();
	if (paletteSize > 0)
	{
	    RGBQUAD *dstPalette = image->GetPalette();
	    if (dstPalette != NULL)
	    {
		BYTE *srcPalette = ((BYTE *) pDib->m_lpBMIH) + sizeof(BITMAPINFOHEADER);
		memcpy(dstPalette, srcPalette, paletteSize);
	    }
	}
	memcpy(image->GetBits(), pDib->m_lpImage, pDib->GetSizeImage());

	int retval = image->Save(pathName, nDocType);

	if (!retval)
	    AfxMessageBox(image->GetLastError());

	delete image;
	
	return (retval);
}
