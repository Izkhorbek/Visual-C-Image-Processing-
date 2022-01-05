
// OffLineThicknessDlg.h : ��� ����
//

#include "ChartViewer.h"
#include "chartdir.h"
#include "Global.h"
#include "XFile.h"
#include "KeyenceSenser.h"
#include "OffLineThickness.h"
#include "MotionPAS.h"
#include "Label.h"
#include "KeyenceParam.h"
#include "TW_Button.h"
#pragma once

static const int sampleSize = 10000;
static const int zoomInLimit = 1;
static const int DataInterval = 10;
static const int initialFullRange = 60;

// COffLineThicknessDlg ��ȭ ����
class COffLineThicknessDlg : public CDialogEx
{
	// �����Դϴ�.
public:
	COffLineThicknessDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	//Ŭ���� ����
	CKeyenceSenser*	 m_pKeyence;
	CMotionPAS* m_pMotion;
	CKeyenceParam* m_pKeyenceParam;
	//��Ʈ
	CChartViewer	m_chartKeyence1; //��Ʈ1
	//MotionPAS m_pMotion; //��� ��ü 
	CScrollBar		m_HScrollBar;	 //��ũ�ѹ�

	CScrollBar m_HScrollBar2;
	CChartViewer m_chartKeyence2;

	//���� �β� �� ������
	double m_timeStamps[3][sampleSize];
	double m_dataSeriesA[3][sampleSize];	// The values for the data series A
	double m_dataSeriesB[3][sampleSize];	// The values for the data series B
	double m_dataSeriesC[3][sampleSize];	// The values for the data series C
	//double m_dataSeriesB1[40000];
	int m_currentIndex;
	int m_currentIndex_static;
	int m_static_test_index = 0;
	double m_nextDataTime;

	double m_dataSeriesB1[40000];	// The values for the data series B // ��Ʈ 2
	double m_dataSeriesC1[40000];


	double m_testSeriesD[400000];
	double m_HzStamps[40000]; //  ��Ʈ2 ���� 
	//�׷��� ǥ��/���� üũ�ڽ� ���� �ʱ�ȭ
	BOOL m_bGraphTop;
	BOOL m_bGraphThickness;
	BOOL m_bGraphBottom;

	//�α� ����
	CListBox	m_cLogList;
	CXFile		m_XFile;



	// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OFFLINETHICKNESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	TW_Button m_moveplus_btn;
	TW_Button m_moveminus_btn;
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitClass();

	//��Ʈ ����
	static UINT OnChartUpdateTimer(LPVOID param);
	afx_msg void OnViewPortChanged();
	afx_msg void OnViewPortChanged_Static();
	void updateStaticControls(CChartViewer* viewer); // ��Ʈ 2 
	void drawStaticChart(CChartViewer* viewer); // ��Ʈ 2
	bool OnFullDataRateTimer();
	void updateControls(CChartViewer *viewer, int nDevice); //��Ʈ 1
	void drawChart(CChartViewer *viewer, int nChartNum);
	void trackLineLabel(XYChart *c, int mouseX);
	void DataLoad(); //  ����� �����͸� �ҷ����� ����Ʈ���ٰ� ���;  
	CString m_fileName; // �ҷ��� ������ �̸�
	int m_FlistControlIndex = 0;
	//�α� ����
	void AddtoList(CString strData, BOOL bWrite = 1);
	void WriteLog(CString str);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckGraphTop();
	afx_msg void OnBnClickedCheckGraphBottom();
	afx_msg void OnBnClickedCheckGraphThickness();

	void UpdateSetting(bool bLoad);

	int m_nGraphMin;
	int m_nGraphMax;
	afx_msg void OnBnClickedButtonParamSave();
	CListCtrl m_ListReport;

	// �Լ� 
	void ListControl(); // ����Ʈ�� ���� ����� ���� 
	void UpdateEditValue(); // UpdateEdit Value�� ��� ���� ��������

	afx_msg void OnBnClickedBtnFilePath();
	afx_msg void OnBnClickedBtnDefault();
	//	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);

	//	afx_msg void OnLvnKeydownList2(NMHDR *pNMHDR, LRESULT *pResult);
	//	afx_msg void OnItemdblclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	//	afx_msg void OnNMRDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMovePlotArea();
	afx_msg void OnMouseMovePlotArea_Static();

	afx_msg void OnBnClickedBtnText();
	double m_dZeroSet;
	afx_msg void OnBnClickedBtnZeroset();




	// ��� ��Ʈ�� ���� �Լ�, ������ 
	double m_dStartPosition;
	double m_dEndPosition;
	double m_dVelocity;
	float m_CPosition; // 
	int  m_nCount;
	int m_KCount;

	CWinThread* m_actualPosThread;
	CWinThread* m_PosThread;
	CWinThread* m_TextThread;
	static UINT ActualPos(LPVOID param);
	static UINT CommandPos(LPVOID param);
	CEdit m_actualPos;
	CEdit m_commandPos;

	static UINT OnStaticChartUpdate(LPVOID param); // Update ��Ʈ 2 Thread
	static UINT OnStaticDataRateTimer(LPVOID param); // ��� �̵� �� Static ��Ʈ�� ������ �ø����  �ʱ�ȭ
	static UINT TestDateSave(LPVOID param); // �׽�Ʈ Button ���� �� �׽�Ʈ�� GetTrendData txt����  ������
	void DrawOldDataStaticChart(CString strPath); // Listcontrol���� �����͸� �ҷ����� ��Ʈ2�� �׷��ִ� �Լ�
	afx_msg void OnBnClickedButton1();
	void CreateFileDirectory(); // ������ ���� ��θ� �̸� ����� ����

	int test_count;
	afx_msg void OnBnClickedOk();

	void UIDesign(); // UI �ٹ̱� ���� �Լ� 
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CLabel m_lMotor_condition;
	CLabel m_lKeyence_Mode;
	CLabel m_lKeyence_signal;
	CLabel m_lMotor_signal;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedKeyenceIp();

	

	void MovePlus();
	void Movestop();
	void MovePrev();
	double m_dMovePos;
	
};
