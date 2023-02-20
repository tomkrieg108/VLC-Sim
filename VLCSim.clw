; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgMain
LastTemplate=CWinThread
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VLCSim.h"
LastPage=0
CDK=Y

ClassCount=21
Class1=CDlgDatetimeMemsig
Class2=CDlgExceptions
Class3=CComsOptions
Class4=CCustom
Class5=gxEditCell
Class6=CEditHex
Class7=CDlgControl
Class8=CGamePlay
Class9=gxListCtrl
Class10=CAboutDlg
Class11=CDlgMain
Class12=CDlgEvents
Class13=CDlgConfiguration
Class14=CDlgMonitor
Class15=CDlgTransactions
Class16=CProgramOptions
Class17=CSDB
Class18=CDlgCommsErrors
Class19=CThreadTest
Class20=CVlcApp
Class21=CReceiveThread

ResourceCount=15
Resource1=IDD_CUSTOM
Resource2=IDD_SDB
Resource3=IDD_GAMEPLAY
Resource4=IDD_COMMS_ERRORS
Resource5=IDD_TRANSACTIONS
Resource6=IDD_EXCEPTIONS
Resource7=IDD_EVENTS
Resource8=IDD_COMMSOPTIONS
Resource9=IDD_MAIN
Resource10=IDD_MONITOR
Resource11=IDD_CONTROL
Resource12=IDD_ABOUTBOX (English (U.S.))
Resource13=IDD_CONFIGURATION
Resource14=IDD_PROGRAMOPTIONS
Resource15=IDD_DATETIME_MEMSIG

[CLS:CDlgDatetimeMemsig]
Type=0
BaseClass=CDialog
HeaderFile=dlg_datetime_memsig.h
ImplementationFile=dlg_datetime_memsig.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_XLISTCTRL_DATETIME_MEMSIG_LIST

[CLS:CDlgExceptions]
Type=0
BaseClass=CDialog
HeaderFile=dlg_exceptions.h
ImplementationFile=dlg_exceptions.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_CHECK_SEND_PENDING

[CLS:CDlgCommsErrors]
Type=0
BaseClass=CDialog
HeaderFile=dlg_comms_errors.h
ImplementationFile=dlg_comms_errors.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_CHECK_N_BITS


[CLS:CComsOptions]
Type=0
BaseClass=CDialog
HeaderFile=coms_options.h
ImplementationFile=coms_options.cpp
LastObject=CComsOptions

[CLS:CCustom]
Type=0
BaseClass=CDialog
HeaderFile=custom_dlg.h
ImplementationFile=custom_dlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CCustom

[CLS:gxEditCell]
Type=0
BaseClass=CEdit
HeaderFile=edit_cell.h
ImplementationFile=edit_cell.cpp

[CLS:CEditHex]
Type=0
BaseClass=CEdit
HeaderFile=edit_hex.h
ImplementationFile=edit_hex.cpp
LastObject=CEditHex

[CLS:CDlgControl]
Type=0
BaseClass=CDialog
HeaderFile=dlg_control.h
ImplementationFile=dlg_control.cpp
LastObject=CDlgControl
Filter=D
VirtualFilter=dWC

[CLS:CGamePlay]
Type=0
BaseClass=CDialog
HeaderFile=game_play_dlg.h
ImplementationFile=game_play_dlg.cpp
LastObject=CGamePlay
Filter=D
VirtualFilter=dWC

[CLS:gxListCtrl]
Type=0
BaseClass=CListCtrl
HeaderFile=list_ctrl.h
ImplementationFile=list_ctrl.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=main_dlg.cpp
ImplementationFile=main_dlg.cpp
LastObject=CAboutDlg

[CLS:CDlgMain]
Type=0
BaseClass=CDialog
HeaderFile=dlg_main.h
ImplementationFile=dlg_main.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMMS_WATCHER

[CLS:CDlgEvents]
Type=0
BaseClass=CDialog
HeaderFile=dlg_events.h
ImplementationFile=dlg_events.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_APPLY_ERRORS_TO

[CLS:CDlgConfiguration]
Type=0
BaseClass=CDialog
HeaderFile=dlg_configuration.h
ImplementationFile=dlg_configuration.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_CHECK_NACK_1ST_PKT

[CLS:CDlgMonitor]
Type=0
BaseClass=CDialog
HeaderFile=dlg_monitor.h
ImplementationFile=dlg_monitor.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_GAME_LIST

