// SDB.cpp : implementation file
//

#include "stdafx.h"
#include "VLCSim.h"
#include "utilities.h"
#include "sdb_dlg.h"
#include "program_options_dlg.h"
#include "math.h"
//#include "xseries_packet.h"
#include "game_play.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSDB* g_sdb_dlg = 0;

/////////////////////////////////////////////////////////////////////////////
// CSDB dialog


CSDB::CSDB(CWnd* pParent /*=NULL*/)
	: CDialog(CSDB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSDB)
	m_ChkGood = FALSE;
	m_Checksum = _T("");
	m_BSCCorrect = TRUE;
	m_CancelledCredit = _T("");
	m_CashBox = _T("");
	m_CashIn = _T("");
	m_CashOut = _T("");
	m_Credit = _T("");
	m_MiscellaneousAccrual = _T("");
	m_MoneyIn = _T("");
	m_MoneyOut = _T("");
	m_P1SEF = FALSE;
	m_P2SEF = FALSE;
	m_P3SEF = FALSE;
	m_P4SEF = FALSE;
	m_P5SEF = FALSE;
	m_P6SEF = FALSE;
	m_ProgramID1 = _T("");
	m_ProgramID2 = _T("");
	m_ProgramID3 = _T("");
	m_ProgramID4 = _T("");
	m_TotalWins = _T("");
	m_Turnover = _T("");
	m_Status1_1 = FALSE;
	m_Status1_2 = FALSE;
	m_Status2_1 = FALSE;
	m_Status1_3 = FALSE;
	m_Status1_4 = FALSE;
	m_Status1_5 = FALSE;
	m_Status2_2 = FALSE;
	m_Status2_3 = FALSE;
	m_Status2_4 = FALSE;
	m_Status2_5 = FALSE;
	m_Status3_1 = FALSE;
	m_Status3_2 = FALSE;
	m_Status3_3 = FALSE;
	m_Status3_4 = FALSE;
	m_Status3_5 = FALSE;
	m_Status4_1 = FALSE;
	m_Status4_2 = FALSE;
	m_Status4_3 = FALSE;
	m_Status4_4 = FALSE;
	m_Status4_5 = FALSE;
	m_Status5_1 = FALSE;
	m_Status5_2 = FALSE;
	m_Status5_3 = FALSE;
	m_Status5_4 = FALSE;
	m_Status5_5 = FALSE;
	m_Status6_1 = FALSE;
	m_Status6_2 = FALSE;
	m_Status6_3 = FALSE;
	m_Status6_4 = FALSE;
	m_Status6_5 = FALSE;
	m_Status7_1 = FALSE;
	m_Status7_2 = FALSE;
	m_Status7_3 = FALSE;
	m_Status7_4 = FALSE;
	m_Status7_5 = FALSE;
	m_Status8_1 = FALSE;
	m_Status8_2 = FALSE;
	m_Status8_3 = FALSE;
	m_Status8_4 = FALSE;
	m_Status8_5 = FALSE;
	m_StatusDisplayString1 = _T("00");
	m_StatusDisplayString2 = _T("00");
	m_StatusDisplayString3 = _T("00");
	m_StatusDisplayString4 = _T("00");
	m_StatusDisplayString5 = _T("00");
	m_StatusDisplayString6 = _T("3F");
	m_bStatus1 = 0;
	m_bStatus2 = 0;
	m_bStatus3 = 0;
	m_bStatus4 = 0;
	m_bStatus5 = 0;
	m_bStatusSEF = 63;
	m_gmid = _T("");
	m_sdb_vers = _T("102");
	m_rtp = _T("9025");
	m_bcv = _T("2");
	m_mgmid = _T("0");
	m_games_door = _T("");
	m_games_power = _T("");
	m_games_played = _T("");
	m_power_up = _T("");
	m_seq_ctr = _T("");
	//}}AFX_DATA_INIT

	//g_notify_mgr->Register( CNotify::SENT_SDB, this );
}


