
// OffLineThicknessDlg.cpp : 구현 파일
//

#include "stdafx.h"

#include "OffLineThicknessDlg.h"
#include "afxdialogex.h"
#include "ini.h"

#include "MotionPAS.h"
#include "KeyenceParam.h"

#include "FAS_EziMOTIONPlusE.h"
#include <fstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <sstream>
#include<iostream>
#include "TW_Button.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COffLineThicknessDlg 대화 상자



COffLineThicknessDlg::COffLineThicknessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OFFLINETHICKNESS_DIALOG, pParent)
	, m_nGraphMin(0)
	, m_nGraphMax(0)
	, m_dZeroSet(0)
	, m_dStartPosition(0)
	, m_dEndPosition(0)
	, m_dVelocity(0)
	, m_dMovePos(0)
{
	m_currentIndex = 0;
	m_nextDataTime = 0;

	//그래프 표시/숨김 체크박스 변수 초기화

	m_bGraphTop = FALSE;

	m_bGraphThickness = TRUE;
	m_bGraphBottom = FALSE;

	m_nCount = 0;
	m_KCount = 0;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void COffLineThicknessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	/*********************************************************/
	// chart
	DDX_Control(pDX, IDC_CHART_KEYENCE1, m_chartKeyence1);
	//스크롤바
	DDX_Control(pDX, IDC_HScrollBar, m_HScrollBar);

	//LOG
	DDX_Control(pDX, IDC_LIST_LOG, m_cLogList);

	//chart 표시/숨김 체크박스
	DDX_Check(pDX, IDC_CHECK_GRAPH_TOP, m_bGraphTop);
	DDX_Check(pDX, IDC_CHECK_GRAPH_THICKNESS, m_bGraphThickness);
	DDX_Check(pDX, IDC_CHECK_GRAPH_BOTTOM, m_bGraphBottom);


	// y축 간격 관리
	DDX_Text(pDX, IDC_EDIT_GRAPH_MIN, m_nGraphMin);
	DDX_Text(pDX, IDC_EDIT_GRAPH_MAX, m_nGraphMax);

	// 이력 관리 리스트
	DDX_Control(pDX, IDC_LIST2, m_ListReport);
	// 제로셋 변수
	DDX_Text(pDX, IDC_EDIT2, m_dZeroSet);

	DDX_Control(pDX, IDC_HScrollBar2, m_HScrollBar2);
	DDX_Control(pDX, IDC_CHART_KEYENCE2, m_chartKeyence2);
	DDX_Text(pDX, IDC_EDIT_START, m_dStartPosition);
	DDX_Text(pDX, IDC_EDIT_END, m_dEndPosition);
	DDX_Text(pDX, IDC_EDIT6, m_dVelocity);
	DDX_Control(pDX, IDC_STATIC_MOTOR_MODE, m_lMotor_condition);
	DDX_Control(pDX, IDC_STATIC_KEYENCE_MODE, m_lKeyence_Mode);
	DDX_Control(pDX, IDC_STATIC_KEYENCE_SIGNAL, m_lKeyence_signal);
	DDX_Control(pDX, IDC_STATIC_MOTOR_SIGNAL, m_lMotor_signal);
	DDX_Text(pDX, IDC_EDIT_JOG_MOVE, m_dMovePos);
}

BEGIN_MESSAGE_MAP(COffLineThicknessDlg, CDialogEx)
	ON_CONTROL(CVN_ViewPortChanged, IDC_CHART_KEYENCE1, OnViewPortChanged)

	ON_CONTROL(CVN_ViewPortChanged, IDC_CHART_KEYENCE2, OnViewPortChanged_Static)
	
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_CHART_KEYENCE1, OnMouseMovePlotArea)
	ON_CONTROL(CVN_MouseMovePlotArea, IDC_CHART_KEYENCE2, OnMouseMovePlotArea_Static)

	ON_CONTROL(BN_CLICK_DOWN, IDC_BTN_MOVE_PLUS, MovePlus)
	ON_CONTROL(BN_CLICK_UP, IDC_BTN_MOVE_PLUS, Movestop)

	ON_CONTROL(BN_CLICK_DOWN, IDC_BTN_MOVE_MINUS, MovePrev)
	ON_CONTROL(BN_CLICK_UP, IDC_BTN_MOVE_MINUS, Movestop)



	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_GRAPH_TOP, &COffLineThicknessDlg::OnBnClickedCheckGraphTop)
	ON_BN_CLICKED(IDC_CHECK_GRAPH_BOTTOM, &COffLineThicknessDlg::OnBnClickedCheckGraphBottom)
	ON_BN_CLICKED(IDC_CHECK_GRAPH_THICKNESS, &COffLineThicknessDlg::OnBnClickedCheckGraphThickness)
	ON_BN_CLICKED(IDC_BUTTON_PARAM_SAVE, &COffLineThicknessDlg::OnBnClickedButtonParamSave)
	
	ON_BN_CLICKED(IDC_BTN_FILE_PATH, &COffLineThicknessDlg::OnBnClickedBtnFilePath)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, &COffLineThicknessDlg::OnBnClickedBtnDefault)

ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &COffLineThicknessDlg::OnNMDblclkList2)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &COffLineThicknessDlg::OnLvnItemchangedList2)
ON_BN_CLICKED(IDC_BTN_TEXT, &COffLineThicknessDlg::OnBnClickedBtnText)
ON_BN_CLICKED(IDC_BTN_ZEROSET, &COffLineThicknessDlg::OnBnClickedBtnZeroset)
ON_BN_CLICKED(IDC_BUTTON1, &COffLineThicknessDlg::OnBnClickedButton1)
ON_BN_CLICKED(IDOK, &COffLineThicknessDlg::OnBnClickedOk)
//ON_WM_CTLCOLOR()
ON_WM_ERASEBKGND()
ON_WM_DESTROY()
ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_KEYENCE_IP, &COffLineThicknessDlg::OnBnClickedKeyenceIp)

ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// COffLineThicknessDlg 메시지 처리기

BOOL COffLineThicknessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	CMenu* pMenu = GetSystemMenu(FALSE);
	pMenu->DeleteMenu(SC_CLOSE, MF_BYCOMMAND);

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//메인 화면 사이즈 조절
	int iScreenWidth = 1920;
	int iScreenHeight = 1082;

	MoveWindow(0, 0, iScreenWidth, iScreenHeight);


	/******************************************  Chart Init **********************************************************************************/
	GetDlgItem(IDC_HScrollBar)->BringWindowToTop();
	GetDlgItem(IDC_HScrollBar2)->BringWindowToTop();

	m_chartKeyence1.setMouseUsage(Chart::MouseUsageScroll);
	m_chartKeyence1.setMouseWheelZoomRatio(1.1);

	m_chartKeyence2.setMouseUsage(Chart::MouseUsageScroll);
	m_chartKeyence2.setMouseWheelZoomRatio(1.1);
	


	int updateType = Chart::KeepVisibleRange;
	bool scaleHasChanged = m_chartKeyence1.updateFullRangeH("x", 0, 100, updateType);
	m_chartKeyence1.setZoomInWidthLimit(zoomInLimit / m_chartKeyence1.getValueAtViewPort("x", 1) - m_chartKeyence1.getValueAtViewPort("x", 0));
	m_chartKeyence1.updateViewPort(true, false);

