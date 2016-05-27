#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef __IMAGEIO_H
#define __IMAGEIO_H

#include "resource.h"
#include "ximage.h"

class	CDib;

struct DocType
{
public:
	int nID;
	BOOL bRead;
	BOOL bWrite;
	const char* description;
	const char* ext;
};

CString FindExtension(const CString& name);
int	FindType(const CString& ext);
int	GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
int	GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
CString GetExtFromType(int nDocType);
CString GetDescFromType(int nDocType);
CString GetFileTypes(BOOL bOpenFileDialog);
BOOL	GetWritableType(int nDocType);

BOOL	PromptForFileName(CString& fileName, UINT nIDSTitle, 
		DWORD dwFlags, BOOL bOpenFileDialog, int* pType=NULL);
BOOL	StoreImage(CDib *dib, LPSTR image);
BOOL	ReadImageFile(CDib *pDib, CString pathname="");
BOOL	SaveImageFile(CDib *pDib, CString pathname, int nType=CXIMAGE_FORMAT_BMP, BOOL bReplace=TRUE);

#endif