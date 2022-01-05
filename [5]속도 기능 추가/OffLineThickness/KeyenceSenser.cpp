// KeyenceSenser.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "OffLineThickness.h"
#include "KeyenceSenser.h"
#include "OffLineThicknessDlg.h"



// CKeyenceSenser

IMPLEMENT_DYNAMIC(CKeyenceSenser, CWnd)

namespace OutNoConverter
{
	CString CreateTargetOutSequence(const std::vector<int>& outTargetList);
	std::vector<int> ConvertOutTargetList(CL3IF_OUTNO outTarget);
}

std::vector<int> OutNoConverter::ConvertOutTargetList(CL3IF_OUTNO outTarget)
{

	BYTE mask = 1;
	std::vector<int> outList;
	for (int i = 0; i < CL3IF_MAX_OUT_COUNT; i++)
	{
		if ((outTarget & mask) != 0)
		{
			outList.push_back(i + 1);
		}
		mask = static_cast<BYTE>(mask << 1);
	}
	return outList;
}

CKeyenceSenser::CKeyenceSenser(CWnd* pParent)
{
	m_pParent = pParent;

	m_nDeviceID = 0;
	m_nDeviceID2 = 1;


	m_bRes[0] = FALSE;
	m_bRes[1] = FALSE;

	//���� �� ���� �ʱ�ȭ
	m_sequenceTrendIndex = 0;
	m_isTrending = false;
	m_threadTrend = INVALID_HANDLE_VALUE;
	m_nFullDataCount = 0;
	m_nAfterErrorCount = 0;

	//���� �� ���� �ʱ�ȭ
	m_sequenceTrendIndexBefore = 0;
	m_isTrendingBefore = false;
	m_threadTrendBefore = INVALID_HANDLE_VALUE;
	m_nFullDataCountBefore = 0;
	m_nBeforeErrorCount = 0;


	//������ ���� ����
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 3; i++) {
			m_dBeforeKeyenceData[j][i] = new double[MAXCOUNT];
			m_dAfterKeyenceData[j][i] = new double[MAXCOUNT];
		}
	}

	for (int i = 0; i < 2; i++) {
		m_DiffDataSave[i] = new CString[MAXCOUNT];
	}

	//������ ���� �÷���
	m_nAfterSaveDataDir = 0;
	m_nBeforeSaveDataDir = 0;

	
}

CKeyenceSenser::~CKeyenceSenser()
{
}


BEGIN_MESSAGE_MAP(CKeyenceSenser, CWnd)
END_MESSAGE_MAP()



// CKeyenceSenser �޽��� ó�����Դϴ�.

bool CKeyenceSenser::OpenNavigatorN()
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)m_pParent;

	CL3IF_ETHERNET_SETTING ethernetSetting[KEYENCE_DEVICE_COUNT] = {};
	m_bRes[KEYENCE_DEVICE_COUNT] = {0,};

	//IP����
	for(int i=0; i<KEYENCE_DEVICE_COUNT; i++){
		ethernetSetting[i].ipAddress[0] = static_cast<BYTE>(pDlg->m_pKeyenceParam->m_nSensor_IP1);
		ethernetSetting[i].ipAddress[1] = static_cast<BYTE>(pDlg->m_pKeyenceParam->m_nSensor_IP2);
		ethernetSetting[i].ipAddress[2] = static_cast<BYTE>(pDlg->m_pKeyenceParam->m_nSensor_IP3+i);
		ethernetSetting[i].ipAddress[3] = static_cast<BYTE>(pDlg->m_pKeyenceParam->m_nSensor_IP4);
		ethernetSetting[i].portNo = static_cast<WORD>(pDlg->m_pKeyenceParam->m_nSensor_Port);

		//����̽�ID
		int deviceID = 0;
		CString strTemp;
		ethernetSetting[i].reserved[0] = 0x00;
		ethernetSetting[i].reserved[1] = 0x00;

		const LONG returnCode = CL3IF_OpenEthernetCommunication(i, &ethernetSetting[i], 10000);  //����:0, ����:105

		if(returnCode == 0){
			strTemp.Format(_T("Keyence Open success (%d.%d.%d.%d_%d)"), ethernetSetting[i].ipAddress[0], ethernetSetting[i].ipAddress[1], ethernetSetting[i].ipAddress[2], ethernetSetting[i].ipAddress[3], ethernetSetting[i].portNo);
			m_bRes[i] = TRUE;
			//break;
		}
		else{
			strTemp.Format(_T("Keyence Open Fail (%d.%d.%d.%d_%d)"), ethernetSetting[i].ipAddress[0], ethernetSetting[i].ipAddress[1], ethernetSetting[i].ipAddress[2], ethernetSetting[i].ipAddress[3], ethernetSetting[i].portNo);
			m_bRes[i] = FALSE;
			AfxMessageBox("���� ���� �ȵ�");
		}	
		pDlg->AddtoList(strTemp);
	}

	return m_bRes;
}