//
//
//	int updateType = Chart::KeepVisibleRange;
/////	bool scaleHasChanged = m_chartKeyence2.updateFullRangeH("x", 0, 100, updateType);
	//m_chartKeyence2.setZoomInWidthLimit(zoomInLimit / m_chartKeyence2.getValueAtViwPort("x", 1) - m_chartKeyence2.getValueAtViewPort("x", 0));
	m_chartKeyence2.updateViewPort(true, false);
	/********************************************************************************************************************************************/
	m_currentIndex_static = 0;
	UpdateSetting(TRUE);
	//OnStaticChartUpdate(this);

	m_moveplus_btn.SubclassDlgItem(IDC_BTN_MOVE_PLUS, this);
	m_moveminus_btn.SubclassDlgItem(IDC_BTN_MOVE_MINUS, this);

	InitClass();

	SYSTEMTIME st;
	GetLocalTime(&st);

	m_nextDataTime = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);


	SetTimer(WM_TIMER_CHART_UPDATE, DataInterval, 0);
	SetTimer(WM_TIMER_STORAGE_READ, 10, 0);
	SetTimer(WM_TIMER_MOTIOM_UPDATE, 100, 0);
	

	ListControl();
	UIDesign();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}
void COffLineThicknessDlg::UIDesign()
{
	CFont font, conditonfont;
	font.CreatePointFont(150, "consolas");
	
	GetDlgItem(IDC_EDIT_TOP)->SetFont(&font);
	GetDlgItem(IDC_EDIT_THICKNESS)->SetFont(&font);
	GetDlgItem(IDC_EDIT_BOTTOM)->SetFont(&font);

	// Keyence 상태

	m_lKeyence_Mode.SetBkColor(RGB(227, 233, 255));
	m_lKeyence_Mode.SetText("키엔스 상태");
	m_lKeyence_Mode.SetTextColor(RGB(0,0,0));
	m_lKeyence_Mode.SetFontBold(TRUE);
	m_lKeyence_Mode.SetFontName("consolas");
	m_lKeyence_Mode.SetFontSize(12);

	m_lKeyence_signal.SetText(" ");
	if (m_pKeyence->m_bRes[0] == FALSE)
	{
		m_lKeyence_signal.SetBkColor(RGB(251, 15, 6)); // Off 때 red color
	}
	else
	{
		m_lKeyence_signal.SetBkColor(RGB(12, 242, 33)); //  On 때green color
	}

	// 모터의 상태 
	
	m_lMotor_condition.SetBkColor(RGB(227, 233, 255));
	m_lMotor_condition.SetText("모터 상태");
	m_lMotor_condition.SetTextColor(RGB(0, 0, 0));
	m_lMotor_condition.SetFontBold(TRUE);
	m_lMotor_condition.SetFontName("consolas");
	m_lMotor_condition.SetFontSize(12);

	m_lMotor_signal.SetText(" ");
	if (m_pMotion->m_bMotion_Signal == FALSE)
	{
		m_lMotor_signal.SetBkColor(RGB(251, 15, 6));  // off 때 red 
	}
	else
	{
		m_lMotor_signal.SetBkColor(RGB(12, 242, 33)); //On 때 green
	}


}
void COffLineThicknessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COffLineThicknessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COffLineThicknessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void COffLineThicknessDlg::CreateFileDirectory()
{
	CString strMD, strMDMS;
	CTime t = CTime::GetCurrentTime();
	strMD.Format(_T("%02d%02d"), t.GetMonth(), t.GetDay());
	strMDMS.Format(_T("%02d%02d_%02d%02d%02d"), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());


	CString strPath;
	strPath = "c:\\GLIM\\Parameter\\";
	CreateDirectory(strPath, NULL);

	strPath = FILE_PATH_DIRECTORY;
	CreateDirectory(strPath, NULL);
	
}
void COffLineThicknessDlg::AddtoList(CString strData, BOOL bWrite)
{
	CString str;
	CTime time = CTime::GetCurrentTime();
	strData = time.Format("[%H:%M:%S]") + strData;

	if (bWrite)
		WriteLog(strData);

	m_cLogList.InsertString(-1, strData);
	int x = m_cLogList.GetCount();
	m_cLogList.SetTopIndex(x - 1);
	if (x > TOTAL_LOG_LIST_COL)
	{
		m_cLogList.DeleteString(0);
		m_cLogList.SetCurSel(TOTAL_LOG_LIST_COL - 1);
	}

}

void COffLineThicknessDlg::WriteLog(CString str)
{
	CTime time = CTime::GetCurrentTime();

	CString strFile;
	//strFile.Format("%s\\%s_System_Log.txt",LOG_PATH,time.Format("%Y%m%d"));  //pgt 주석 해제해야함
	strFile.Format("%s\\Log\\%s\\%s_System_Log.txt", DEFAULT_GLIM, time.Format("%Y%m%d"), time.Format("%Y%m%d"));
	m_XFile.XCreateDirectory(strFile);
	//WriteDetailLog(str);

	FILE *fp;
	//str = time.Format("[%y-%m-%d %H:%M:%S]") + str;
	fp = fopen((LPCSTR)strFile, "a");

	if (!fp)
		return;
	char* buf = new char[str.GetLength() + 1];
	memcpy(buf, str.LockBuffer(), str.GetLength());
	buf[str.GetLength()] = '\0';

	fputs(buf, fp);
	fputs("\n", fp);
	delete[]buf;
	fclose(fp);
}


void COffLineThicknessDlg::updateControls(CChartViewer *viewer, int nDevice)
{
	//마우스 휠 돌릴때 실행함

	// Update the scroll bar to reflect the view port position and width of the view port.
	m_HScrollBar.EnableWindow(viewer->getViewPortWidth() < 1);
	if (viewer->getViewPortWidth() < 1)
	{
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		info.nMin = 0;
		info.nMax = 0x1fffffff;
		info.nPage = (int)ceil(viewer->getViewPortWidth() * (info.nMax - info.nMin));
		info.nPos = (int)(0.5 + viewer->getViewPortLeft() * (info.nMax - info.nMin)) + info.nMin;
		m_HScrollBar.SetScrollInfo(&info);
	}
	
}

void COffLineThicknessDlg::OnViewPortChanged()
{
	/*updateControls(&m_chartKeyence1,0);*/
	// Update the chart if necessary
	if (m_chartKeyence1.needUpdateChart())
		drawChart(&m_chartKeyence1, 0);
}