void CSDB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSDB)
	DDX_Control(pDX, IDC_SFS, m_sfs);
	DDX_Control(pDX, IDC_BSC, m_BSCCtrl);
	DDX_Control(pDX, IDC_CHKSUM, m_ChkSum);
	DDX_Check(pDX, IDC_CHKSUMCORRECT, m_ChkGood);
	DDX_Text(pDX, IDC_CHKSUM, m_Checksum);
	DDV_MaxChars(pDX, m_Checksum, 3);
	DDX_Check(pDX, IDC_BSCCORRECT, m_BSCCorrect);
	DDX_Text(pDX, IDC_CANCELLEDCR, m_CancelledCredit);
	DDV_MaxChars(pDX, m_CancelledCredit, 10);
	DDX_Text(pDX, IDC_CASHBOX, m_CashBox);
	DDV_MaxChars(pDX, m_CashBox, 10);
	DDX_Text(pDX, IDC_CASHIN, m_CashIn);
	DDV_MaxChars(pDX, m_CashIn, 10);
	DDX_Text(pDX, IDC_CASHOUT, m_CashOut);
	DDV_MaxChars(pDX, m_CashOut, 10);
	DDX_Text(pDX, IDC_CREDIT, m_Credit);
	DDV_MaxChars(pDX, m_Credit, 10);
	DDX_Text(pDX, IDC_MISCACC, m_MiscellaneousAccrual);
	DDV_MaxChars(pDX, m_MiscellaneousAccrual, 10);
	DDX_Text(pDX, IDC_MONEYIN, m_MoneyIn);
	DDV_MaxChars(pDX, m_MoneyIn, 10);
	DDX_Text(pDX, IDC_MONEYOUT, m_MoneyOut);
	DDV_MaxChars(pDX, m_MoneyOut, 10);
	DDX_Check(pDX, IDC_P1SEF, m_P1SEF);
	DDX_Check(pDX, IDC_P2SEF, m_P2SEF);
	DDX_Check(pDX, IDC_P3SEF, m_P3SEF);
	DDX_Check(pDX, IDC_P4SEF, m_P4SEF);
	DDX_Check(pDX, IDC_P5SEF, m_P5SEF);
	DDX_Check(pDX, IDC_P6SEF, m_P6SEF);
	DDX_Text(pDX, IDC_PGMID1, m_ProgramID1);
	DDV_MaxChars(pDX, m_ProgramID1, 8);
	DDX_Text(pDX, IDC_PGMID2, m_ProgramID2);
	DDV_MaxChars(pDX, m_ProgramID2, 8);
	DDX_Text(pDX, IDC_PGMID3, m_ProgramID3);
	DDV_MaxChars(pDX, m_ProgramID3, 8);
	DDX_Text(pDX, IDC_PGMID4, m_ProgramID4);
	DDV_MaxChars(pDX, m_ProgramID4, 8);
	DDX_Text(pDX, IDC_TOTALWINS, m_TotalWins);
	DDV_MaxChars(pDX, m_TotalWins, 10);
	DDX_Text(pDX, IDC_TURNOVER, m_Turnover);
	DDV_MaxChars(pDX, m_Turnover, 10);
	DDX_Check(pDX, IDC_STATUS1_1, m_Status1_1);
	DDX_Check(pDX, IDC_STATUS1_2, m_Status1_2);
	DDX_Check(pDX, IDC_STATUS2_1, m_Status2_1);
	DDX_Check(pDX, IDC_STATUS1_3, m_Status1_3);
	DDX_Check(pDX, IDC_STATUS1_4, m_Status1_4);
	DDX_Check(pDX, IDC_STATUS1_5, m_Status1_5);
	DDX_Check(pDX, IDC_STATUS2_2, m_Status2_2);
	DDX_Check(pDX, IDC_STATUS2_3, m_Status2_3);
	DDX_Check(pDX, IDC_STATUS2_4, m_Status2_4);
	DDX_Check(pDX, IDC_STATUS2_5, m_Status2_5);
	DDX_Check(pDX, IDC_STATUS3_1, m_Status3_1);
	DDX_Check(pDX, IDC_STATUS3_2, m_Status3_2);
	DDX_Check(pDX, IDC_STATUS3_3, m_Status3_3);
	DDX_Check(pDX, IDC_STATUS3_4, m_Status3_4);
	DDX_Check(pDX, IDC_STATUS3_5, m_Status3_5);
	DDX_Check(pDX, IDC_STATUS4_1, m_Status4_1);
	DDX_Check(pDX, IDC_STATUS4_2, m_Status4_2);
	DDX_Check(pDX, IDC_STATUS4_3, m_Status4_3);
	DDX_Check(pDX, IDC_STATUS4_4, m_Status4_4);
	DDX_Check(pDX, IDC_STATUS4_5, m_Status4_5);
	DDX_Check(pDX, IDC_STATUS5_1, m_Status5_1);
	DDX_Check(pDX, IDC_STATUS5_2, m_Status5_2);
	DDX_Check(pDX, IDC_STATUS5_3, m_Status5_3);
	DDX_Check(pDX, IDC_STATUS5_4, m_Status5_4);
	DDX_Check(pDX, IDC_STATUS5_5, m_Status5_5);
	DDX_Check(pDX, IDC_STATUS6_1, m_Status6_1);
	DDX_Check(pDX, IDC_STATUS6_2, m_Status6_2);
	DDX_Check(pDX, IDC_STATUS6_3, m_Status6_3);
	DDX_Check(pDX, IDC_STATUS6_4, m_Status6_4);
	DDX_Check(pDX, IDC_STATUS6_5, m_Status6_5);
	DDX_Check(pDX, IDC_STATUS7_1, m_Status7_1);
	DDX_Check(pDX, IDC_STATUS7_2, m_Status7_2);
	DDX_Check(pDX, IDC_STATUS7_3, m_Status7_3);
	DDX_Check(pDX, IDC_STATUS7_4, m_Status7_4);
	DDX_Check(pDX, IDC_STATUS7_5, m_Status7_5);
	DDX_Check(pDX, IDC_STATUS8_1, m_Status8_1);
	DDX_Check(pDX, IDC_STATUS8_2, m_Status8_2);
	DDX_Check(pDX, IDC_STATUS8_3, m_Status8_3);
	DDX_Check(pDX, IDC_STATUS8_4, m_Status8_4);
	DDX_Check(pDX, IDC_STATUS8_5, m_Status8_5);
	DDX_Text(pDX, IDC_STATUSDISPLAY1, m_StatusDisplayString1);
	DDX_Text(pDX, IDC_STATUSDISPLAY2, m_StatusDisplayString2);
	DDX_Text(pDX, IDC_STATUSDISPLAY3, m_StatusDisplayString3);
	DDX_Text(pDX, IDC_STATUSDISPLAY4, m_StatusDisplayString4);
	DDX_Text(pDX, IDC_STATUSDISPLAY5, m_StatusDisplayString5);
	DDX_Text(pDX, IDC_STATUSDISPLAY6, m_StatusDisplayString6);
	DDX_Control(pDX, IDC_STATUSDISPLAY6, m_StatusDisplayCtrl6);
	DDX_Control(pDX, IDC_STATUSDISPLAY5, m_StatusDisplayCtrl5);
	DDX_Control(pDX, IDC_STATUSDISPLAY4, m_StatusDisplayCtrl4);
	DDX_Control(pDX, IDC_STATUSDISPLAY3, m_StatusDisplayCtrl3);
	DDX_Control(pDX, IDC_STATUSDISPLAY2, m_StatusDisplayCtrl2);
	DDX_Control(pDX, IDC_STATUSDISPLAY1, m_StatusDisplayCtrl1);
	DDX_Text(pDX, IDC_GMID, m_gmid);
	DDV_MaxChars(pDX, m_gmid, 6);
	DDX_Text(pDX, IDC_SDBVER, m_sdb_vers);
	DDV_MaxChars(pDX, m_sdb_vers, 4);
	DDX_Text(pDX, IDC_PRTPH, m_rtp);
	DDV_MaxChars(pDX, m_rtp, 4);
	DDX_Text(pDX, IDC_BCV, m_bcv);
	DDV_MaxChars(pDX, m_bcv, 3);
	DDX_Text(pDX, IDC_MGMID, m_mgmid);
	DDV_MaxChars(pDX, m_mgmid, 4);
	DDX_Text(pDX, IDC_GAMESDOOR, m_games_door);
	DDV_MaxChars(pDX, m_games_door, 8);
	DDX_Text(pDX, IDC_GAMESPOWER, m_games_power);
	DDV_MaxChars(pDX, m_games_power, 8);
	DDX_Text(pDX, IDC_PLAYED, m_games_played);
	DDV_MaxChars(pDX, m_games_played, 8);
	DDX_Text(pDX, IDC_POWERUP, m_power_up);
	DDV_MaxChars(pDX, m_power_up, 8);
	DDX_Text(pDX, IDC_BSC, m_seq_ctr);
	DDV_MaxChars(pDX, m_seq_ctr, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSDB, CDialog)
	//{{AFX_MSG_MAP(CSDB)
	ON_BN_CLICKED(IDC_STATUS1_1, OnStatus)
	ON_BN_CLICKED(IDC_P1SEF, OnSEF)
	ON_BN_CLICKED(IDC_INCREASETOTW10000, OnIncreaseTOTW10000)
	ON_BN_CLICKED(IDC_STATUS1_2, OnStatus)
	ON_BN_CLICKED(IDC_STATUS1_3, OnStatus)
	ON_BN_CLICKED(IDC_STATUS1_4, OnStatus)
	ON_BN_CLICKED(IDC_STATUS1_5, OnStatus)
	ON_BN_CLICKED(IDC_STATUS2_1, OnStatus)
	ON_BN_CLICKED(IDC_STATUS2_2, OnStatus)
	ON_BN_CLICKED(IDC_STATUS2_3, OnStatus)
	ON_BN_CLICKED(IDC_STATUS2_4, OnStatus)
	ON_BN_CLICKED(IDC_STATUS2_5, OnStatus)
	ON_BN_CLICKED(IDC_STATUS3_1, OnStatus)
	ON_BN_CLICKED(IDC_STATUS3_2, OnStatus)
	ON_BN_CLICKED(IDC_STATUS3_3, OnStatus)
	ON_BN_CLICKED(IDC_STATUS3_4, OnStatus)
	ON_BN_CLICKED(IDC_STATUS3_5, OnStatus)
	ON_BN_CLICKED(IDC_STATUS4_1, OnStatus)
	ON_BN_CLICKED(IDC_STATUS4_2, OnStatus)
	ON_BN_CLICKED(IDC_STATUS4_3, OnStatus)
	ON_BN_CLICKED(IDC_STATUS4_4, OnStatus)
	ON_BN_CLICKED(IDC_STATUS4_5, OnStatus)
	ON_BN_CLICKED(IDC_STATUS5_1, OnStatus)
	ON_BN_CLICKED(IDC_STATUS5_2, OnStatus)
	ON_BN_CLICKED(IDC_STATUS5_3, OnStatus)
	ON_BN_CLICKED(IDC_STATUS5_4, OnStatus)
	ON_BN_CLICKED(IDC_STATUS5_5, OnStatus)
	ON_BN_CLICKED(IDC_STATUS6_1, OnStatus)
	ON_BN_CLICKED(IDC_STATUS6_2, OnStatus)
	ON_BN_CLICKED(IDC_STATUS6_3, OnStatus)
	ON_BN_CLICKED(IDC_STATUS6_4, OnStatus)
	ON_BN_CLICKED(IDC_STATUS6_5, OnStatus)
	ON_BN_CLICKED(IDC_STATUS7_1, OnStatus)
	ON_BN_CLICKED(IDC_STATUS7_2, OnStatus)
	ON_BN_CLICKED(IDC_STATUS7_3, OnStatus)
	ON_BN_CLICKED(IDC_STATUS7_4, OnStatus)
	ON_BN_CLICKED(IDC_STATUS7_5, OnStatus)
	ON_BN_CLICKED(IDC_STATUS8_1, OnStatus)
	ON_BN_CLICKED(IDC_STATUS8_2, OnStatus)
	ON_BN_CLICKED(IDC_STATUS8_3, OnStatus)
	ON_BN_CLICKED(IDC_STATUS8_4, OnStatus)
	ON_BN_CLICKED(IDC_STATUS8_5, OnStatus)
	ON_BN_CLICKED(IDC_P2SEF, OnSEF)
	ON_BN_CLICKED(IDC_P3SEF, OnSEF)
	ON_BN_CLICKED(IDC_P4SEF, OnSEF)
	ON_BN_CLICKED(IDC_P5SEF, OnSEF)
	ON_BN_CLICKED(IDC_P6SEF, OnSEF)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSDB message handlers