[CLS:CDlgTransactions]
Type=0
BaseClass=CDialog
HeaderFile=dlg_transactions.h
ImplementationFile=dlg_transactions.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_BTN_SEND_ALL_IDS

[CLS:CProgramOptions]
Type=0
BaseClass=CDialog
HeaderFile=program_options_dlg.h
ImplementationFile=program_options_dlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CProgramOptions

[CLS:CSDB]
Type=0
BaseClass=CDialog
HeaderFile=sdb_dlg.h
ImplementationFile=sdb_dlg.cpp
LastObject=IDC_BCV
Filter=D

[CLS:CVlcApp]
Type=0
BaseClass=CWinApp
HeaderFile=VLCSim.h
ImplementationFile=VLCSim.cpp

[DLG:IDD_DATETIME_MEMSIG]
Type=1
Class=CDlgDatetimeMemsig
ControlCount=2
Control1=IDC_XLISTCTRL_DATETIME_MEMSIG_PKT,SysListView32,1350631425
Control2=IDC_CHK_USE_CURRENT_DATETIME,button,1342275587

[DLG:IDD_COMMSOPTIONS]
Type=1
Class=CComsOptions
ControlCount=1
Control1=IDC_STATIC,button,1342210055

[DLG:IDD_CUSTOM]
Type=1
Class=CCustom
ControlCount=3
Control1=IDC_STATIC,button,1342210055
Control2=IDC_STATIC,button,1342210055
Control3=IDC_STATIC,button,1342210055

[DLG:IDD_GAMEPLAY]
Type=1
Class=CGamePlay
ControlCount=51
Control1=IDC_STATIC,button,1342210055
Control2=IDC_ADD_100,button,1342275584
Control3=IDC_ADD_50,button,1342275584
Control4=IDC_ADD_20,button,1342275584
Control5=IDC_ADD_10,button,1342275584
Control6=IDC_ADD_5,button,1342275584
Control7=IDC_BUTTON6,button,1342275584
Control8=IDC_ADD_1_HOP,button,1342275584
Control9=IDC_ADD_1_CB,button,1342275584
Control10=IDC_STATIC,button,1342210055
Control11=IDC_HOPPER_OUT,button,1342275584
Control12=IDC_CANCEL_CR,button,1342275584
Control13=IDC_CONFIRM,button,1208057856
Control14=IDC_STATIC,button,1342210055
Control15=IDC_BET,edit,1342251008
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_WIN,edit,1342251008
Control19=IDC_PLAY,button,1342275584
Control20=IDC_DURATION,edit,1342251008
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,button,1342210055
Control23=IDC_SAP_1,button,1342275587
Control24=IDC_SAP_2,button,1342275587
Control25=IDC_SAP_3,button,1342275587
Control26=IDC_SAP_4,button,1342275587
Control27=IDC_STATIC,button,1342210055
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_MIN_BET,edit,1342251008
Control31=IDC_MAX_BET,edit,1342251008
Control32=IDC_START,button,1342275584
Control33=IDC_STATUS_1,static,1342308352
Control34=IDC_STATIC,button,1342210055
Control35=IDC_LP_1,button,1342275587
Control36=IDC_LP_2,button,1342275587
Control37=IDC_LP_3,button,1342275587
Control38=IDC_LP_4,button,1342275587
Control39=IDC_PROG_RESET,button,1208057856
Control40=IDC_STATUS_2,static,1342308352
Control41=IDC_STATUS_3,static,1342308352
Control42=IDC_STATIC,button,1342210055
Control43=IDC_DOOR_MAIN,button,1342275587
Control44=IDC_DOOR_SECURITY,button,1342275587
Control45=IDC_ERROR_SELF_AUDIT,button,1342275587
Control46=IDC_ERROR_MEMORY,button,1342275587
Control47=IDC_AUDIT_MODE,button,1342275587
Control48=IDC_SEF,button,1342275587
Control49=IDC_POWER_DOWN,button,1342275587
Control50=IDC_CANCEL,button,1208057856
Control51=IDC_DISPLAY_ERROR,button,1342275587

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_CONFIGURATION]
Type=1
Class=CDlgConfiguration
ControlCount=4
Control1=IDC_XLISTCTRL_CONFIG_PACKET,SysListView32,1350631425
Control2=IDC_CHECK_O_BIT,button,1342275587
Control3=IDC_CHECK_NACK_1ST_PKT,button,1342275587
Control4=IDC_CHECK_NACK_2ND_PKT,button,1342275587