void COffLineThicknessDlg::OnViewPortChanged_Static()
{

	updateStaticControls(&m_chartKeyence2);
	// Update the chart if necessary
	if (m_chartKeyence2.needUpdateChart())
		drawStaticChart(&m_chartKeyence2);

}
void COffLineThicknessDlg::updateStaticControls(CChartViewer* viewer)
{
	m_HScrollBar2.EnableWindow(viewer->getViewPortWidth() < 1);
	if (viewer->getViewPortWidth() < 1)
	{
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_ALL;
		info.nMin = 0;
		info.nMax = 0x1fffffff;
		info.nPage = (int)ceil(viewer->getViewPortWidth() * (info.nMax - info.nMin));
		info.nPos = (int)(0.5 + viewer->getViewPortLeft() * (info.nMax - info.nMin)) + info.nMin;
		m_HScrollBar2.SetScrollInfo(&info);
	}
	
}
void COffLineThicknessDlg::drawStaticChart(CChartViewer* viewer)
{
	UpdateData(TRUE); // 1210
					  // Get the start date and end date that are visible on the chart.
	double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
	double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() +
		viewer->getViewPortWidth());
	// Extract the part of the data arrays that are visible.


	DoubleArray viewPortTimeStamps;
	DoubleArray viewPortDataSeriesA;
	DoubleArray viewPortDataSeriesB;
	DoubleArray viewPortDataSeriesC;

	if (m_currentIndex_static > 0)
	{
		// Get the array indexes that corresponds to the visible start and end dates
		int startIndex = (int)floor(Chart::bSearch(DoubleArray(m_HzStamps, m_currentIndex_static), viewPortStartDate));
		int endIndex = (int)ceil(Chart::bSearch(DoubleArray(m_HzStamps, m_currentIndex_static), viewPortEndDate));
		int noOfPoints = endIndex - startIndex + 1;

		// Extract the visible data
		viewPortTimeStamps = DoubleArray(m_HzStamps + startIndex, noOfPoints);
		/*viewPortDataSeriesA = DoubleArray(m_dataSeriesA1 + startIndex, noOfPoints);*/
		viewPortDataSeriesB = DoubleArray(m_dataSeriesB1 + startIndex, noOfPoints);
		/*viewPortDataSeriesC = DoubleArray(m_dataSeriesC1 + startIndex, noOfPoints);*/
	}

	//
	// At this stage, we have extracted the visible data. We can use those data to plot the chart.
	//

	//================================================================================
	// Configure overall chart appearance.
	//================================================================================

	// Create an XYChart object of size 640 x 350 pixels
	CRect rt;
	GetDlgItem(IDC_CHART_KEYENCE2)->GetWindowRect(&rt);

	XYChart *c;
	c = new XYChart(rt.Width(), rt.Height(), 0xf4f4f4, 0x000000, 1);


	c->setPlotArea(30, 50, rt.Width() - 60, c->getHeight() - 80, 0xffffff, -1, Chart::Transparent)->setGridColor(0xc0c0c0);
	//c->setRoundedFrame();  //그래프 꼭지점을 둥글게

	// As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
	c->setClipping();

	// Add a title to the chart using 18pt Arial font
	c->addTitle("Static thickness measurement(uM)", "arial.ttf", 18);

	// Add a legend box at (55, 25) using horizontal layout. Use 10pt Arial Bold as font. Set the
	// background and border color to transparent and use line style legend key.
	LegendBox* b = c->addLegend(55, 25, false, "arialbd.ttf", 10);
	b->setBackground(Chart::Transparent);
	b->setLineStyleKey();

	// Set the x and y axis stems to transparent and the label font to 10pt Arial
	c->xAxis()->setColors(Chart::Transparent);
	c->yAxis()->setColors(Chart::Transparent);

	c->setColors(whiteOnBlackPalette);
	c->xAxis()->setLabelStyle("arial.ttf", 10);
	c->yAxis()->setLabelStyle("arial.ttf", 10);
	//c->yAxis()->setLinearScale(-100, 300, 50);
	//c->yAxis()->setLinearScale(-50, 400, 50);
	c->yAxis()->setLinearScale(m_nGraphMin, m_nGraphMax, 50);
	// Set the y-axis tick length to 0 to disable the tick and put the labels closer to the axis.
	c->yAxis()->setTickLength(0);

	// Add axis title using 12pt Arial Bold Italic font
	c->yAxis()->setTitle("Thickness(uM)", "arialbd.ttf", 10);

	//================================================================================
	// Add data to chart
	//================================================================================

	//
	// In this example, we represent the data by lines. You may modify the code below to use other
	// representations (areas, scatter plot, etc).
	//

	// Add a line layer for the lines, using a line width of 2 pixels
	LineLayer* layer = c->addLineLayer();
	layer->setLineWidth(3);
	layer->setFastLineMode();

	// Now we add the 3 data series to a line layer, using the color red (ff0000), green (00cc00)
	// and blue (0000ff)
	layer->setXData(viewPortTimeStamps);
	/*if (!((CButton*)GetDlgItem(IDC_CHECK_TOP))->GetCheck()) {
	layer->addDataSet(viewPortDataSeriesA, 0xff0000, "Top");
	}*/
	//if (m_bGraphThickness == TRUE){
	layer->addDataSet(viewPortDataSeriesB, 0xff0000, "thickness");
	//}

	/*if (m_bGraphThickness == TRUE) {
		layer->addDataSet(viewPortDataSeriesB, 0xff0000, "thickness");
	}*/
	/*if (!((CButton*)GetDlgItem(IDC_CHECK3_BTM))->GetCheck()) {
	layer->addDataSet(viewPortDataSeriesC, 0x0000ff, "Bottom");
	}*/



	//================================================================================
	// Configure axis scale and labelling
	//================================================================================

	// Set the x-axis as a date/time axis with the scale according to the view port x range.
	if (m_currentIndex_static > 0)
		c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

	// For the automatic axis labels, set the minimum spacing to 75/30 pixels for the x/y axis.
	c->xAxis()->setTickDensity(75);
	c->yAxis()->setTickDensity(30);

	//
	// In this demo, the time range can be from many hours to a few seconds. We can need to define
	// the date/time format the various cases. 
	//

	// If all ticks are hour algined, we use "hh:nn<*br*>mmm dd" in bold font as the first label of
	// the Day, and "hh:nn" for other labels.


	//================================================================================
	// Output the chart
	//================================================================================

	// We need to update the track line too. If the mouse is moving on the chart (eg. if 
	// the user drags the mouse on the chart to scroll it), the track line will be updated
	// in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
	if (!viewer->isInMouseMoveEvent())
	{
		trackLineLabel(c, (0 == viewer->getChart()) ? c->getPlotArea()->getRightX() :
			viewer->getPlotAreaMouseX());
	}

	// Set the chart image to the WinChartViewer
	delete viewer->getChart();
	viewer->setChart(c);
	updateStaticControls(&m_chartKeyence2);
}



