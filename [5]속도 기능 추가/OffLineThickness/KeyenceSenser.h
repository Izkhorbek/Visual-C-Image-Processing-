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

	//Ű���� �����Լ��� ����ϴ� ����

	MeasurementData measurementData = {};
	MeasurementData measurementData2 = {};

	//��� ������ �޾ƿ��� ����
	//���� ��
	DWORD m_sequenceTrendIndex;
	volatile bool m_isTrending;
	volatile HANDLE m_threadTrend;
	std::vector<MeasurementData> m_sequenceTrendData;
	int m_nFullDataCount;
	int m_nAfterErrorCount;

	//���� ��
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

	CString *m_DiffDataSave[2];    //���� ��, �� ���� ������
	
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
	void resetStartStorage();  //���丮�� ������ ����->����->����
	void StopStorage();
	void StorageDataThread();  //���丮�� ������ ���� ��� ������
	void GetTrandDataThread(); //��� ������ �ٴٿ��� ������ ����
	static UINT	StorageReadData(LPVOID param);  //����1 �������� ������ �ҷ�����


	bool OpenNavigatorN();  //��� Open
	void CloseNavigatorN(); //��� Close
	void KeyenceAutoZeroSingle(int nDevice);  //���μ�
	void KeyenceZeroSetOffSet(int nDevice);

	void StopTrendProcessAfter();
	void StopTrendProcessBefore();

	// ��� ���� �׸�
	
	DWORD m_nIndex_TrendData = 0; // nIndexStt1 in IGlimYS
	float temp; // ��� �̵� �� �� ����
	
private:
	static UINT __cdecl ContinuouslyExecuteTrendProcessBefore(LPVOID pParam);
	void ContinuouslyExecuteTrendProcessBefore();
	BOOL SensorOpenSignal;
	

	
	

};