[DLG:IDD_MONITOR]
Type=1
Class=CDlgMonitor
ControlCount=24
Control1=IDC_XLISTCTRL_MONITOR_PACKET,SysListView32,1350631425
Control2=IDC_COMBO_CUR_DE,combobox,1344339971
Control3=IDC_BUTTON_HOPPER_IN,button,1342275584
Control4=IDC_BUTTON_DROP_BOX_IN,button,1342275584
Control5=IDC_BUTTON_HOPPER_OUT,button,1342275584
Control6=IDC_BUTTON_MANUAL_PAY,button,1342275584
Control7=IDC_BUTTON_COLLECT_DROP_BOX,button,1342275584
Control8=IDC_BUTTON_PLAY_GAME,button,1342275584
Control9=IDC_COMBO_GAME_LIST,combobox,1344339971
Control10=IDC_EDIT_HOPPER_IN,edit,1342251136
Control11=IDC_EDIT_DROP_BOX_IN,edit,1342251136
Control12=IDC_EDIT_HOPPER_OUT,edit,1342251136
Control13=IDC_EDIT_MANUAL_PAY,edit,1342251136
Control14=IDC_EDIT_COLLECT_DROP,edit,1342251136
Control15=IDC_EDIT_BET,edit,1342251136
Control16=IDC_EDIT_WIN,edit,1342251136
Control17=IDC_STATIC_CREDITS,static,1342308352
Control18=IDC_BUTTON_SNAPSHOT,button,1342275584
Control19=IDC_STATIC,button,1342210055
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,button,1342210055
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352

[DLG:IDD_TRANSACTIONS]
Type=1
Class=CDlgTransactions
ControlCount=6
Control1=IDC_LISTCTRL_TRANS_REQUEST,SysListView32,1350631425
Control2=IDC_LISTCTRL_TRANS_REPLY,SysListView32,1350631425
Control3=IDC_BUTTON_SEND,button,1342275584
Control4=IDC_STATIC,button,1342210055
Control5=IDC_STATIC,button,1342210055
Control6=IDC_BTN_SEND_ALL_IDS,button,1342275584

[DLG:IDD_PROGRAMOPTIONS]
Type=1
Class=CProgramOptions
ControlCount=4
Control1=IDC_STATIC,button,1342210055
Control2=IDC_STATIC,button,1342210055
Control3=IDC_STATIC,button,1342210055
Control4=IDC_STATIC,button,1342210055