void COffLineThicknessDlg::drawChart(CChartViewer *viewer, int nChartNum)
{
	// Get the start date and end date that are visible on the chart.
	double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
	double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() +
		viewer->getViewPortWidth());

	// Extract the part of the data arrays that are visible.
	//그래프용 데이터
	DoubleArray viewPortTimeStamps;
	DoubleArray viewPortDataSeriesA;
	DoubleArray viewPortDataSeriesB;
	DoubleArray viewPortDataSeriesC;

	double dLabelDataA = 0;
	double dLabelDataB = 0;
	double dLabelDataC = 0;

	int nindex = 0;

	int n_GrapMin;
	int n_GrapMax;



 	nindex = m_currentIndex;
	n_GrapMin = m_nGraphMin;
	n_GrapMax = m_nGraphMax;


	if (nindex > 0)
	{
		// Get the array indexes that corresponds to the visible start and end dates
		int startIndex = (int)floor(Chart::bSearch(DoubleArray(m_timeStamps[nChartNum], nindex), viewPortStartDate));
		int endIndex = (int)ceil(Chart::bSearch(DoubleArray(m_timeStamps[nChartNum], nindex), viewPortEndDate));
		int noOfPoints = endIndex - startIndex + 1;


		// Extract the visible data
		viewPortTimeStamps = DoubleArray(m_timeStamps[nChartNum] + startIndex, noOfPoints);
		viewPortDataSeriesA = DoubleArray(m_dataSeriesA[nChartNum] + startIndex, noOfPoints);
		viewPortDataSeriesB = DoubleArray(m_dataSeriesB[nChartNum] + startIndex, noOfPoints);
		viewPortDataSeriesC = DoubleArray(m_dataSeriesC[nChartNum] + startIndex, noOfPoints);

		if (noOfPoints < 6000) {
			dLabelDataA = m_dataSeriesA[nChartNum][noOfPoints - 1];
			dLabelDataB = m_dataSeriesB[nChartNum][noOfPoints - 1];
			dLabelDataC = m_dataSeriesC[nChartNum][noOfPoints - 1];
		}
		else {
			dLabelDataA = m_dataSeriesA[nChartNum][endIndex];
			dLabelDataB = m_dataSeriesB[nChartNum][endIndex];
			dLabelDataC = m_dataSeriesC[nChartNum][endIndex];
		}
	}

	//================================================================================
	// Configure overall chart appearance.
	//================================================================================

	// Create an XYChart object of size 640 x 350 pixels
	CRect rt;
	GetDlgItem(IDC_CHART_KEYENCE1)->GetWindowRect(&rt);

	XYChart *c;
	c = new XYChart(rt.Width(), rt.Height(), 0xf4f4f4, 0x000000, 1);
	

	c->setPlotArea(30, 50, rt.Width() - 60, c->getHeight() - 80, 0xffffff, -1, Chart::Transparent)->setGridColor(0xc0c0c0);
	//c->setRoundedFrame();  //그래프 꼭지점을 둥글게


	// As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
	c->setClipping();

	c->addTitle("        Live Thickness (um)", "arial.ttf", 18);



	// Add a legend box at (55, 25) using horizontal layout. Use 10pt Arial Bold as font. Set the
	// background and border color to transparent and use line style legend key.
	LegendBox *b = c->addLegend(55, 25, false, "arialbd.ttf", 10);
	b->setBackground(Chart::Transparent);
	b->setLineStyleKey();

	// Set the x and y axis stems to transparent and the label font to 10pt Arial
	c->xAxis()->setColors(Chart::Transparent);
	c->yAxis()->setColors(Chart::Transparent);
	c->setColors(whiteOnBlackPalette);
	c->xAxis()->setLabelStyle("arial.ttf", 10);
	c->yAxis()->setLabelStyle("arial.ttf", 10);

	// Set the y-axis tick length to 0 to disable the tick and put the labels closer to the axis.
	c->yAxis()->setTickLength(0);
	
	// Add axis title using 12pt Arial Bold Italic font
	c->yAxis()->setTitle("Thickness(um)", "arialbd.ttf", 10);
	c->yAxis()->setLinearScale(-100, 1000, 100);

	// Add a line layer for the lines, using a line width of 2 pixels
	LineLayer *layer = c->addLineLayer();
	layer->setLineWidth(2);
	layer->setFastLineMode();



	// Now we add the 3 data series to a line layer, using the color red (ff0000), green (00cc00)
	// and blue (0000ff)
	layer->setXData(viewPortTimeStamps);

	// 그래프에 두께값 출력 및 그래프에 데이터 넣기
	char buffer1[1024], buffer2[1024], buffer3[1024];
	if (m_bGraphTop == TRUE) {
		//sprintf(buffer1, "Top: <*bgColor=000000*> %.1f ", *viewPortDataSeriesA.data);  //변위1
		sprintf(buffer1, "Top: <*bgColor=000000*> %.1f", dLabelDataA);  //변위1
		layer->addDataSet(viewPortDataSeriesA, 0x0000ff, buffer1);
	}
	if (m_bGraphThickness == TRUE) {
		//sprintf(buffer2, "ThickNess: <*bgColor=000000*> %.1f", *viewPortDataSeriesB.data);  //두께
		sprintf(buffer2, "ThickNess: <*bgColor=000000*> %.1f", dLabelDataB);  //두께
		layer->addDataSet(viewPortDataSeriesB, 0x88000, buffer2);
	}
	if (m_bGraphBottom == TRUE) {
		//sprintf(buffer3, "Bottom: <*bgColor=000000*> %.1f", *viewPortDataSeriesC.data);  //변위2
		sprintf(buffer3, "Bottom: <*bgColor=000000*> %.1f", dLabelDataC);  //변위2
		layer->addDataSet(viewPortDataSeriesC, 0xff8800, buffer3);
	}


	//================================================================================
	// Configure axis scale and labelling
	//================================================================================

	// Set the x-axis as a date/time axis with the scale according to the view port x range.
	if (nindex > 0)
		c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);


	// If all ticks are hour algined, we use "hh:nn<*br*>mmm dd" in bold font as the first label of
	// the Day, and "hh:nn" for other labels.
	c->xAxis()->setFormatCondition("align", 3600);
	c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(), "<*font=bold*>{value|hh:nn<*br*>mmm dd}",
		Chart::AllPassFilter(), "{value|hh:nn}");

	// If all ticks are minute algined, then we use "hh:nn" as the label format.
	c->xAxis()->setFormatCondition("align", 60);
	c->xAxis()->setLabelFormat("{value|hh:nn}");

	// If all other cases, we use "hh:nn:ss" as the label format.
	c->xAxis()->setFormatCondition("else");
	c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

	// We make sure the tick increment must be at least 1 second.
	c->xAxis()->setMinTickInc(1);

	//================================================================================
	// Output the chart
	//================================================================================

	// We need to update the track line too. If the mouse is moving on the chart (eg. if 
	// the user drags the mouse on the chart to scroll it), the track line will be updated
	// in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
	if (!viewer->isInMouseMoveEvent())
	{
		trackLineLabel(c, (0 == viewer->getChart()) ? c->getPlotArea()->getRightX() :
			viewer->getPlotAreaMouseX());
	}

	// Set the chart image to the WinChartViewer
	delete viewer->getChart();
	viewer->setChart(c);
	updateControls(&m_chartKeyence1, 0);
}