void CSDB::OnStatus() 
{
	m_bStatus1 = 0;
	m_bStatus2 = 0;
	m_bStatus3 = 0;
	m_bStatus4 = 0;
	m_bStatus5 = 0;

//////////////////////////////////////////////////////////////////////
/*
	todo
	This is dodgey below because it assumes the check box ids increase consecutively
	see pmdb_dlg.cpp.
  */


/////////////////// STATUS1 //////////////////////////////////////////
	for (int i = 0; i < 8; i++)
	{
    	int nButton = IDC_STATUS1_1;
		CButton* pBitSet = (CButton*)GetDlgItem(nButton+i);
		if (pBitSet->GetCheck()){
			int exponent = (int)pow(2, i);
			m_bStatus1 = m_bStatus1 | exponent;
		}
	}
	m_StatusDisplayString1 = HexToString((m_bStatus1 - (m_bStatus1 % 16))/16); // = F
	m_StatusDisplayString1 += HexToString(m_bStatus1 % 16);
	m_StatusDisplayCtrl1.SetWindowText(m_StatusDisplayString1);
/////////////////// end STATUS1 //////////////////////////////////////////
	
/////////////////// STATUS2 //////////////////////////////////////////
	for (i = 0; i < 8; i++)
	{
    	int nButton = IDC_STATUS1_2;
		CButton* pBitSet = (CButton*)GetDlgItem(nButton+i);
		if (pBitSet->GetCheck()){
			int exponent = (int)pow(2, i);
			m_bStatus2 = m_bStatus2 | exponent;
		}
	}
	m_StatusDisplayString2 = HexToString((m_bStatus2 - (m_bStatus2 % 16))/16); // = F
	m_StatusDisplayString2 += HexToString(m_bStatus2 % 16);
	m_StatusDisplayCtrl2.SetWindowText(m_StatusDisplayString2);
/////////////////// end STATUS2 //////////////////////////////////////////
	
/////////////////// STATUS3 //////////////////////////////////////////
	for (i = 0; i < 8; i++)
	{
    	int nButton = IDC_STATUS1_3;
		CButton* pBitSet = (CButton*)GetDlgItem(nButton+i);
		if (pBitSet->GetCheck()){
			int exponent = (int)pow(2, i);
			m_bStatus3 = m_bStatus3 | exponent;
		}
	}
	m_StatusDisplayString3 = HexToString((m_bStatus3 - (m_bStatus3 % 16))/16); // = F
	m_StatusDisplayString3 += HexToString(m_bStatus3 % 16);
	m_StatusDisplayCtrl3.SetWindowText(m_StatusDisplayString3);
/////////////////// end STATUS3 //////////////////////////////////////////
	
/////////////////// STATUS4 //////////////////////////////////////////
	for (i = 0; i < 8; i++)
	{
    	int nButton = IDC_STATUS1_4;
		CButton* pBitSet = (CButton*)GetDlgItem(nButton+i);
		if (pBitSet->GetCheck()){
			int exponent = (int)pow(2, i);
			m_bStatus4 = m_bStatus4 | exponent;
		}
	}
	m_StatusDisplayString4 = HexToString((m_bStatus4 - (m_bStatus4 % 16))/16); // = F
	m_StatusDisplayString4 += HexToString(m_bStatus4 % 16);
	m_StatusDisplayCtrl4.SetWindowText(m_StatusDisplayString4);
/////////////////// end STATUS4 //////////////////////////////////////////
	
/////////////////// STATUS5 //////////////////////////////////////////
	for (i = 0; i < 8; i++)
	{
    	int nButton = IDC_STATUS1_5;
		CButton* pBitSet = (CButton*)GetDlgItem(nButton+i);
		if (pBitSet->GetCheck()){
			int exponent = (int)pow(2, i);
			m_bStatus5 = m_bStatus5 | exponent;
		}
	}
	m_StatusDisplayString5 = HexToString((m_bStatus5 - (m_bStatus5 % 16))/16); // = F
	m_StatusDisplayString5 += HexToString(m_bStatus5 % 16);
	m_StatusDisplayCtrl5.SetWindowText(m_StatusDisplayString5);
/////////////////// end STATUS5 //////////////////////////////////////////
	

}


