; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSetup
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "tekdump.h"
LastPage=0

ClassCount=10
Class1=CMainFrame
Class2=CSetup
Class3=CTekDumpApp
Class4=CTekDumpDoc
Class5=CTekDumpView
Class6=oTTabWnd
Class7=TTabWnd
Class8=TVisualFramework
Class9=TVisualFormView
Class10=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SETUP

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=\ProgWin\TekDump\h\MainFrm.h
ImplementationFile=\ProgWin\TekDump\Src\MainFrm.cpp

[CLS:CSetup]
Type=0
BaseClass=CFormView
HeaderFile=\ProgWin\TekDump\h\Setup.h
ImplementationFile=\ProgWin\TekDump\Src\Setup.cpp
LastObject=IDC_COMBO_BAUDRATE
Filter=D
VirtualFilter=VWC

[CLS:CTekDumpApp]
Type=0
BaseClass=CWinApp
HeaderFile=\ProgWin\TekDump\h\TekDump.h
ImplementationFile=\ProgWin\TekDump\Src\TekDump.cpp

[CLS:CTekDumpDoc]
Type=0
BaseClass=CDocument
HeaderFile=\ProgWin\TekDump\h\TekDumpDoc.h
ImplementationFile=\ProgWin\TekDump\Src\TekDumpDoc.cpp
Filter=N
VirtualFilter=DC
LastObject=ID_FRAME_FIRST

[CLS:CTekDumpView]
Type=0
BaseClass=CScrollView
HeaderFile=\ProgWin\TekDump\h\TekDumpView.h
ImplementationFile=\ProgWin\TekDump\Src\TekDumpView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CTekDumpView

[CLS:oTTabWnd]
Type=0
BaseClass=TTabWnd
HeaderFile=\ProgWin\TekDump\Lib\TabWnd.h
ImplementationFile=\ProgWin\TekDump\Lib\TabWnd.cpp

[CLS:TTabWnd]
Type=0
BaseClass=CWnd
HeaderFile=\ProgWin\TekDump\Lib\VisualFx.h
ImplementationFile=\ProgWin\TekDump\Lib\VisualFx.cpp

[CLS:TVisualFramework]
Type=0
BaseClass=CCmdTarget
HeaderFile=\ProgWin\TekDump\Lib\VisualFx.h
ImplementationFile=\ProgWin\TekDump\Lib\VisualFx.cpp

[CLS:TVisualFormView]
Type=0
BaseClass=CFormView
HeaderFile=\ProgWin\TekDump\Lib\VisualFx.h
ImplementationFile=\ProgWin\TekDump\Lib\VisualFx.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=\ProgWin\TekDump\Src\TekDump.cpp
ImplementationFile=\ProgWin\TekDump\Src\TekDump.cpp
LastObject=CAboutDlg

[DLG:IDD_SETUP]
Type=1
Class=CSetup
ControlCount=19
Control1=IDC_STATIC,static,1342308865
Control2=IDC_IMAGE_COUNT,edit,1350568065
Control3=IDC_STATIC,static,1342308865
Control4=IDC_TOTAL_BYTES,edit,1350568065
Control5=IDC_STATIC,static,1342308865
Control6=IDC_DATASET_SIZE,edit,1350568065
Control7=IDC_STATIC,static,1342308865
Control8=IDC_IMAGE_SIZE,edit,1350568065
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308864
Control11=IDC_IMAGE_PATH,edit,1350631552
Control12=IDC_STATIC,static,1342308864
Control13=IDC_SEQ_NO,edit,1350631552
Control14=IDC_SAVE_SETUP,button,1342242817
Control15=IDC_ABORT_SETUP,button,1342242816
Control16=IDC_STATIC,static,1342177792
Control17=IDC_IMAGE_TYPE,combobox,1344340035
Control18=IDC_COMBO_COMPORT,combobox,1344339971
Control19=IDC_COMBO_BAUDRATE,combobox,1344339971

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_SAVE
Command2=ID_FRAME_PREV
Command3=ID_FRAME_NEXT
Command4=ID_FRAME_PREV10
Command5=ID_FRAME_NEXT10
Command6=ID_FRAME_FIRST
Command7=ID_FRAME_LAST
Command8=ID_FILE_PRINT
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_SAVE
Command2=ID_FILE_MRU_FILE1
Command3=ID_APP_EXIT
Command4=ID_VIEW_TOOLBAR
Command5=ID_VIEW_STATUS_BAR
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_SAVE
Command5=ID_FRAME_NEXT
Command6=ID_FRAME_LAST
Command7=ID_NEXT_PANE
Command8=ID_PREV_PANE
Command9=ID_FRAME_FIRST
Command10=ID_FRAME_NEXT10
Command11=ID_FRAME_PREV10
Command12=ID_FRAME_PREV
CommandCount=12

