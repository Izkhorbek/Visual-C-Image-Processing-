// KeyenceParam.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "OffLineThickness.h"
#include "KeyenceParam.h"
#include "afxdialogex.h"
#include "OffLineThicknessDlg.h"
#include "ini.h"
// CKeyenceParam 대화 상자입니다.

IMPLEMENT_DYNAMIC(CKeyenceParam, CDialogEx)

CKeyenceParam::CKeyenceParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_KEYENCE_IP, pParent)
	, m_nIP_Adress1(192)
	, m_nIP_Adress2(168)
	, m_nIP_Adress3(0)
	, m_nIP_Adress4(2)
	, m_nPort_Adress(0)
	, m_nSensor_IP1(100)
	, m_nSensor_IP2(0)
	, m_nSensor_IP3(0)
	, m_nSensor_IP4(3)
	, m_nSensor_Port(24685)
{

}

CKeyenceParam::~CKeyenceParam()
{
}

void CKeyenceParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IP1, m_nIP_Adress1);
	DDX_Text(pDX, IDC_EDIT_IP2, m_nIP_Adress2);
	DDX_Text(pDX, IDC_EDIT_IP3, m_nIP_Adress3);
	DDX_Text(pDX, IDC_EDIT_IP4, m_nIP_Adress4);

	DDX_Text(pDX, IDC_EDIT_SENSOR_IP1, m_nSensor_IP1);
	DDX_Text(pDX, IDC_EDIT_SENSOR_IP2, m_nSensor_IP2);
	DDX_Text(pDX, IDC_EDIT_SENSOR_IP3, m_nSensor_IP3);
	DDX_Text(pDX, IDC_EDIT_SENSOR_IP4, m_nSensor_IP4);
	DDX_Text(pDX, IDC_EDIT_SENSOR_PORT, m_nSensor_Port);
}


BEGIN_MESSAGE_MAP(CKeyenceParam, CDialogEx)
	ON_BN_CLICKED(IDOK, &CKeyenceParam::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CKeyenceParam::OnBnClickedOk2)
END_MESSAGE_MAP()


// CKeyenceParam 메시지 처리기입니다.


void CKeyenceParam::OnBnClickedOk()
{
	COffLineThicknessDlg *pDlg = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;
	
    UpdateParam(FALSE);
	
}

void CKeyenceParam::UpdateParam(bool bLoad)
{
	if (!bLoad)	//저장
	{
		UpdateData();
	}

	CIni ini(SETTING_PATH, "\nKeyence and Sensor Param", 1);

	// 키엔스 IP 설정
	ini.SER_GETD(bLoad, m_nIP_Adress1, 192);
	ini.SER_GETD(bLoad, m_nIP_Adress2, 168);
	ini.SER_GETD(bLoad, m_nIP_Adress3, 0);
	ini.SER_GETD(bLoad, m_nIP_Adress4, 2);
	
	// 센서
	ini.SER_GETD(bLoad, m_nSensor_IP1, 100);
	ini.SER_GETD(bLoad, m_nSensor_IP2, 0);
	ini.SER_GETD(bLoad, m_nSensor_IP3, 0);
	ini.SER_GETD(bLoad, m_nSensor_IP4, 3);
	ini.SER_GETD(bLoad, m_nSensor_Port, 24685);

	if (bLoad)
	{
		UpdateData(FALSE);
	}

}
void CKeyenceParam::OnBnClickedOk2()
{
	CDialogEx::OnOK();
}