void CSDB::OnSEF() 
{
	m_bStatusSEF = 63;
	for (int i = 0; i < 6; i++)
	{
    	int nButton = IDC_P1SEF;
		CButton* pBitSet = (CButton*)GetDlgItem(nButton+i);
		if (pBitSet->GetCheck()){
			int exponent = (int)pow(2, i);
			m_bStatusSEF = m_bStatusSEF & ~exponent;/*m_bStatusSEF| exponent*/
		}
	}

	m_StatusDisplayString6 = HexToString((m_bStatusSEF - (m_bStatusSEF % 16))/16); // = F
	m_StatusDisplayString6 += HexToString(m_bStatusSEF % 16);
	m_StatusDisplayCtrl6.SetWindowText(m_StatusDisplayString6);
	
}

BOOL CSDB::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

BOOL CSDB::OnInitDialog()
{
	CDialog::OnInitDialog();	// CG: This was added by the ToolTips component.
	// CG: The following block was added by the ToolTips component.

	m_sfs.SetLimitText(2);

	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		m_tooltip.AddTool(GetDlgItem(IDC_GMID), "GMID to Send");

		m_tooltip.AddTool(GetDlgItem(IDC_STATUS1_1), "Idle");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS2_1), "Game Cycle");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS3_1), "Power Up");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS4_1), "Reset");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS5_1), "CCCE Transfer Complete");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS6_1), "0 (Spare)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS7_1), "0 (Spare)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS8_1), "0");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS1_2), "Large Win or Large Accumulated Credit");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS2_2), "Collect Cash");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS3_2), "Cancel Credit (Book Pay)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS4_2), "Progressive Win");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS5_2), "Manufacturer Specific Win");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS6_2), "Manufacturer Specific Win");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS7_2), "Manufacturer Specific Win");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS8_2), "0");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS1_3), "Security Door(s) Open");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS2_3), "Security Cage(s) Open");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS3_3), "Display Error");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS4_3), "Self Audit Error");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS5_3), "Memory Error");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS6_3), "Cash Input Error");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS7_3), "Cash Output Error");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS8_3), "0");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS1_4), "Audit Mode");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS2_4), "Test Mode");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS3_4), "Power Save");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS4_4), "Subsidiary Equipment Play Suspended");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS5_4), "Mechanical Meter Disconnect");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS6_4), "Manufacturer Specific Error");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS7_4), "Manufacturer Specific Error");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS8_4), "0");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS1_5), "Cancel Credit Error");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS2_5), "0 (Spare)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS3_5), "0 (Spare)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS4_5), "0 (Spare)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS5_5), "0 (Spare)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS6_5), "0 (Spare)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS7_5), "0 (Spare)");
		m_tooltip.AddTool(GetDlgItem(IDC_STATUS8_5), "0");

		m_tooltip.AddTool(GetDlgItem(IDC_PRTPH), "PRTP as a 4 digit number ie. 85.69% = 8569");
		m_tooltip.AddTool(GetDlgItem(IDC_PGMID1), "Program Identification, if unused set to 8 spaces");
		m_tooltip.AddTool(GetDlgItem(IDC_PGMID2), "Program Identification, if unused set to 8 spaces");
		m_tooltip.AddTool(GetDlgItem(IDC_PGMID3), "Program Identification, if unused set to 8 spaces");
		m_tooltip.AddTool(GetDlgItem(IDC_PGMID4), "Program Identification, if unused set to 8 spaces");
		m_tooltip.AddTool(GetDlgItem(IDC_BCV), "Base Credit Value, in cents");
		m_tooltip.AddTool(GetDlgItem(IDC_MGMID), "Multi - Game ID");
		m_tooltip.AddTool(GetDlgItem(IDC_SDBVER), "SDB Version number");

		m_tooltip.AddTool(GetDlgItem(IDC_CHKSUM), "Checksum of SDB");
		m_tooltip.AddTool(GetDlgItem(IDC_CHKSUMCORRECT), "Calculate the Checksum, or specify your own");
		m_tooltip.AddTool(GetDlgItem(IDC_BSC), "Block Sequence Counter");
		m_tooltip.AddTool(GetDlgItem(IDC_BSCCORRECT), "Calculate the BSC, or specify your own");

		m_tooltip.AddTool(GetDlgItem(IDC_P1SEF), "SEF Fail Port 1");
		m_tooltip.AddTool(GetDlgItem(IDC_P2SEF), "SEF Fail Port 2");
		m_tooltip.AddTool(GetDlgItem(IDC_P3SEF), "SEF Fail Port 3");
		m_tooltip.AddTool(GetDlgItem(IDC_P4SEF), "SEF Fail Port 4");
		m_tooltip.AddTool(GetDlgItem(IDC_P5SEF), "SEF Fail Port 5");
		m_tooltip.AddTool(GetDlgItem(IDC_P6SEF), "SEF Fail Port 6");

		m_tooltip.AddTool(GetDlgItem(IDC_TURNOVER), "Turnover");
		m_tooltip.AddTool(GetDlgItem(IDC_TOTALWINS), "Total Wins");
		m_tooltip.AddTool(GetDlgItem(IDC_CASHIN), "Cash In");
		m_tooltip.AddTool(GetDlgItem(IDC_CASHOUT), "Cash Out");
		m_tooltip.AddTool(GetDlgItem(IDC_MONEYIN), "Money In");
		m_tooltip.AddTool(GetDlgItem(IDC_MONEYOUT), "Money Out");
		m_tooltip.AddTool(GetDlgItem(IDC_CANCELLEDCR), "Cancelled Credit");
		m_tooltip.AddTool(GetDlgItem(IDC_CASHBOX), "Cash Box");
		m_tooltip.AddTool(GetDlgItem(IDC_MISCACC), "Miscellaneous Accrual");
		m_tooltip.AddTool(GetDlgItem(IDC_CREDIT), "Credit");
		m_tooltip.AddTool(GetDlgItem(IDC_PLAYED), "Games Played (Stroke)");
		m_tooltip.AddTool(GetDlgItem(IDC_POWERUP), "Power Cycles");
		m_tooltip.AddTool(GetDlgItem(IDC_GAMESDOOR), "Games Played since last door open");
		m_tooltip.AddTool(GetDlgItem(IDC_GAMESPOWER), "Games Played since last power cycle");

		
		
		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
	}
	return TRUE;	// CG: This was added by the ToolTips component.

}