void CKeyenceSenser::CloseNavigatorN() {
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)m_pParent;

	CString strState;
	
	const LONG returnCode = CL3IF_CloseCommunication(m_nDeviceID);
	if (returnCode == 0) {
		strState.Format("Keyencce Device_%d �������� ����", m_nDeviceID);	
	}
	else {
		strState.Format("Keyencce Device_%d �������� ����", m_nDeviceID);
	}
	pDlg->AddtoList(strState);
	
}

void CKeyenceSenser::KeyenceZeroSetOffSet(int nDevice)
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)m_pParent;

	BYTE programNo = 0;
	BYTE outNo = 0;
	CString strOffSet;
	CString str;
	double dThickness;
	

	GetDlgItemText(IDC_EDIT_ZEOSET_THICKNESS, str);

	dThickness = atof(str);
	LONG OffSet = dThickness*10000;

	//LONG OffSet = pDlg->m_pDlgpKeyence->m_dZeroSetOffset[nDevice]*10000;  //OffSet���� 999999�϶�, �ּ� ǥ�ô��� 0.0001mm =>  99.9999mm

	const LONG returnCode = CL3IF_SetOffset(nDevice, programNo, outNo, OffSet);

	if (returnCode == CL3IF_RC_OK) {
		strOffSet.Format("OffSet ���� ����(%.1d um)_(%d)", OffSet/10, nDevice);
		pDlg->AddtoList(strOffSet);
	}
	else {
		strOffSet.Format("OffSet ���� ����_(%d)", nDevice);
		pDlg->AddtoList(strOffSet);
	}
}

void CKeyenceSenser::KeyenceAutoZeroSingle(int nDevice)
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)m_pParent;

	BYTE outNo = 0;
	CString str;


	CL3IF_StopStorage(nDevice);  //OffSet������ ���丮�� �ߴ�
	
	KeyenceZeroSetOffSet(nDevice);  //ZeroSet OffSet

	Sleep(1000);
	const LONG returnCode = CL3IF_AutoZeroSingle(nDevice, outNo, TRUE);  //ù��° ����

	
	//���μ� ���� �ٽ� ���丮�� ������ ��� ����
	CL3IF_ClearStorageData(nDevice);
	CL3IF_StartStorage(nDevice);
	m_storageData.clear();

	if (returnCode == 0) {
		str.Format("Keyence Zero Set OK (%d)", nDevice);
		pDlg->AddtoList(str);
	}
	else {
		str.Format("Keyence Zero Set Fail (%d)", nDevice);
		pDlg->AddtoList(str);
	}
}

void CKeyenceSenser::GetTrandDataThread()   // ��� ������ �޾ƿ��� ���
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)m_pParent;

	AfxBeginThread(ContinuouslyExecuteTrendProcessBefore, this);  //������ ��� ����(���� ��)
	
}

