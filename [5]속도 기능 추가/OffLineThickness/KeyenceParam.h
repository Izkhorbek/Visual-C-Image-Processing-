#pragma once


// CKeyenceParam 대화 상자입니다.

class CKeyenceParam : public CDialogEx
{
	DECLARE_DYNAMIC(CKeyenceParam)

public:
	CKeyenceParam(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CKeyenceParam();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_KEYENCE_IP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	int m_nIP_Adress1;
	int m_nIP_Adress2;
	int m_nIP_Adress3;
	int m_nIP_Adress4;

	void UpdateParam(bool bLoad);
	afx_msg void OnBnClickedOk2();
	int m_nPort_Adress;
	int m_nSensor_IP1;
	int m_nSensor_IP2;
	int m_nSensor_IP3;
	int m_nSensor_IP4;
	int m_nSensor_Port;
};