int CSDB::serialize(CArchive* pArchive)
{
    int nStatus = 0;
	
    // Serialize the object ...
    ASSERT (pArchive != NULL);
    try
    {
		if (pArchive->IsStoring()) {
			(*pArchive)	<<	m_ChkGood	;
			(*pArchive)	<<	m_Checksum	;
			(*pArchive)	<<	m_gmid	;
			(*pArchive)	<<	m_sdb_vers	;
			(*pArchive)	<<	m_bcv	;
			(*pArchive)	<<	m_seq_ctr	;
			(*pArchive)	<<	m_BSCCorrect	;
			(*pArchive)	<<	m_CancelledCredit	;
			(*pArchive)	<<	m_CashBox	;
			(*pArchive)	<<	m_CashIn	;
			(*pArchive)	<<	m_CashOut	;
			(*pArchive)	<<	m_Credit	;
			(*pArchive)	<<	m_games_door	;
			(*pArchive)	<<	m_games_power	;
			(*pArchive)	<<	m_mgmid	;
			(*pArchive)	<<	m_MiscellaneousAccrual	;
			(*pArchive)	<<	m_MoneyIn	;
			(*pArchive)	<<	m_MoneyOut	;
			(*pArchive)	<<	m_P1SEF	;
			(*pArchive)	<<	m_P2SEF	;
			(*pArchive)	<<	m_P3SEF	;
			(*pArchive)	<<	m_P4SEF	;
			(*pArchive)	<<	m_P5SEF	;
			(*pArchive)	<<	m_P6SEF	;
			(*pArchive)	<<	m_ProgramID1	;
			(*pArchive)	<<	m_ProgramID2	;
			(*pArchive)	<<	m_ProgramID3	;
			(*pArchive)	<<	m_ProgramID4	;
			(*pArchive)	<<	m_games_played	;
			(*pArchive)	<<	m_power_up	;
			(*pArchive)	<<	m_rtp	;
			(*pArchive)	<<	m_TotalWins	;
			(*pArchive)	<<	m_Turnover	;
			(*pArchive)	<<	m_Status1_1	;
			(*pArchive)	<<	m_Status1_2	;
			(*pArchive)	<<	m_Status2_1	;
			(*pArchive)	<<	m_Status1_3	;
			(*pArchive)	<<	m_Status1_4	;
			(*pArchive)	<<	m_Status1_5	;
			(*pArchive)	<<	m_Status2_2	;
			(*pArchive)	<<	m_Status2_3	;
			(*pArchive)	<<	m_Status2_4	;
			(*pArchive)	<<	m_Status2_5	;
			(*pArchive)	<<	m_Status3_1	;
			(*pArchive)	<<	m_Status3_2	;
			(*pArchive)	<<	m_Status3_3	;
			(*pArchive)	<<	m_Status3_4	;
			(*pArchive)	<<	m_Status3_5	;
			(*pArchive)	<<	m_Status4_1	;
			(*pArchive)	<<	m_Status4_2	;
			(*pArchive)	<<	m_Status4_3	;
			(*pArchive)	<<	m_Status4_4	;
			(*pArchive)	<<	m_Status4_5	;
			(*pArchive)	<<	m_Status5_1	;
			(*pArchive)	<<	m_Status5_2	;
			(*pArchive)	<<	m_Status5_3	;
			(*pArchive)	<<	m_Status5_4	;
			(*pArchive)	<<	m_Status5_5	;
			(*pArchive)	<<	m_Status6_1	;
			(*pArchive)	<<	m_Status6_2	;
			(*pArchive)	<<	m_Status6_3	;
			(*pArchive)	<<	m_Status6_4	;
			(*pArchive)	<<	m_Status6_5	;
			(*pArchive)	<<	m_Status7_1	;
			(*pArchive)	<<	m_Status7_2	;
			(*pArchive)	<<	m_Status7_3	;
			(*pArchive)	<<	m_Status7_4	;
			(*pArchive)	<<	m_Status7_5	;
			(*pArchive)	<<	m_Status8_1	;
			(*pArchive)	<<	m_Status8_2	;
			(*pArchive)	<<	m_Status8_3	;
			(*pArchive)	<<	m_Status8_4	;
			(*pArchive)	<<	m_Status8_5	;
			(*pArchive)	<<	m_StatusDisplayString1	;
			(*pArchive)	<<	m_StatusDisplayString2	;
			(*pArchive)	<<	m_StatusDisplayString3	;
			(*pArchive)	<<	m_StatusDisplayString4	;
			(*pArchive)	<<	m_StatusDisplayString5	;
			(*pArchive)	<<	m_StatusDisplayString6	;
			(*pArchive)	<<	m_bStatus1	;
			(*pArchive)	<<	m_bStatus2	;
			(*pArchive)	<<	m_bStatus3	;
			(*pArchive)	<<	m_bStatus4	;
			(*pArchive)	<<	m_bStatus5	;
			(*pArchive)	<<	m_Checksum	;
			CString sfs_str; m_sfs.GetWindowText(sfs_str);
			(*pArchive)	<<	sfs_str	;
		}
		else {
			(*pArchive)	>>	m_ChkGood	;
			(*pArchive)	>>	m_Checksum	;
			(*pArchive)	>>	m_gmid	;
			(*pArchive)	>>	m_sdb_vers	;
			(*pArchive)	>>	m_bcv	;
			(*pArchive)	>>	m_seq_ctr	;
			(*pArchive)	>>	m_BSCCorrect	;
			(*pArchive)	>>	m_CancelledCredit	;
			(*pArchive)	>>	m_CashBox	;
			(*pArchive)	>>	m_CashIn	;
			(*pArchive)	>>	m_CashOut	;
			(*pArchive)	>>	m_Credit	;
			(*pArchive)	>>	m_games_door	;
			(*pArchive)	>>	m_games_power	;
			(*pArchive)	>>	m_mgmid	;
			(*pArchive)	>>	m_MiscellaneousAccrual	;
			(*pArchive)	>>	m_MoneyIn	;
			(*pArchive)	>>	m_MoneyOut	;
			(*pArchive)	>>	m_P1SEF	;
			(*pArchive)	>>	m_P2SEF	;
			(*pArchive)	>>	m_P3SEF	;
			(*pArchive)	>>	m_P4SEF	;
			(*pArchive)	>>	m_P5SEF	;
			(*pArchive)	>>	m_P6SEF	;
			(*pArchive)	>>	m_ProgramID1	;
			(*pArchive)	>>	m_ProgramID2	;
			(*pArchive)	>>	m_ProgramID3	;
			(*pArchive)	>>	m_ProgramID4	;
			(*pArchive)	>>	m_games_played	;
			(*pArchive)	>>	m_power_up	;
			(*pArchive)	>>	m_rtp	;
			(*pArchive)	>>	m_TotalWins	;
			(*pArchive)	>>	m_Turnover	;
			(*pArchive)	>>	m_Status1_1	;
			(*pArchive)	>>	m_Status1_2	;
			(*pArchive)	>>	m_Status2_1	;
			(*pArchive)	>>	m_Status1_3	;
			(*pArchive)	>>	m_Status1_4	;
			(*pArchive)	>>	m_Status1_5	;
			(*pArchive)	>>	m_Status2_2	;
			(*pArchive)	>>	m_Status2_3	;
			(*pArchive)	>>	m_Status2_4	;
			(*pArchive)	>>	m_Status2_5	;
			(*pArchive)	>>	m_Status3_1	;
			(*pArchive)	>>	m_Status3_2	;
			(*pArchive)	>>	m_Status3_3	;
			(*pArchive)	>>	m_Status3_4	;
			(*pArchive)	>>	m_Status3_5	;
			(*pArchive)	>>	m_Status4_1	;
			(*pArchive)	>>	m_Status4_2	;
			(*pArchive)	>>	m_Status4_3	;
			(*pArchive)	>>	m_Status4_4	;
			(*pArchive)	>>	m_Status4_5	;
			(*pArchive)	>>	m_Status5_1	;
			(*pArchive)	>>	m_Status5_2	;
			(*pArchive)	>>	m_Status5_3	;
			(*pArchive)	>>	m_Status5_4	;
			(*pArchive)	>>	m_Status5_5	;
			(*pArchive)	>>	m_Status6_1	;
			(*pArchive)	>>	m_Status6_2	;
			(*pArchive)	>>	m_Status6_3	;
			(*pArchive)	>>	m_Status6_4	;
			(*pArchive)	>>	m_Status6_5	;
			(*pArchive)	>>	m_Status7_1	;
			(*pArchive)	>>	m_Status7_2	;
			(*pArchive)	>>	m_Status7_3	;
			(*pArchive)	>>	m_Status7_4	;
			(*pArchive)	>>	m_Status7_5	;
			(*pArchive)	>>	m_Status8_1	;
			(*pArchive)	>>	m_Status8_2	;
			(*pArchive)	>>	m_Status8_3	;
			(*pArchive)	>>	m_Status8_4	;
			(*pArchive)	>>	m_Status8_5	;
			(*pArchive)	>>	m_StatusDisplayString1	;
			(*pArchive)	>>	m_StatusDisplayString2	;
			(*pArchive)	>>	m_StatusDisplayString3	;
			(*pArchive)	>>	m_StatusDisplayString4	;
			(*pArchive)	>>	m_StatusDisplayString5	;
			(*pArchive)	>>	m_StatusDisplayString6	;
			(*pArchive)	>>	m_bStatus1	;
			(*pArchive)	>>	m_bStatus2	;
			(*pArchive)	>>	m_bStatus3	;
			(*pArchive)	>>	m_bStatus4	;
			(*pArchive)	>>	m_bStatus5	;
			(*pArchive)	>>	m_Checksum	;
			CString sfs_str; m_sfs.GetWindowText(sfs_str);
			(*pArchive)	>>	sfs_str	;
		}
    }
    catch (CException* pException)
    {
		nStatus = 1;
    }
    return (nStatus);
	
}