UINT CKeyenceSenser::ContinuouslyExecuteTrendProcessBefore(LPVOID pParam)
{
	CKeyenceSenser* pDlg1 = (CKeyenceSenser*)pParam;
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)pDlg1->m_pParent;

	float temp1 = pDlg->m_CPosition;
	//Sensor1 sensor1;
	Sensor sensor;
	sensor.top = 0;
	sensor.btm = 0;
	sensor.ths = 0;
	int nSttTriggerCount = 0, nEndTriggerCount;

	DWORD nCount = 0;

	pDlg1->nOldMeter = pDlg->m_dStartPosition; //35 

	const int MaxRequestDataLength = 100000;
	DWORD MaxMeasurementDataCountPerTime = 100000;
	DWORD nextIndex = 1;
	DWORD obtainedDataCount = 0;
	DWORD obtainedDataCount1 = 10;
	CL3IF_OUTNO outTarget = {};
	BYTE buffer[MaxRequestDataLength] = {};
	BYTE buffer1[MaxRequestDataLength] = {};
	DWORD m_data11[40000];
	int	nCount1 = 0;
	while (TRUE)
	{
			pDlg1->temp = pDlg->m_pMotion->getCPosition(0);

		const LONG returnCodeStorageData = CL3IF_GetTrendData(pDlg1->m_nDeviceID, pDlg1->m_nIndex_TrendData, MaxRequestDataLength,
			&nextIndex, &obtainedDataCount, &outTarget, reinterpret_cast<DWORD*>(buffer));
		pDlg1->m_nIndex_TrendData = nextIndex;



		int Old_m_index = pDlg1->m_index;//30 40
		
		std::vector<int> outTargetList = OutNoConverter::ConvertOutTargetList(outTarget);
		pDlg1->m_storageData.resize(obtainedDataCount + nCount);

		const BYTE* pReadPosition = buffer;
		for (DWORD i = 0; i < obtainedDataCount; i++)
		{
			pDlg1->measurementData = {};
			pDlg1->measurementData.outMeasurementData.resize(outTargetList.size());
			pDlg1->measurementData.addInfo = *reinterpret_cast<const CL3IF_ADD_INFO*>(pReadPosition);
			pReadPosition += sizeof(CL3IF_ADD_INFO);
			m_data11[pDlg1->m_index] = pDlg1->measurementData.addInfo.triggerCount;
			for (unsigned int j = 0; j < outTargetList.size(); j++)
			{
				pDlg1->measurementData.outMeasurementData[j] = *reinterpret_cast<const CL3IF_OUTMEASUREMENT_DATA*>(pReadPosition);
				pReadPosition += sizeof(CL3IF_OUTMEASUREMENT_DATA);
			}
			if (pDlg->m_nCount == 1)
			{
				//pDlg1->m_data[pDlg1->m_index] = (pDlg1->measurementData.outMeasurementData[1].measurementValue + pDlg1->measurementData.outMeasurementData[2].measurementValue )/ 10;
				pDlg1->m_data[pDlg1->m_index] = pDlg1->measurementData.outMeasurementData[0].measurementValue / 10;

				pDlg1->m_index += 1; // nechta qiymat olgani 
			}
		}

		double m_count = 0;

		double pre = ((double)pDlg1->temp - (double)pDlg1->nOldMeter) / ((double)obtainedDataCount);

		//obtainedDataCount  pDlg1->nOldMeter  (double)pDlg1->temp pre
		for (int k = Old_m_index; k <= pDlg1->m_index; k++)
		{
			pDlg->m_HzStamps[k] = (double)pDlg1->nOldMeter + pre * m_count;
			m_count += 1;
		}

		pDlg1->nOldMeter = pDlg1->temp;
		Sleep(10);

		// Shu yerni ko'r
		if (pDlg->m_nCount == 0)
		{
			pDlg1->m_index = 0;
			for (int i = 0; i < 40000; ++i)
			{
				pDlg->m_pKeyence->m_data[i] = Chart::NoValue;

			}
			break;

		}

		nCount1++;

	}

	return 0;
}


void CKeyenceSenser::ContinuouslyExecuteTrendProcessBefore()
{

}

void CKeyenceSenser::StopTrendProcessAfter()
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)m_pParent;

	m_isTrending = false;
	m_threadTrend = INVALID_HANDLE_VALUE;

}

void CKeyenceSenser::StopTrendProcessBefore()
{
	m_isTrendingBefore = false;
	m_threadTrendBefore = INVALID_HANDLE_VALUE;
}

void CKeyenceSenser::resetStartStorage() 
{
	CL3IF_StopStorage(0);
	CL3IF_ClearStorageData(0);
	CL3IF_StartStorage(0);
	m_storageData.clear();
}

void CKeyenceSenser::StopStorage()
{
	CL3IF_StopStorage(0);
	CL3IF_ClearStorageData(0);
	m_storageData.clear();
}

