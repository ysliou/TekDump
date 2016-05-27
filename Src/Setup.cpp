/** \file	Setup.cpp
    \brief	Setup for THS720 hardcopy reader.

	   Date		Description
	----------  ---------------------------------------------
	2005/08/24  Created.
	2005/08/31  Add image path and sequence number.
	2006/06/08  Add COM port selection.
*/

#include "stdafx.h"

#include "resource.h"
#include "TekDump.h"
#include "TekDumpDoc.h"
#include "TekDumpView.h"
#include "Setup.h"
#include "Ini\ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSetup, CFormView)

BEGIN_MESSAGE_MAP(CSetup, CFormView)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SAVE_SETUP, OnSaveSetup)
	ON_BN_CLICKED(IDC_ABORT_SETUP, OnAbortSetup)
	ON_EN_CHANGE(IDC_IMAGE_PATH, OnSetupChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT, OnSelchangeComboComport)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_SEQ_NO, OnSetupChanged)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, OnSelchangeComboBaudrate)
	ON_WM_SERIAL(OnSerialMsg)
END_MESSAGE_MAP()

CSetup	*g_pSetup = NULL;

struct 
{
    CSerial::EBaudrate	bpsID;
    long    bps;
} baudrateTable[] = {
    {CSerial::EBaud9600,    9600L},
    {CSerial::EBaud19200,   19200L},
    {CSerial::EBaud38400,   38400L},
    {CSerial::EBaud57600,   57600L},
    {CSerial::EBaud115200,  115200L}};

/** \brief  Constructor.
*/
CSetup::CSetup() : CFormView(CSetup::IDD)
{
	m_imagePath = _T("");
	m_seqNo = 0;

	m_statusChanged = 0;
	m_comPortSel = -1;
	m_bpsSel = -1;
	m_errorNo = 0;
	m_index = 0L;
	m_total = 0L;

	m_pBuffer = new char[BUFSIZE]; 

	//  read setup from profile
	CIni ini;
	ini.SetPathName(g_iniName);

	m_imagePath = ini.GetString(_T("Setup"), _T("m_imagePath"), _T("Image"));
	m_seqNo	    = ini.GetUInt(_T("Setup"), _T("m_seqNo"), 0);
	m_comPort   = ini.GetUInt(_T("Setup"), _T("m_comPort"), 1);
	m_bpsSel    = ini.GetUInt(_T("Setup"), _T("m_bpsSel"), 0);
}

/** \brief  Destructor.
*/
CSetup::~CSetup()
{
	CIni ini;
	ini.SetPathName(g_iniName);

	ini.WriteString(_T("Setup"), _T("m_imagePath"), m_imagePath);
	ini.WriteUInt(_T("Setup"), _T("m_seqNo"), m_seqNo);
	ini.WriteUInt(_T("Setup"), _T("m_comPort"), m_comPort);
	ini.WriteUInt(_T("Setup"), _T("m_bpsSel"), m_bpsSel);

	if (m_pBuffer != NULL)
	{
	    delete[] m_pBuffer;
	    m_pBuffer = NULL;
	}

	Close();
}

void	CSetup::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBaudrate);
	DDX_Control(pDX, IDC_COMBO_COMPORT, m_comboComPort);
	DDX_Control(pDX, IDC_SAVE_SETUP, m_btnSave);
	DDX_Control(pDX, IDC_ABORT_SETUP, m_btnAbort);
	DDX_Text(pDX, IDC_IMAGE_PATH, m_imagePath);
	DDX_Text(pDX, IDC_SEQ_NO, m_seqNo);
}

/** \brief  Disable button.
*/
void	CSetup::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	SetButton(FALSE);

	//  COM port
	int nIndex = 0;
	for (int comPort=1; comPort<30; comPort++)
	{
		CString portName;
		portName.Format(_T("\\\\.\\COM%d"), comPort);
		if (CSerial::CheckPort(portName) == CSerial::EPortAvailable)
		{
			portName.Format(_T("COM%d"), comPort);
			m_comboComPort.AddString(portName);
			if (m_comPort == comPort)
				m_comPortSel = nIndex;
			m_allComPort[nIndex++] = comPort;
		}
	}
	if (m_comPortSel >= 0)
		m_comboComPort.SetCurSel(m_comPortSel);

	//  Baudrate
	for (int i=0; i<LENGTH(baudrateTable); i++)
	{
		CString bpsStr;
		bpsStr.Format(_T("%ld"), baudrateTable[i].bps);
		m_comboBaudrate.AddString(bpsStr);
	}
	m_comboBaudrate.SetCurSel(m_bpsSel);

	OpenComPort(m_comPort);

	UpdateData();
}

/** \brief  Show view.
*/
void	CSetup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFormView::OnShowWindow(bShow, nStatus);

	ShowData(IDC_TOTAL_BYTES, _T("0"));
	ShowData(IDC_DATASET_SIZE, _T("0"));
	ShowData(IDC_IMAGE_SIZE, _T("0*0"));
	ShowData(IDC_IMAGE_COUNT, _T("0/0"));
}

