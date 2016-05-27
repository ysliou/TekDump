#ifndef __SETUP_H
#define __SETUP_H

#pragma once

#define	SIO_TIMER	1

#include "Serial\SerialMFC.h"

#define	LENGTH(x)   (sizeof(x)/sizeof(x[0]))

#define	BUFSIZE		100000

/** \brief  Setup for TekDump.
*/
class CSetup : public CFormView, public CSerialMFC
{
	friend class	CTekDumpDoc;

	DECLARE_DYNCREATE(CSetup)
	enum { IDD = IDD_SETUP };
public:
	CSetup();           // protected constructor used by dynamic creation
	virtual ~CSetup();

	void	ShowData(int id, const TCHAR *format, ...);
	void	SetButton(BOOL bEnable);

protected:
	BOOL	OpenComPort(int comPort);
	int		SetProtocol(EBaudrate eBaudrate = EBaud9600,
			    EParity   eParity   = EParNone,
    			EDataBits eDataBits = EData8,
			    EStopBits eStopBits = EStop1,
			    EHandshake eHandshake = EHandshakeOff);

	int		m_allComPort[30];
	int		m_comPort;			// current COM port
	int		m_comPortSel;		// COM port selection
	int		m_bpsSel;			// baud rate selection

	int		m_statusChanged;	// counter for RS-232 status changed
	int		m_errorNo;			// counter for RS-232 error

	long	m_index;			// current index to receive buffer
	long	m_total;			// total number of bytes received through RS-232

	char	*m_pBuffer;

	CComboBox	m_comboBaudrate;
	CComboBox	m_comboComPort;

	CString	m_imagePath;
	UINT	m_seqNo;
	CButton	m_btnSave;
	CButton	m_btnAbort;

public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSaveSetup();
	afx_msg void OnAbortSetup();
	afx_msg void OnSetupChanged();
	afx_msg void OnSelchangeComboComport();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeComboBaudrate();
	afx_msg LRESULT OnSerialMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

extern	CSetup	*g_pSetup;

#endif