[DLG:IDD_SDB]
Type=1
Class=CSDB
ControlCount=123
Control1=IDC_STATUS1_1,button,1342275587
Control2=IDC_STATUS2_1,button,1342275587
Control3=IDC_STATUS3_1,button,1342275587
Control4=IDC_STATUS4_1,button,1342275587
Control5=IDC_STATUS5_1,button,1342275587
Control6=IDC_STATUS6_1,button,1342275587
Control7=IDC_STATUS7_1,button,1342275587
Control8=IDC_STATUS8_1,button,1342275587
Control9=IDC_STATUS1_2,button,1342275587
Control10=IDC_STATUS2_2,button,1342275587
Control11=IDC_STATUS3_2,button,1342275587
Control12=IDC_STATUS4_2,button,1342275587
Control13=IDC_STATUS5_2,button,1342275587
Control14=IDC_STATUS6_2,button,1342275587
Control15=IDC_STATUS7_2,button,1342275587
Control16=IDC_STATUS8_2,button,1342275587
Control17=IDC_STATUS1_3,button,1342275587
Control18=IDC_STATUS2_3,button,1342275587
Control19=IDC_STATUS3_3,button,1342275587
Control20=IDC_STATUS4_3,button,1342275587
Control21=IDC_STATUS5_3,button,1342275587
Control22=IDC_STATUS6_3,button,1342275587
Control23=IDC_STATUS7_3,button,1342275587
Control24=IDC_STATUS8_3,button,1342275587
Control25=IDC_STATUS1_4,button,1342275587
Control26=IDC_STATUS2_4,button,1342275587
Control27=IDC_STATUS3_4,button,1342275587
Control28=IDC_STATUS4_4,button,1342275587
Control29=IDC_STATUS5_4,button,1342275587
Control30=IDC_STATUS6_4,button,1342275587
Control31=IDC_STATUS7_4,button,1342275587
Control32=IDC_STATUS8_4,button,1342275587
Control33=IDC_STATUS1_5,button,1342275587
Control34=IDC_STATUS2_5,button,1342275587
Control35=IDC_STATUS3_5,button,1342275587
Control36=IDC_STATUS4_5,button,1342275587
Control37=IDC_STATUS5_5,button,1342275587
Control38=IDC_STATUS6_5,button,1342275587
Control39=IDC_STATUS7_5,button,1342275587
Control40=IDC_STATUS8_5,button,1342275587
Control41=IDC_P1SEF,button,1342275587
Control42=IDC_P2SEF,button,1342275587
Control43=IDC_P3SEF,button,1342275587
Control44=IDC_P4SEF,button,1342275587
Control45=IDC_P5SEF,button,1342275587
Control46=IDC_P6SEF,button,1342275587
Control47=IDC_GMID,edit,1342251008
Control48=IDC_PGMID1,edit,1342242816
Control49=IDC_PGMID2,edit,1342242816
Control50=IDC_PGMID3,edit,1342242816
Control51=IDC_PGMID4,edit,1342242816
Control52=IDC_PRTPH,edit,1342251010
Control53=IDC_BCV,edit,1342251010
Control54=IDC_SDBVER,edit,1342251010
Control55=IDC_MGMID,edit,1342251010
Control56=IDC_TURNOVER,edit,1342251018
Control57=IDC_TOTALWINS,edit,1342251018
Control58=IDC_CASHBOX,edit,1342251018
Control59=IDC_CANCELLEDCR,edit,1342251018
Control60=IDC_MISCACC,edit,1342251018
Control61=IDC_MONEYIN,edit,1342251018
Control62=IDC_MONEYOUT,edit,1342251018
Control63=IDC_CASHIN,edit,1342251018
Control64=IDC_CASHOUT,edit,1342251018
Control65=IDC_CREDIT,edit,1342251018
Control66=IDC_PLAYED,edit,1342251018
Control67=IDC_POWERUP,edit,1342251018
Control68=IDC_GAMESPOWER,edit,1342251018
Control69=IDC_GAMESDOOR,edit,1342251018
Control70=IDC_BSCCORRECT,button,1342275587
Control71=IDC_BSC,edit,1342251010
Control72=IDC_CHKSUMCORRECT,button,1342275587
Control73=IDC_CHKSUM,edit,1342242816
Control74=IDC_STATIC,static,1342308864
Control75=IDC_STATIC,static,1342308864
Control76=IDC_STATIC,static,1342308864
Control77=IDC_STATIC,static,1342308864
Control78=IDC_STATIC,static,1342308864
Control79=IDC_STATIC,static,1342308864
Control80=IDC_STATIC,button,1342341127
Control81=IDC_STATIC,button,1342341127
Control82=IDC_STATIC,static,1342308352
Control83=IDC_STATIC,static,1342308352
Control84=IDC_STATIC,static,1342308352
Control85=IDC_STATIC,static,1342308352
Control86=IDC_STATIC,static,1342308352
Control87=IDC_STATIC,static,1342308352
Control88=IDC_STATUSDISPLAY1,edit,1342244992
Control89=IDC_STATUSDISPLAY2,edit,1342244992
Control90=IDC_STATUSDISPLAY3,edit,1342244992
Control91=IDC_STATUSDISPLAY4,edit,1342244992
Control92=IDC_STATUSDISPLAY5,edit,1342244992
Control93=IDC_STATUSDISPLAY6,edit,1342244992
Control94=IDC_STATIC,button,1342210055
Control95=IDC_STATIC,button,1342210055
Control96=IDC_STATIC,static,1342308864
Control97=IDC_STATIC,static,1342308864
Control98=IDC_STATIC,static,1342308864
Control99=IDC_STATIC,static,1342308864
Control100=IDC_STATIC,static,1342308864
Control101=IDC_STATIC,static,1342308864
Control102=IDC_STATIC,static,1342308864
Control103=IDC_STATIC,static,1342308864
Control104=IDC_STATIC,static,1342308864
Control105=IDC_STATIC,static,1342308864
Control106=IDC_STATIC,static,1342308864
Control107=IDC_STATIC,static,1342308864
Control108=IDC_STATIC,static,1342308864
Control109=IDC_STATIC,static,1342308864
Control110=IDC_STATIC,static,1342308864
Control111=IDC_STATIC,button,1342210055
Control112=IDC_STATIC,static,1342308864
Control113=IDC_STATIC,static,1342308864
Control114=IDC_STATIC,static,1342308864
Control115=IDC_STATIC,static,1342308864
Control116=IDC_STATIC,static,1342308864
Control117=IDC_STATIC,static,1342308864
Control118=IDC_STATIC,static,1342308864
Control119=IDC_STATIC,button,1342210055
Control120=IDC_STATIC,static,1342308864
Control121=IDC_STATIC,static,1342308864
Control122=IDC_SFS,edit,1342242818
Control123=IDC_STATIC,static,1342308864

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342406657