/** \brief  Show data fields.
*/
void	CSetup::ShowData(int id, const TCHAR *format, ...)
{
    va_list argptr;
	CString strMsg;

        va_start(argptr, format);       // pre-format messages
        strMsg.FormatV(format, argptr);
        va_end(argptr);
	
	SetDlgItemText(id, strMsg);
}

/** \brief  Setup changed.
*/
void	CSetup::OnSetupChanged() 
{
	SetButton(TRUE);
}

/** \brief  Save setup.
*/
void	CSetup::OnSaveSetup() 
{
	UpdateData();
	SetButton(FALSE);
}

/** \brief  Abort setup.
*/
void	CSetup::OnAbortSetup() 
{
	UpdateData(FALSE);
	SetButton(FALSE);
}

/** \brief  Set button.
*/
void	CSetup::SetButton(BOOL bEnable)
{
	m_btnSave.EnableWindow(bEnable);		
	m_btnAbort.EnableWindow(bEnable);		
}

/** \brief  Change COM port.
*/
void	CSetup::OnSelchangeComboComport() 
{
	m_comPortSel = m_comboComPort.GetCurSel();
	m_comPort = m_allComPort[m_comPortSel];
	OpenComPort(m_comPort);
}

/** \brief  Change baudrate.
*/
void	CSetup::OnSelchangeComboBaudrate() 
{
	m_bpsSel = m_comboBaudrate.GetCurSel();
	OpenComPort(m_comPort);
}

/** \brief  Open COM port.
*/
BOOL	CSetup::OpenComPort(int comPort)
{
	if (IsOpen())
	    Close();

	if (comPort > 0)
	{
	    TCHAR portName[20];
	    if (comPort < 10)
    		_stprintf_s(portName, sizeof(portName), _T("COM%d"), comPort);
	    else
    		_stprintf_s(portName, sizeof(portName), _T("\\\\.\\COM%d"), comPort);

	    Open(portName, this, WM_NULL, 0, 40960, 40960);

	    SetProtocol(baudrateTable[m_bpsSel].bpsID, EParNone, EData8, EStop1, EHandshakeHardware);

	    return TRUE;
	}

	return FALSE;
}

/** \brief  Set SIO protocol.

    \param[in]	eBaudrate   傳輸速率
    \param[in]	eParity	    同位檢查
    \param[in]	eDataBits   資料長度
    \param[in]	eStopBits   停止位元
    \param[in]	eHandshake  通信協定

    \par    
    設定通信協定。
*/ 
int	CSetup::SetProtocol(EBaudrate eBaudrate, EParity eParity, 
			     EDataBits eDataBits, EStopBits eStopBits, EHandshake eHandshake)
{
	int lastError = SetMask(EEventDSR |
				EEventError |
				EEventSend |
				EEventRecv);
	// Setup the COM port
	if (!lastError)
	    lastError = Setup(eBaudrate, eDataBits, eParity, eStopBits);

	// Setup handshaking
	if (!lastError)
	    lastError = SetupHandshaking(eHandshake);

	if (!lastError)
	    lastError = SetupReadTimeouts(EReadTimeoutNonblocking);

	return lastError ? false : TRUE;
}

/** \brief  Process RS-232 I/O.

    \param[in]	wParam	Error code 及 Event
    \param[in]	lParam	對應的 SIO 通道

    \note 此為 Callback function。
*/
LRESULT	CSetup::OnSerialMsg(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	const EError eError = EError(HIWORD(wParam));
	const EEvent eEvent = EEvent(LOWORD(wParam));

//	int portNo = lParam;

	if (eError || (eEvent & EEventError))
	    m_errorNo++;

	if (eEvent & EEventDSR)
	    m_statusChanged++;
#if 0
	if (eEvent & EEventSend)
	    m_sendNo++;
#endif
	if (eEvent & EEventRecv)
	{
		long freeSize;
		DWORD dwRead;
		do
		{
			freeSize = BUFSIZE - m_index;
			Read(&m_pBuffer[m_index], freeSize, &dwRead);
			m_index += dwRead;
			ShowData(IDC_TOTAL_BYTES, _T("%ld"), m_total+m_index);
		} while ((long) dwRead == freeSize);

		SetTimer(SIO_TIMER, 500, NULL);
	}

	return 0;
}

/** \brief  Timer processing.
*/
void	CSetup::OnTimer(UINT nIDEvent) 
{
	CSize imgSize = g_pTekDumpDoc->StoreImage(m_pBuffer, m_index);
	
	if (imgSize != CSize(0, 0))
	{
	    KillTimer(SIO_TIMER);
	    m_total += m_index;
	    m_index = 0;	
	    g_pTekDumpView->RedrawWindow();
	}

	CFormView::OnTimer(nIDEvent);
}