void COffLineThicknessDlg::trackLineLabel(XYChart *c, int mouseX)
{
	// Clear the current dynamic layer and get the DrawArea object to draw on it.
	DrawArea *d = c->initDynamicLayer();

	// The plot area object
	PlotArea *plotArea = c->getPlotArea();

	// Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
	double xValue = c->getNearestXValue(mouseX);
	int xCoor = c->getXCoor(xValue);
	if (xCoor < plotArea->getLeftX())
		return;

	// Draw a vertical track line at the x-position
	d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0x888888);

	// Draw a label on the x-axis to show the track line position.
	ostringstream xlabel;
	xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss.ff")
		<< " <*/font*>";
	TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 10);

	// Restrict the x-pixel position of the label to make sure it stays inside the chart image.
	int xLabelPos = max(0, min(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
	t->draw(xLabelPos, plotArea->getBottomY() + 6, 0xffffff);
	t->destroy();

	// Iterate through all layers to draw the data labels
	for (int i = 0; i < c->getLayerCount(); ++i) {
		Layer *layer = c->getLayerByZ(i);

		// The data array index of the x-value
		int xIndex = layer->getXIndexOf(xValue);

		// Iterate through all the data sets in the layer
		for (int j = 0; j < layer->getDataSetCount(); ++j)
		{
			DataSet *dataSet = layer->getDataSetByZ(j);
			const char *dataSetName = dataSet->getDataName();

			// Get the color, name and position of the data label
			int color = dataSet->getDataColor();
			int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

			// Draw a track dot with a label next to it for visible data points in the plot area
			if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) && (color !=
				Chart::Transparent) && dataSetName && *dataSetName)
			{
				d->circle(xCoor, yCoor, 4, 4, color, color);

				ostringstream label;
				label << "<*font,bgColor=" << hex << color << "*> "
					<< c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
				t = d->text(label.str().c_str(), "arialbd.ttf", 15);

				// Draw the label on the right side of the dot if the mouse is on the left side the
				// chart, and vice versa. This ensures the label will not go outside the chart image.
				if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
					t->draw(xCoor + 6, yCoor, 0xffffff, Chart::Left);
				else
					t->draw(xCoor - 6, yCoor, 0xffffff, Chart::Right);

				t->destroy();
			}
		}
	}
}


void COffLineThicknessDlg::InitClass()
{
	m_pKeyence = new CKeyenceSenser(this);
	m_pMotion = new CMotionPAS(this);

	// Keyence 생성
	CRect rect;
	m_pKeyenceParam = new CKeyenceParam(this);
	m_pKeyenceParam->Create(IDD_DLG_KEYENCE_IP, NULL);
	
	//키엔스 오픈 
	m_pKeyence->OpenNavigatorN();  //배상현 선임(집에서 할때)   Return value is m_bRes[i]
	m_pKeyence->resetStartStorage();

	// 모터 오픈
	m_pMotion->InitMotion(m_pKeyenceParam->m_nIP_Adress1, m_pKeyenceParam->m_nIP_Adress2, m_pKeyenceParam->m_nIP_Adress3, m_pKeyenceParam->m_nIP_Adress4, 0);
}


bool COffLineThicknessDlg::OnFullDataRateTimer()
{
	// The current time in millisecond resolution
	//X축 시간 베이스 
	SYSTEMTIME st;
	GetLocalTime(&st);
	double now = Chart::chartTime(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute,
		st.wSecond) + st.wMilliseconds / 1000.0;


	double dataA = 0;
	double dataB = 0;
	double dataC = 0;

	// This is our formula for the random number generator
	do
	{
		//센서 1 data
		dataA = m_pKeyence->KyenceSensorValue.top;
		dataB = m_pKeyence->KyenceSensorValue.ths;
		dataC = m_pKeyence->KyenceSensorValue.btm;

		// In this demo, if the data arrays are full, the oldest 5% of data are discarded.
		if (m_currentIndex >= sampleSize)
		{
			m_currentIndex = sampleSize * 95 / 100 - 1;

			for (int i = 0; i < m_currentIndex; ++i)
			{
				int srcIndex = i + sampleSize - m_currentIndex;

				if (srcIndex < sampleSize) {  //프로그램 죽어서 예외처리함
											  //1번째 차트 데이터
					m_timeStamps[0][i] = m_timeStamps[0][srcIndex];
					m_dataSeriesA[0][i] = m_dataSeriesA[0][srcIndex];
					m_dataSeriesB[0][i] = m_dataSeriesB[0][srcIndex];
					m_dataSeriesC[0][i] = m_dataSeriesC[0][srcIndex];
				}
			}
		}

		// Store the new values in the current index position, and increment the index.	// The timestamps for the data series

		m_timeStamps[0][m_currentIndex] = m_nextDataTime;  //시간



		/************************************************************/
		//변위1
		m_dataSeriesA[0][m_currentIndex] = dataA;

		//if (abs(m_dataSeriesA[0][m_currentIndexBefore]) > 2000) {
		//	m_dataSeriesA[0][m_currentIndexBefore] = m_dataSeriesA[0][m_currentIndexBefore - 1];
		//}
		/*************************************************************/

		/*************************************************************/
		//두께
		m_dataSeriesB[0][m_currentIndex] = dataB;

		//if (abs(m_dataSeriesB[0][m_currentIndexBefore]) > 2000 || m_dataSeriesB[0][m_currentIndexBefore] < -500) {
		//	m_dataSeriesB[0][m_currentIndexBefore] = m_dataSeriesB[0][m_currentIndexBefore - 1];
		//}
		/*************************************************************/

		//변위2
		m_dataSeriesC[0][m_currentIndex] = dataC;

		//if (abs(m_dataSeriesC[0][m_currentIndexBefore]) > 2000) {
		//	m_dataSeriesC[0][m_currentIndexBefore] = m_dataSeriesC[0][m_currentIndexBefore - 1];
		//}


		++m_currentIndex;

		m_nextDataTime += DataInterval / 1000.0;  //그래프 쉬프트되는 위치 조절

	} while (m_nextDataTime < now);

	return 0;

}