[CLS:CReceiveThread]
Type=0
HeaderFile=receive_thread.h
ImplementationFile=receive_thread.cpp
BaseClass=CWinThread
Filter=N
LastObject=CReceiveThread

[CLS:CThreadTest]
Type=0
HeaderFile=ThreadTest.h
ImplementationFile=ThreadTest.cpp
BaseClass=CWinThread
Filter=N
LastObject=CThreadTest

[DLG:IDD_COMMS_ERRORS]
Type=1
Class=CDlgCommsErrors
ControlCount=37
Control1=IDC_STATIC,button,1342210055
Control2=IDC_EDIT_POLL_RESPONSE_TIME,edit,1342251136
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_INTER_CHAR_TIME,edit,1342251136
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_DATA_READ_RESPONSE_TIME,edit,1342251136
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_RTS_DISABLE_TIME,edit,1342251136
Control9=IDC_STATIC,static,1342308352
Control10=IDC_CHECK_DONT_ENABLE_RTS,button,1342275587
Control11=IDC_CHECK_KEEP_RTP_ENABLED,button,1342275587
Control12=IDC_CHECK_CORRUPT_HEADER,button,1342275587
Control13=IDC_CHECK_CORRUPT_TRAILER,button,1342275587
Control14=IDC_CHECK_CORRUPT_CRC,button,1342275587
Control15=IDC_CHECK_CORRUPT_VALIDATION_CODE,button,1342275587
Control16=IDC_CHECK_CORRUPT_TERMINAL_ID,button,1342275587
Control17=IDC_STATIC,button,1342210055
Control18=IDC_CHECK_CORRUPT_COMMAND,button,1342275587
Control19=IDC_CHECK_CORRUPT_DATA,button,1342275587
Control20=IDC_STATIC,button,1342210055
Control21=IDC_EDIT_NUM_BYTES,edit,1342251136
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,button,1342210055
Control24=IDC_COMBO_NUMBER_OF_BREAKS,combobox,1344340227
Control25=IDC_STATIC,static,1342308352
Control26=IDC_CHECK_CORRUPT_VALIDATION_DATETIME,button,1342275587
Control27=IDC_COMBO_APPLY_PKT_TYPE,combobox,1344339971
Control28=IDC_COMBO_DATA_SIZE_ERRORS,combobox,1344339970
Control29=IDC_COMBO_FILL_TYPE,combobox,1344339970
Control30=IDC_CHECK_NEXT_READ_RESPONSE,button,1342275587
Control31=IDC_CHECK_NEXT_ACK_NACK,button,1342275587
Control32=IDC_BUTTON_SET_ERRORS,button,1342275584
Control33=IDC_STATIC_ENABLE,button,1342210055
Control34=IDC_CHECK_ENABLE_TIMING_ER,button,1342275587
Control35=IDC_CHECK_ENABLE_DATA_ER,button,1342275587
Control36=IDC_CHECK_ENABLE_SIZE_ER,button,1342275587
Control37=IDC_CHECK_N_BITS,button,1342275587

[DLG:IDD_EXCEPTIONS]
Type=1
Class=CDlgExceptions
ControlCount=5
Control1=IDC_CHECK_SEND_PENDING,button,1342275587
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_XLISTCTRL_EXCEPTIONS,SysListView32,1350631425
Control5=IDC_LISTCTRL_PENDING,SysListView32,1350631425

