
// OffLineThicknessDlg.h : 헤더 파일
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

// COffLineThicknessDlg 대화 상자
class COffLineThicknessDlg : public CDialogEx
{
	// 생성입니다.
public:
	COffLineThicknessDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	//클레스 선언
	CKeyenceSenser*	 m_pKeyence;
	CMotionPAS* m_pMotion;
	CKeyenceParam* m_pKeyenceParam;
	//차트
	CChartViewer	m_chartKeyence1; //차트1
	//MotionPAS m_pMotion; //모션 개체 
	CScrollBar		m_HScrollBar;	 //스크롤바

	CScrollBar m_HScrollBar2;
	CChartViewer m_chartKeyence2;

	//실제 두께 및 변위값
	double m_timeStamps[3][sampleSize];
	double m_dataSeriesA[3][sampleSize];	// The values for the data series A
	double m_dataSeriesB[3][sampleSize];	// The values for the data series B
	double m_dataSeriesC[3][sampleSize];	// The values for the data series C
	//double m_dataSeriesB1[40000];
	int m_currentIndex;
	int m_currentIndex_static;
	int m_static_test_index = 0;
	double m_nextDataTime;

	double m_dataSeriesB1[40000];	// The values for the data series B // 차트 2
	double m_dataSeriesC1[40000];


	double m_testSeriesD[400000];
	double m_HzStamps[40000]; //  차트2 변수 
	//그래프 표시/숨김 체크박스 변수 초기화
	BOOL m_bGraphTop;
	BOOL m_bGraphThickness;
	BOOL m_bGraphBottom;

	//로그 관련
	CListBox	m_cLogList;
	CXFile		m_XFile;



	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OFFLINETHICKNESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	TW_Button m_moveplus_btn;
	TW_Button m_moveminus_btn;
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void InitClass();

	//차트 관련
	static UINT OnChartUpdateTimer(LPVOID param);
	afx_msg void OnViewPortChanged();
	afx_msg void OnViewPortChanged_Static();
	void updateStaticControls(CChartViewer* viewer); // 차트 2 
	void drawStaticChart(CChartViewer* viewer); // 차트 2
	bool OnFullDataRateTimer();
	void updateControls(CChartViewer *viewer, int nDevice); //차트 1
	void drawChart(CChartViewer *viewer, int nChartNum);
	void trackLineLabel(XYChart *c, int mouseX);
	void DataLoad(); //  저장된 대이터를 불러오고 리스트에다가 띄기;  
	CString m_fileName; // 불러온 파일의 이름
	int m_FlistControlIndex = 0;
	//로그 관련
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

	// 함수 
	void ListControl(); // 리스트에 관한 기능을 관리 
	void UpdateEditValue(); // UpdateEdit Value의 계속 값을 변경해준

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




	// 모션 컨트롤 관련 함수, 변수들 
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

	static UINT OnStaticChartUpdate(LPVOID param); // Update 차트 2 Thread
	static UINT OnStaticDataRateTimer(LPVOID param); // 모션 이동 후 Static 차트에 데이터 올리기와  초기화
	static UINT TestDateSave(LPVOID param); // 테스트 Button 누를 때 테스트의 GetTrendData txt으로  저장함
	void DrawOldDataStaticChart(CString strPath); // Listcontrol에서 데이터를 불러오고 차트2에 그려주는 함수
	afx_msg void OnBnClickedButton1();
	void CreateFileDirectory(); // 저장을 위해 경로를 미리 만들어 놓기

	int test_count;
	afx_msg void OnBnClickedOk();

	void UIDesign(); // UI 꾸미기 위한 함수 
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