UINT COffLineThicknessDlg::OnChartUpdateTimer(LPVOID param)  //오븐 전
{
	COffLineThicknessDlg* pMain = (COffLineThicknessDlg*)param;
	if (pMain->m_currentIndex > 0)
	{
		//
		// As we added more data, we may need to update the full range of the viewport. 
		//

		double startDate = pMain->m_timeStamps[0][0];
		double endDate = pMain->m_timeStamps[0][pMain->m_currentIndex - 1];

		// Use the initialFullRange (which is 60 seconds in this demo) if this is sufficient.
		double duration = endDate - startDate;
		if (duration < initialFullRange)
			endDate = startDate + initialFullRange;

		// Update the full range to reflect the actual duration of the data. In this case, 
		// if the view port is viewing the latest data, we will scroll the view port as new
		// data are added. If the view port is viewing historical data, we would keep the 
		// axis scale unchanged to keep the chart stable.
		int updateType = Chart::ScrollWithMax;
		if (pMain->m_chartKeyence1.getViewPortLeft() + pMain->m_chartKeyence1.getViewPortWidth() < 0.999)
			updateType = Chart::KeepVisibleRange;
		bool scaleHasChanged = pMain->m_chartKeyence1.updateFullRangeH("x", startDate, endDate, updateType);

		// Set the zoom in limit as a ratio to the full range
		pMain->m_chartKeyence1.setZoomInWidthLimit(zoomInLimit / (pMain->m_chartKeyence1.getValueAtViewPort("x", 1) -
			pMain->m_chartKeyence1.getValueAtViewPort("x", 0)));

		// Trigger the viewPortChanged event to update the display if the axis scale has changed 
		// or if new data are added to the existing axis scale.
		if (scaleHasChanged || (duration < initialFullRange))
			pMain->m_chartKeyence1.updateViewPort(true, false);


	}
	return 0;
}

UINT COffLineThicknessDlg::OnStaticChartUpdate(LPVOID param) {

	COffLineThicknessDlg* pMain = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;
	double startDate = pMain->m_HzStamps[0];
	double endDate = pMain->m_HzStamps[pMain->m_currentIndex_static - 1];

	// Use the initialFullRange (which is 60 seconds in this demo) if this is sufficient.
	double duration = endDate - startDate;
	if (duration < initialFullRange)
		endDate = startDate + initialFullRange;

	// Update the full range to reflect the actual duration of the data. In this case, 
	// if the view port is viewing the latest data, we will scroll the view port as new
	// data are added. If the view port is viewing historical data, we would keep the 
	// axis scale unchanged to keep the chart stable.
	int updateType = Chart::ScrollWithMax;
	if (pMain->m_chartKeyence2.getViewPortLeft() + pMain->m_chartKeyence2.getViewPortWidth() < 0.999)
		updateType = Chart::KeepVisibleRange;
	bool scaleHasChanged = pMain->m_chartKeyence2.updateFullRangeH("x", startDate, endDate, updateType);
	int zoomInLimit1 = 10;
	// Set the zoom in limit as a ratio to the full range
	pMain->m_chartKeyence2.setZoomInWidthLimit(zoomInLimit / (pMain->m_chartKeyence2.getValueAtViewPort("x", 1) -
		pMain->m_chartKeyence2.getValueAtViewPort("x", 0)));

	// Trigger the viewPortChanged event to update the display if the axis scale has changed 
	// or if new data are added to the existing axis scale.
	if (scaleHasChanged || (duration < initialFullRange))
		pMain->m_chartKeyence2.updateViewPort(true, false);
	return 0;

}

void COffLineThicknessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nIDEvent == WM_TIMER_CHART_UPDATE) {
		m_actualPosThread  = AfxBeginThread(ActualPos, this);
		m_PosThread =  	AfxBeginThread(CommandPos,this);

		if (m_nCount == 1)
			{
				KillTimer(WM_TIMER_STORAGE_READ);
				if (m_KCount == 1 && m_pMotion->getAPosition(0) > m_dStartPosition - 2 && m_pMotion->getAPosition(0) < m_dStartPosition + 2)
				{
					OnStaticChartUpdate(this);
					CommandPos(this);
					m_pKeyence->temp = m_CPosition;
					const LONG returnStorageValue = CL3IF_GetTrendIndex(0, &m_pKeyence->m_nIndex_TrendData);

					m_pKeyence->m_index = 0;
					m_pKeyence->GetTrandDataThread();
					m_pKeyence->nOldMeter = m_dStartPosition; // shu yeda ham OldMeter qara ? 

					//m_pKeyence.Ip = 0;

					m_KCount = 0;
				}
			}
			 if (m_dEndPosition < m_pMotion->getAPosition(0))
			{


				OnStaticDataRateTimer(this);
				OnStaticChartUpdate(this);
			//	m_TextThread = AfxBeginThread(TestDateSave, this);
				TestDateSave(0);
				//Sleep(100);
				m_nCount = 0;
				SetTimer(WM_TIMER_STORAGE_READ, 100, 0);
			}	

		 OnChartUpdateTimer(this);
	}
	if (nIDEvent== WM_TIMER_MOTIOM_UPDATE) {
		
	}
	if (nIDEvent == WM_TIMER_STORAGE_READ) {
		m_pKeyence->StorageDataThread();
		UpdateEditValue();
	}

	CDialogEx::OnTimer(nIDEvent);
}

UINT COffLineThicknessDlg::TestDateSave(LPVOID param)
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;
	CKeyenceSenser* nKey = (CKeyenceSenser*)param;
	CString strMD, strMDMS;
	CTime t = CTime::GetCurrentTime();

	strMD.Format(_T("%02d%02d"), t.GetMonth(), t.GetDay());
	strMDMS.Format(_T("%02d%02d_%02d%02d%02d"), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	
	CString strPath; 
	strPath = "c:\\glim\\data\\" + strMD;
	CreateDirectory(strPath, NULL);

	CString strfileName = strPath + "\\" + strMDMS + ".txt";
	
	ofstream file(strfileName);
	file << "StartPoint: " << pDlg->m_dStartPosition << " mm" << "\t" << "EndPoint: " << pDlg->m_dEndPosition << " mm" << "\t" << "Velocity: " << (float)pDlg->m_dVelocity << " mm/sec" << endl;
	file << endl;
	file << endl;
	CString strst;

	for (int i = 0; i < pDlg->m_static_test_index ; i++)
	{
		file << (float)pDlg->m_HzStamps[i] << "\t"<< (float)pDlg->m_testSeriesD[i] << endl;
	}

	pDlg->DataLoad();
	Sleep(1000);// 
	file.close();
	pDlg->m_pKeyence->resetStartStorage();
	return 0;

}