[DLG:IDD_MAIN]
Type=1
Class=CDlgMain
ControlCount=51
Control1=IDC_COMPORTLIST,SysListView32,1342259230
Control2=IDC_CONNECT,button,1342279683
Control3=IDC_CHILDWINDOWTREE,SysTreeView32,1342242851
Control4=IDC_PLACEHOLDER,static,1082261504
Control5=IDOK,button,1342275584
Control6=IDC_TITLE,static,1082261504
Control7=IDC_MSCOMMCTRL,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control8=IDC_SEF,edit,1342248965
Control9=IDC_RECEIVE_DEBUG,static,1342308352
Control10=IDC_COMMS_WATCHER,listbox,1353777505
Control11=IDC_MSCOMM2,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control12=IDC_STATIC_ACTIVE_ID,static,1342308352
Control13=IDC_XLISTCTRL_POLLING,SysListView32,1350631437
Control14=IDC_BTN_ADD_ID,button,1342275584
Control15=IDC_EDIT_ADD_ID,edit,1350631552
Control16=IDC_BTN_DATA_SET_NEXT,button,1342275584
Control17=IDC_STATIC,button,1342210055
Control18=IDC_BTN_DATA_VIEW_NEXT,button,1342275584
Control19=IDC_BTN_DATA_VIEW_DEFAULT,button,1342275584
Control20=IDC_STATIC,button,1342210055
Control21=IDC_CHK_ERRORS_TIMING,button,1342275587
Control22=IDC_CHK_ERRORS_DATA,button,1342275587
Control23=IDC_CHK_ERRORS_SIZE,button,1342275587
Control24=IDC_BTN_ERRORS_SET,button,1342275584
Control25=IDC_STATIC,button,1342210055
Control26=IDC_COMBO_NEXT_RESPONSE,combobox,1344340034
Control27=IDC_BTN_NEXT_RESPONSE_SET,button,1342275584
Control28=IDC_STATIC_DATE,static,1342308352
Control29=IDC_STATIC_TIME,static,1342308352
Control30=IDC_STATIC,button,1342210055
Control31=IDC_CHECK_PAUSE,button,1342275587
Control32=IDC_CHECK_ACTIVE_ID_ONLY,button,1342275587
Control33=IDC_CHECK_INCLUDE_DESCRIPTION,button,1342275587
Control34=IDC_CHECK_INCLUDE_ERRORS,button,1342275587
Control35=IDC_CHECK_LOG,button,1342275587
Control36=IDC_STATIC_STATUS,static,1342308352
Control37=IDC_BTN_DATA_VIEW_LAST_READ_REPLY,button,1342275584
Control38=IDC_BTN_DATA_VIEW_LAST_SITE_REPLY,button,1342275584
Control39=IDC_BTN_DATA_VIEW_LAST_RECEIVED,button,1342275584
Control40=IDC_CHECK_SHOW_POLLS,button,1342275587
Control41=IDC_STATIC,button,1342210055
Control42=IDC_BTN_ERRORS_VIEW,button,1342275584
Control43=IDC_STATIC,button,1342210055
Control44=IDC_STATIC,button,1342210055
Control45=IDC_STATIC,static,1342308352
Control46=IDC_STATIC_ID_STATUS,button,1342210055
Control47=IDC_STATIC_TERMINAL_MODE,static,1342308352
Control48=IDC_CHECK_SET_NOT_RESPONDING,button,1342275587
Control49=IDC_STATIC,static,1342308352
Control50=IDC_CHECK_NON_RESPONDING_READ_REQ,button,1342275587
Control51=IDC_CHECK_NO_RESP_WRITE_REQ,button,1342275587

[DLG:IDD_CONTROL]
Type=1
Class=CDlgControl
ControlCount=1
Control1=IDC_XLISTCTRL_CONTROL_PKT,SysListView32,1350631425

[DLG:IDD_EVENTS]
Type=1
Class=CDlgEvents
ControlCount=15
Control1=IDC_LISTCTRL_EVENT_DATA,SysListView32,1350631425
Control2=IDC_LISTCTRL_EVENT_LOG,SysListView32,1350631425
Control3=IDC_COMBO_EVENT_TYPES,combobox,1344339970
Control4=IDC_BTN_ADD,button,1342275584
Control5=IDC_STATIC,button,1342210055
Control6=IDC_STATIC,button,1342210055
Control7=IDC_CHK_USE_CURRENT_TIME,button,1342275587
Control8=IDC_STATIC_END_READ,static,1342308352
Control9=IDC_STATIC_START_READ,static,1342308352
Control10=IDC_BTN_SET_DEFAULT,button,1342275584
Control11=IDC_STATIC,button,1342210055
Control12=IDC_STATIC_RECORDS,static,1342308352
Control13=IDC_STATIC_PACKETS,static,1342308352
Control14=IDC_COMBO_APPLY_ERRORS_TO,combobox,1344339971
Control15=IDC_STATIC,static,1342308352