void CSDB::OnIncreaseTOTW10000() 
{
	UpdateData(true);
	double tolow = atoi(m_Turnover);
	double twlow = atoi(m_TotalWins);

	tolow += 10000;
	twlow += 10000;

	m_Turnover.Format("%.0f", tolow);
	m_TotalWins.Format("%.0f", twlow);

	UpdateData(false);
}


void CSDB::GetMeters(void) // gets the current meters from sdb packet
{
	const CSDBPacket& sdb = g_game_mgr->GetSDB();

	sdb.GetDBField( CSDBPacket::TURNOVER, m_Turnover);
	sdb.GetDBField( CSDBPacket::TOTAL_WINS, m_TotalWins);	
	sdb.GetDBField( CSDBPacket::CASHBOX, m_CashBox);	
	sdb.GetDBField( CSDBPacket::CANCEL_CR, m_CancelledCredit);	
	sdb.GetDBField( CSDBPacket::GAMES_PLAYED, m_games_played);	
	sdb.GetDBField( CSDBPacket::MONEY_IN, m_MoneyIn);
	sdb.GetDBField( CSDBPacket::MONEY_OUT, m_MoneyOut);
	sdb.GetDBField( CSDBPacket::CASH_IN, m_CashIn);
	sdb.GetDBField( CSDBPacket::CASH_OUT, m_CashOut);
	sdb.GetDBField( CSDBPacket::CREDIT_METER, m_Credit);
	sdb.GetDBField( CSDBPacket::MISC_ACCRUAL, m_MiscellaneousAccrual);
	sdb.GetDBField( CSDBPacket::POWER_UP_COUNT, m_power_up);
	sdb.GetDBField( CSDBPacket::GAMES_PLAYED_POWER_UP, m_games_power);
	sdb.GetDBField( CSDBPacket::GAMES_PLAYED_DOOR, m_games_door);
	sdb.GetDBField( CSDBPacket::SEQ_COUNTER, m_seq_ctr);
}

