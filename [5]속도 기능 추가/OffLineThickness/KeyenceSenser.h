#pragma once
#include <vector>
#include "CL3_IF.h"  
#include "pch.h"

// CKeyenceSenser
#define KEYENCE_DEVICE_COUNT 1

const int MaxRequestDataLength=512000;

// Maximum number of sequential acquired data - 10 seconds at sampling cycle 100us(fastest)
const int MaxSequenceMeasurementData = 100000;

// Maximum number of acquired data per time
const int MaxMeasurementDataCountPerTime = 8000;

#define MAXCOUNT 40000

struct MeasurementData
{
	CL3IF_ADD_INFO addInfo;
	std::vector<CL3IF_OUTMEASUREMENT_DATA> outMeasurementData;
};
//
//typedef struct _Sensor {
//	double top = 0;
//	double btm = 0;
//	double ths = 0;
//}Sensor;

class CKeyenceSenser : public CWnd
{
	DECLARE_DYNAMIC(CKeyenceSenser)

public:
	CKeyenceSenser(CWnd* pParent = NULL);
	virtual ~CKeyenceSenser();
	CWnd*	m_pParent;	
	double nOldMeter = 0;
	LONG m_nDeviceID;
	LONG m_nDeviceID2;

	bool m_bRes[KEYENCE_DEVICE_COUNT];

	//키엔스 측정함수에 사용하는 변수

	MeasurementData measurementData = {};
	MeasurementData measurementData2 = {};

	//모든 데이터 받아오는 변수
	//오븐 후
	DWORD m_sequenceTrendIndex;
	volatile bool m_isTrending;
	volatile HANDLE m_threadTrend;
	std::vector<MeasurementData> m_sequenceTrendData;
	int m_nFullDataCount;
	int m_nAfterErrorCount;

	//오븐 전
	DWORD m_sequenceTrendIndexBefore;

	volatile bool m_isTrendingBefore;
	volatile HANDLE m_threadTrendBefore;
	std::vector<MeasurementData> m_sequenceTrendDataBefore;
	int m_nFullDataCountBefore;
	int m_nBeforeErrorCount;


	double *m_dBeforeKeyenceData[2][3];   //  0: Thickness, 1: Top, 2: Bottom
	int    m_nBeforeSaveDataDir;
	double *m_dAfterKeyenceData[2][3];    //  0: Thickness, 1: Top, 2: Bottom
	int    m_nAfterSaveDataDir;

	CString *m_DiffDataSave[2];    //오븐 전, 후 차이 데이터
	
	Sensor KyenceSensorValue;
	Sensor OldKyenceSensorValue;

	Sensor KyenceSensorValue2;
	Sensor OldKyenceSensorValue2;


protected:
	std::vector<MeasurementData> m_storageData;
	DECLARE_MESSAGE_MAP()
public:
	// 211224 
	int m_index = 0;
	double m_data[40000];

	CWinThread* m_KyenceThread1;
	void resetStartStorage();  //스토리지 데이터 중지->리셋->시작
	void StopStorage();
	void StorageDataThread();  //스토리지 데이터 시작 명령 쓰레드
	void GetTrandDataThread(); //모든 데이터 바다오는 쓰레드 실행
	static UINT	StorageReadData(LPVOID param);  //센서1 스토이지 데이터 불러오기


	bool OpenNavigatorN();  //통신 Open
	void CloseNavigatorN(); //통신 Close
	void KeyenceAutoZeroSingle(int nDevice);  //제로셋
	void KeyenceZeroSetOffSet(int nDevice);

	void StopTrendProcessAfter();
	void StopTrendProcessBefore();

	// 모션 관련 항목
	
	DWORD m_nIndex_TrendData = 0; // nIndexStt1 in IGlimYS
	float temp; // 모션 이동 떄 쓸 것음
	
private:
	static UINT __cdecl ContinuouslyExecuteTrendProcessBefore(LPVOID pParam);
	void ContinuouslyExecuteTrendProcessBefore();
	BOOL SensorOpenSignal;
	

	
	

};