UINT COffLineThicknessDlg::OnStaticDataRateTimer(LPVOID param)
{
	COffLineThicknessDlg * pDlg = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;
	// This is our formula for the random number generator
	pDlg->m_currentIndex_static = 0;

	pDlg->m_static_test_index = 0;
	string hzText;
	struct tm buf;

	for (int i = 0; i < 40000; ++i)
	{
		pDlg->m_dataSeriesB1[i] = Chart::NoValue;
	}


	CString Thick;
	CString Pos;

	for (int j = 0; j <= pDlg->m_pKeyence->m_index; j++) {

		pDlg->m_dataSeriesB1[pDlg->m_currentIndex_static] = pDlg->m_pKeyence->m_data[j];
		if (pDlg->m_dataSeriesB1[pDlg->m_currentIndex_static] < 0)
		{
			pDlg->m_dataSeriesB1[pDlg->m_currentIndex_static] = pDlg->m_dataSeriesB1[pDlg->m_currentIndex_static - 1];
		}
		pDlg->m_currentIndex_static ++;
	}

	for (int i = 0; i < pDlg->m_pKeyence->m_index; i++)
	{
		pDlg->m_testSeriesD[pDlg->m_static_test_index] = pDlg->m_pKeyence->m_data[i];

		if (pDlg->m_testSeriesD[pDlg->m_static_test_index] < 0)
		{
			pDlg->m_testSeriesD[pDlg->m_static_test_index] = pDlg->m_testSeriesD[pDlg->m_static_test_index-1];
		}

		pDlg->m_static_test_index++;

	}

	return 0;
}

UINT COffLineThicknessDlg::ActualPos(LPVOID param)
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;
	double value;
	value = pDlg->m_pMotion->getAPosition(0);
	CString strvalue;
	
	
	strvalue.Format(_T("%.2f"), value);
	pDlg->SetDlgItemText(IDC_EDIT_ACTUALPOS, strvalue);

	return 0;
}


UINT COffLineThicknessDlg::CommandPos(LPVOID param)
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;

	pDlg->m_CPosition = pDlg->m_pMotion->getCPosition(0);
	CString strvalue;

	strvalue.Format(_T("%.2f"), pDlg->m_CPosition);
	pDlg->SetDlgItemText(IDC_EDIT_COMMANDPOS, strvalue);
	return 0;
}

void COffLineThicknessDlg::UpdateEditValue() {

	CString strTop, strThickness, strBottom;
	
	double dataA = m_pKeyence->KyenceSensorValue.top;
	double dataC = m_pKeyence->KyenceSensorValue.btm;
	double dataB = m_pKeyence->KyenceSensorValue.ths;


	strTop.Format(_T("Top : %.3f"), dataA);
	SetDlgItemText(IDC_EDIT_TOP, strTop);

	strThickness.Format(_T("Thickness : %.3f"), dataB);
	SetDlgItemText(IDC_EDIT_THICKNESS, strThickness);

	strBottom.Format(_T("Bottom : %.3f"), dataC);
	SetDlgItemText(IDC_EDIT_BOTTOM, strBottom);


}

void COffLineThicknessDlg::OnBnClickedCheckGraphTop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void COffLineThicknessDlg::OnBnClickedCheckGraphBottom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}


void COffLineThicknessDlg::OnBnClickedCheckGraphThickness()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
}

void COffLineThicknessDlg::UpdateSetting(bool bLoad)
{
	if (!bLoad)	//저장
	{
		UpdateData();
	}

	CIni ini(SETTING_PATH, "Setting", 1);

	//그래프 표시
	ini.SER_GETD(bLoad, m_bGraphTop,		FALSE);
	ini.SER_GETD(bLoad, m_bGraphThickness,	TRUE);
	ini.SER_GETD(bLoad, m_bGraphBottom,		FALSE);

	//그래프 설정
	ini.SER_GETD(bLoad, m_nGraphMin, -50);
	ini.SER_GETD(bLoad, m_nGraphMax, 200);
	ini.SER_GETD(bLoad, m_dZeroSet, 200);

	//테스트 속도, 거리,이동 거리
	ini.SER_GETD(bLoad, m_dStartPosition, 35);
	ini.SER_GETD(bLoad, m_dEndPosition, 85);
	ini.SER_GETD(bLoad, m_dVelocity, 17.5);
	
	ini.SER_GETD(bLoad, m_dMovePos, 40.000);


	if (bLoad)
	{
		UpdateData(FALSE);
	}

}

void COffLineThicknessDlg::OnBnClickedButtonParamSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateSetting(FALSE);
}

void COffLineThicknessDlg::ListControl()
{
	CRect frt;
	m_ListReport.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ListReport.GetWindowRect(&frt);
	m_ListReport.InsertColumn(0, TEXT("Index"), LVCFMT_CENTER, frt.Width() * 0.05);
	m_ListReport.InsertColumn(1, TEXT("FileName"), LVCFMT_CENTER, frt.Width() * 0.2);
	m_ListReport.InsertColumn(2, TEXT("Position"), LVCFMT_CENTER, frt.Width());
}

void COffLineThicknessDlg::OnBnClickedBtnFilePath()
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장됭 폴더를 선택하세요.");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemdList, szBuffer);

	m_fileName.Format(_T("%s"), szBuffer);
	
	SetDlgItemText(IDC_EDIT_FILE_PATH, m_fileName);

	AddtoList(m_fileName);

	DataLoad();

}


void COffLineThicknessDlg::OnBnClickedBtnDefault()
{
	CString strDefaultPath;
	CString strPath, strTime;
	CTime t = CTime::GetCurrentTime();
	strPath.Format(_T("%02d%02d"), t.GetMonth(), t.GetDay());
	strTime.Format(_T("%02d%02d_%02d%02d%02d"), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	m_fileName.Format(_T("C:\\glim\\data\\1228"));
	SetDlgItemText(IDC_EDIT_FILE_PATH, m_fileName);
	DataLoad();
}


void COffLineThicknessDlg::DataLoad()
{
	m_ListReport.DeleteAllItems();
	m_FlistControlIndex = 0;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(m_fileName + "\\*.txt");
	CString tFileName;
	CString index;
	CString _fileName;


	while (bWorking)
	{
		index.Format(_T("%d"), m_FlistControlIndex);
		bWorking = finder.FindNextFile();
		if (finder.IsArchived())
		{

			_fileName = finder.GetFileName();
			tFileName = finder.GetFileTitle();
			m_ListReport.InsertItem(m_FlistControlIndex, index);
			m_ListReport.SetItemText(m_FlistControlIndex, 1, tFileName);
			m_FlistControlIndex += 1;

		}
	}

}

void COffLineThicknessDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString str, strReturn;
	int iItem, iPrev = -1;
	if (m_ListReport.GetItemCount() != 0) {
		iItem = m_ListReport.GetSelectionMark();
	}
	strReturn = m_ListReport.GetItemText(iItem,1);
	str.Format(_T("%d"), iItem);
	/*AddtoList(strReturn);*/
	DrawOldDataStaticChart(strReturn);
}
void COffLineThicknessDlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	*pResult = 0;
}