/*
void CSDB::SetStaticData(CSDBPacket& p_packet) // only get the static data - no meters etc
{
	UpdateData(true);
	
	p_packet.SetDBField( CSDBPacket::GM_ID, m_gmid );
	p_packet.SetDBField( CSDBPacket::PROG_ID_1, m_ProgramID1 );
	p_packet.SetDBField( CSDBPacket::PROG_ID_2, m_ProgramID2 );
	p_packet.SetDBField( CSDBPacket::PROG_ID_3, m_ProgramID3 );
	p_packet.SetDBField( CSDBPacket::PROG_ID_4, m_ProgramID4 );

	p_packet.SetDBField( CSDBPacket::RTP, m_rtp );
	p_packet.SetDBField( CSDBPacket::BASE_CREDIT_VAL, m_bcv );
	p_packet.SetDBField( CSDBPacket::MULTIGAME_NUM, m_mgmid );
	p_packet.SetDBField( CSDBPacket::SDB_VERS, m_sdb_vers );

	CString sfs_str; m_sfs.GetWindowText(sfs_str);
	p_packet.SetDBField( CSDBPacket::SEC_FUNCS, sfs_str );
}
*/


void CSDB::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// Initialise the static data from the program options dialog - ie program id, gmid

	// todo
	// what if the dialog is not shown before begin transmission
	// it doesn't change - thats what!!!

	if( bShow )
	{
	
		UpdateData(false);
	}
	else
	{
		UpdateData(true);
	}
}

void CSDB::ProcessNotify( int p_id )  // for INotifiable
{
	if( IsWindowVisible() )
	{
		GetMeters();
		UpdateData(false);
	}
}