void CKeyenceSenser::StorageDataThread() {  // ������ ���丮�� ���

 	m_KyenceThread1 = AfxBeginThread(StorageReadData, this);

}

UINT CKeyenceSenser::StorageReadData(LPVOID param)
{
	CKeyenceSenser* pKyence = (CKeyenceSenser*)param;
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)pKyence->m_pParent;
	
	
	CString str1;
	CL3IF_ClearStorageData(pKyence->m_nDeviceID);
	
	int nSttTriggerCount = 0, nEndTriggerCount;

	DWORD nCount = 0;
	DWORD nIndexStt = 0;
	const int MaxRequestDataLength = 512000;
	DWORD MaxMeasurementDataCountPerTime = 10000;
	DWORD nextIndex = 1;
	DWORD obtainedDataCount = 0;
	CL3IF_OUTNO outTarget = {};
	BYTE buffer[MaxRequestDataLength] = {};

	const LONG returnCodeStorageData = CL3IF_GetStorageData(pKyence->m_nDeviceID, nIndexStt, MaxRequestDataLength,
		&nextIndex, &obtainedDataCount, &outTarget, reinterpret_cast<DWORD*>(buffer));
	
	if (obtainedDataCount != 0){
		std::vector<int> outTargetList = OutNoConverter::ConvertOutTargetList(outTarget);
		pKyence->m_storageData.resize(1);
		const BYTE* pReadPosition = buffer;

		pKyence->measurementData = {};
		pKyence->measurementData.outMeasurementData.resize(outTargetList.size());
		pKyence->measurementData.addInfo = *reinterpret_cast<const CL3IF_ADD_INFO*>(pReadPosition);
		pReadPosition += sizeof(CL3IF_ADD_INFO);

		for (unsigned int j = 0; j < outTargetList.size(); j++)
		{
			pKyence->measurementData.outMeasurementData[j] = *reinterpret_cast<const CL3IF_OUTMEASUREMENT_DATA*>(pReadPosition);
			pReadPosition += sizeof(CL3IF_OUTMEASUREMENT_DATA);
		}


		nEndTriggerCount = pKyence->measurementData.addInfo.triggerCount;
		if (nSttTriggerCount == 0)	nSttTriggerCount = nEndTriggerCount;

		//���� ������ ġȯ
		/*if (pKyence->measurementData.outMeasurementData[0].measurementValue <= 0) {
			str1.Format("������ ���� (������ : %ld)", pKyence->measurementData.outMeasurementData[0].measurementValue/10);
			pDlg->AddtoList(str1);*/
			//pKyence->KyenceSensorValue.ths = pKyence->OldKyenceSensorValue.ths * 10;

		//}
		//else {
			pKyence->KyenceSensorValue.ths = pKyence->measurementData.outMeasurementData[0].measurementValue;// + pKyence->measurementData.outMeasurementData[2].measurementValue;
			//�β��� ��� ������ OffSet
			//pKyence->KyenceSensorValue.ths = pKyence->KyenceSensorValue.ths/* + pDlg->m_pDlgpKeyence->m_dDataOffSet[0]*10*/;
		//}

		pKyence->KyenceSensorValue.top = pKyence->measurementData.outMeasurementData[1].measurementValue;
		pKyence->KyenceSensorValue.btm = pKyence->measurementData.outMeasurementData[2].measurementValue;

		nCount += obtainedDataCount;
		pKyence->KyenceSensorValue.top /= 10;
		pKyence->KyenceSensorValue.btm /= 10;
		pKyence->KyenceSensorValue.ths /= 10;

		//���� ������
		pKyence->OldKyenceSensorValue.top = pKyence->KyenceSensorValue.top;
		pKyence->OldKyenceSensorValue.btm = pKyence->KyenceSensorValue.btm;
		pKyence->OldKyenceSensorValue.ths = pKyence->KyenceSensorValue.ths;
		
	}
	/*else {
		pKyence->KyenceSensorValue.top = 10;
		pKyence->KyenceSensorValue.btm = 30;
		pKyence->KyenceSensorValue.ths = 24;
	}*/

	pDlg->OnFullDataRateTimer();

	return 0;
}