void COffLineThicknessDlg::OnBnClickedBtnText()
{
	
	for (int i = 0; i < 40000; ++i)
		{
			m_HzStamps[i] = m_pKeyence->m_data[i] = m_dataSeriesB1[i] = Chart::NoValue;
		}

		CString start;

		UpdateSetting(FALSE);
		

		if ((float)m_CPosition != (float)m_dStartPosition - 10.0)
		{
			m_pMotion->moveS(0, (m_dStartPosition - 10) * 1000, 40000);
			Sleep(5000);
		}

		m_nCount = 1; //  n_count = 1da Ontimer 실행 시킴, 기존 0이다.
		m_KCount = 1;

		m_pMotion->moveS(0, ((int)m_dEndPosition + 10) * 1000, m_dVelocity*1000);
		m_pMotion->moveS(0, ((int)m_dStartPosition - 10) * 1000, m_dVelocity*1000);

		//Hz = 0;

}

void COffLineThicknessDlg::OnBnClickedBtnZeroset()
{
	//CIGlimYSDlg* pDlg = (CIGlimYSDlg*)m_pParent;
	UpdateData(TRUE);

	BYTE programNo;
	LONG offset = (LONG)m_dZeroSet* 10;
	LONG upperLimit = 99999;
	LONG lowerLimit = 99999;
	LONG hysteresis = 99999;


	CL3IF_StopStorage(0);
	CL3IF_GetProgramNo(0, &programNo);
	CL3IF_AutoZeroMulti(0, CL3IF_OUTNO_ALL, TRUE);

	CL3IF_SetTolerance(0, programNo, CL3IF_OUTNO_ALL, upperLimit, lowerLimit, hysteresis);
	CL3IF_SetOffset(0, programNo, CL3IF_OUTNO_01, offset / 2);
	CL3IF_SetOffset(0, programNo, CL3IF_OUTNO_02, offset / 2);
	CL3IF_StartStorage(0);

	UpdateSetting(FALSE);
}

void COffLineThicknessDlg::DrawOldDataStaticChart(CString strPath)
{
	CString filePath1, FullFilePath;
	int countSpace = 0;
	m_pKeyence->m_index = 0;
	for (int i = 0; i < 40000; ++i)
	{
		m_HzStamps[i] = m_pKeyence->m_data[i] = m_dataSeriesB1[i] = Chart::NoValue;
	}

	//filePath1 = m_folderReport.GetItemText(i, 1);
	FullFilePath = m_fileName + "\\" + strPath + ".txt";
	ifstream file(FullFilePath);
	while (!file.eof())
	{
		char arr[256];
		file.getline(arr, 256);
		countSpace += 1;
		if (countSpace>3)
		{

			char* PositionArr;
			char* ThicknessArr;
			strtok_s(arr, "\t", &PositionArr);
			m_HzStamps[m_pKeyence->m_index] = atof(arr);
			m_pKeyence->m_data[m_pKeyence->m_index] = atof(PositionArr);
			m_pKeyence->m_index += 1;

				}
			
			int a = 0;
		}
	
	m_pKeyence->m_index -= 3;
	OnStaticDataRateTimer(this);
	OnStaticChartUpdate(this);
}

void COffLineThicknessDlg::OnBnClickedButton1()
{
	m_pMotion->moveEmergencyStop(0);

}


void COffLineThicknessDlg::OnBnClickedOk()
{
	if (AfxMessageBox("프로그램 종료 하겠습니까?", IDOK) == IDOK)
	{
		m_pKeyence->StopStorage();
		AfxGetMainWnd()->DestroyWindow();
		delete m_pKeyence;
		delete m_pKeyenceParam;
		delete m_pMotion;
		


		//	CKeyenceSenser*	 m_pKeyence;
		//CMotionPAS* m_pMotion;
		//CKeyenceParam* m_pKeyenceParam;
		////차트
		//CChartViewer	m_chartKeyence1; //차트1
		//								 //MotionPAS m_pMotion; //모션 개체 
		//CScrollBar		m_HScrollBar;	 //스크롤바

		//CScrollBar m_HScrollBar2;
		//CChartViewer m_chartKeyence2;
	}
	
}



BOOL COffLineThicknessDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;

	GetClientRect(&rect);
	pDC->FillSolidRect(rect,RGB(227,233,255));

	return TRUE; //  CDialogEx::OnEraseBkgnd(pDC);
}


void COffLineThicknessDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

}


HBRUSH COffLineThicknessDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDD_OFFLINETHICKNESS_DIALOG)
	{
		return hbr;
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_CONNECT || pWnd->GetDlgCtrlID() == IDC_STATIC_PARAM || pWnd->GetDlgCtrlID() == IDC_STATIC_VELOCITY || pWnd->GetDlgCtrlID() == IDC_STATIC_TEST) {
		pDC->SetBkColor(RGB(227, 233, 255));
		//pDC->SetBkMode(TRANSPARENT);  //그룹 박스는 이거 해주면 안됨
		pDC->SetTextColor(RGB(0, 0, 0));
		return(HBRUSH)GetStockObject(NULL_BRUSH);
	}

	//텍스트 컨트롤
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkColor(RGB(220, 240, 180));
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		

		return(HBRUSH)GetStockObject(NULL_BRUSH);
	}


	return hbr;
}


BOOL COffLineThicknessDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_TAB) {
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void COffLineThicknessDlg::OnBnClickedKeyenceIp()
{
	if (m_pKeyenceParam->IsWindowVisible())
	{
		m_pKeyenceParam->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pKeyenceParam->ShowWindow(SW_SHOW);
	}
}



void COffLineThicknessDlg::OnMouseMovePlotArea()
{
	trackLineLabel((XYChart*)m_chartKeyence1.getChart(), m_chartKeyence1.getPlotAreaMouseX());
	m_chartKeyence1.updateDisplay();
}

void COffLineThicknessDlg::OnMouseMovePlotArea_Static()
{
	trackLineLabel((XYChart*)m_chartKeyence2.getChart(), m_chartKeyence2.getPlotAreaMouseX());
	m_chartKeyence2.updateDisplay();
}


void COffLineThicknessDlg::MovePlus() {
	CString str;
	int nTestNum;
	GetDlgItemText(IDC_EDIT_JOG_MOVE, str);
	nTestNum = (int)(_ttof(str) * 1000);
	DWORD Velo = nTestNum;
	m_pMotion->moveV(0, 1, Velo);

}

void COffLineThicknessDlg::Movestop() {
	m_pMotion->moveStop(0);
}

void COffLineThicknessDlg::MovePrev() {
	CString str;
	GetDlgItemText(IDC_EDIT_JOG_MOVE, str);
	int nTestNum;
	nTestNum = (int)(_ttof(str) * 1000);
	DWORD Velo = nTestNum;
	m_pMotion->moveV(0, 0, Velo);
}
