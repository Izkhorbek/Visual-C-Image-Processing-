// MotionPAS.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "pch.h"
#include "OffLineThicknessDlg.h"
#include "MotionPAS.h"


using namespace std;
using namespace chrono;

// CMotionPAS

IMPLEMENT_DYNAMIC(CMotionPAS, CWnd)

CMotionPAS::CMotionPAS(CWnd* pParent)
{
	m_pParent = pParent;

	for (int i = 0; i < MAX_AXIS; i++) {
		m_lScale[i] = 1000;
		m_nVelocity[i] = 30000;
	}
	m_bMotion = FALSE;
	m_bMotion_Signal = FALSE;
} 

CMotionPAS::~CMotionPAS()
{
}


BEGIN_MESSAGE_MAP(CMotionPAS, CWnd)
END_MESSAGE_MAP()



// CMotionPAS 메시지 처리기입니다.

int CMotionPAS::InitMotion(BYTE ip1, BYTE ip2, BYTE ip3, BYTE ip4, int m_iBdID)
{
	COffLineThicknessDlg *pDlg = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;
	FAS_EnableLog(true);
	FAS_SetLogLevel(LOG_LEVEL_MOTION);

	
	if (!FAS_IsSlaveExist(m_iBdID))
	{
		if (!FAS_Connect(192, 168, 0, 2, 0)){
			pDlg->AddtoList("Motor is failed to connect");
			m_bMotion_Signal = FALSE;
			AfxMessageBox("모터 연결 안됨");
		}
		else
		{
			pDlg->AddtoList("Motor is successed to connect");
			m_bMotion_Signal = TRUE;
		}
	}
	else
	{
		FAS_Close(m_iBdID);
	}

	m_bMotion = FAS_IsSlaveExist(m_iBdID);

	if (m_bMotion)
	{
 		//FAS_PrintCustomLog(0, LOG_LEVEL_ALL, _T("[ALL Log]    It's Connected.\n"));	// will not print.
 		//FAS_PrintCustomLog(0, LOG_LEVEL_MOTION, _T("[Motion Log] It's Connected.\n"));	// will print.
	}
	int nRet = FAS_ServoEnable(m_iBdID, true);

	return m_bMotion;
}



int CMotionPAS::moveS(int nAxis, int nPos, int vel)
{
	if (!isMotion())	return 0;
	long lPos = nPos;
	if (vel == 0)	vel = m_nVelocity[nAxis];


	return FAS_MoveSingleAxisAbsPos(nAxis, lPos, vel);

}

int CMotionPAS::moveR(int nAxis, int nPos , long lSpeed)
{
	if (!isMotion())	return 0;
	long lPos = nPos * m_lScale[nAxis];
	int nRtn;

	nRtn = FAS_MoveSingleAxisIncPos(nAxis, lPos, lSpeed);
	
	return nRtn;

}

int CMotionPAS::moveSxy(int mode, int pos1, int pos2)  //2개 모션 제어
{
//	if (!isMotion())	return 0;
// 	int		axis[] = { 0, 1 };
// 	long	posX = nX * m_lScale[0];
// 	long	posY = nY * m_lScale[1];
// 	long	lPos[] = { posX, posY };
// 	return FAS_MoveLinearAbsPos(2, axis, lPos, m_nVelocity[0], 100);

	DWORD dwAxisStatus, dwInput;
	EZISERVO_AXISSTATUS stAxisStatus;
	long lAbsPos, lIncPos, lVelocity;
	int nRtn;
	long lValue;	
	int	axis[2];

	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;
	CString strTemp;

	if (mode == 0) { // 오븐 전 
		axis[0] = 0;
		axis[1] = 1;
	}
	else { // 오븐 후 
		axis[0] = 2;
		axis[1] = 3;
	}
	
	long	lPos[] = { pos1, pos2 };
	nRtn = FAS_MoveLinearAbsPos(2, axis, lPos, m_nVelocity[0], 100);

	 
	if (nRtn != FMM_OK) return FALSE;
	// Motion 명령이 완전히 끝날 때 까지 대기.
	do
	{
		Sleep(1);
		if (mode == 0) {
			nRtn = FAS_GetAxisStatus(0, &dwAxisStatus);  // 오븐 전 상부만 확인
		}
		else {
			nRtn = FAS_GetAxisStatus(2, &dwAxisStatus);	 // 오븐 후 상부만 확인
		}
		
		if (nRtn != FMM_OK) return FALSE;
		stAxisStatus.dwValue = dwAxisStatus;
	} while (stAxisStatus.FFLAG_MOTIONING);

	Sleep(100);

	// 이동 위치 재 확인  
	int nCValue = 5; // 0.005 mm 이내 

	if (mode == 0) { // 오븐 전 
		if (FAS_GetActualPos(0, &lValue) == FMM_OK)	// ACT POS
		{
			if (lValue > pos1 + nCValue || lValue < pos1 - nCValue) {
				strTemp.Format("오븐 전 상부 위치가 설정 위치와 다릅니다.");
				pDlg->AddtoList(strTemp);
				return false;
			}
		}

		if (FAS_GetActualPos(1, &lValue) == FMM_OK)	// ACT POS
		{
			if (lValue > pos2 + nCValue || lValue < pos2 - nCValue) {
				strTemp.Format("오븐 전 하부 위치가 설정 위치와 다릅니다.");
				pDlg->AddtoList(strTemp);
				return false;
			}
		}
		
	}
	else {
		if (FAS_GetActualPos(2, &lValue) == FMM_OK)	// ACT POS
		{
			if (lValue > pos1 + nCValue || lValue < pos1 - nCValue) {
				strTemp.Format("오븐 후 상부 위치가 설정 위치와 다릅니다.");
				pDlg->AddtoList(strTemp);
				return false;
			}
		}

		if (FAS_GetActualPos(3, &lValue) == FMM_OK)	// ACT POS
		{
			if (lValue > pos2 + nCValue || lValue < pos2 - nCValue) {
				strTemp.Format("오븐 후 하부 위치가 설정 위치와 다릅니다.");
				pDlg->AddtoList(strTemp);
				return false;
			}
		}
		
	}

	return TRUE;
}

int CMotionPAS::moveSxyOneMotion(int mode, int pos1)  //1개 모션 제어
{
	COffLineThicknessDlg* pDlg = (COffLineThicknessDlg*)AfxGetApp()->m_pMainWnd;

	DWORD dwAxisStatus, dwInput;
	EZISERVO_AXISSTATUS stAxisStatus;
	long lAbsPos, lIncPos, lVelocity;
	int nRtn;
	long lValue;
	int axis = 100;
	CString strTemp;

	if (mode == 0) {  //오븐 전
		axis = 0;
	}
	else {           //오븐 후
		axis = 2;
	}

	nRtn = FAS_MoveSingleAxisAbsPos(axis, pos1, pDlg->m_dVelocity);


	if (nRtn != FMM_OK) return FALSE;
	// Motion 명령이 완전히 끝날 때 까지 대기.
	do
	{
		Sleep(1);
		if (mode == 0) {
			nRtn = FAS_GetAxisStatus(0, &dwAxisStatus);  // 오븐 전 상부만 확인
		}
		else {
			nRtn = FAS_GetAxisStatus(2, &dwAxisStatus);	 // 오븐 후 상부만 확인
		}

		if (nRtn != FMM_OK) return FALSE;
		stAxisStatus.dwValue = dwAxisStatus;
	} while (stAxisStatus.FFLAG_MOTIONING);

	Sleep(100);

	// 이동 위치 재 확인  
	int nCValue = 5; // 0.005 mm 이내 

	if (mode == 0) { // 오븐 전 
		if (FAS_GetActualPos(0, &lValue) == FMM_OK)	// ACT POS
		{
			if (lValue > pos1 + nCValue || lValue < pos1 - nCValue) {
				strTemp.Format("오븐 전 상부 위치가 설정 위치와 다릅니다.");
				pDlg->AddtoList(strTemp);
				return false;
			}
		}
	}

	else {
		if (FAS_GetActualPos(2, &lValue) == FMM_OK)	// ACT POS
		{
			if (lValue > pos1 + nCValue || lValue < pos1 - nCValue) {
				strTemp.Format("오븐 후 상부 위치가 설정 위치와 다릅니다.");
				pDlg->AddtoList(strTemp);
				return false;
			}
		}
	}

	return TRUE;
}

int CMotionPAS::moveJog(int nAxis, int nPos, int vel)
{
	if (!isMotion())	return 0;
	long lPos = nPos * m_lScale[nAxis];
	if (vel == 0)	vel = m_nVelocity[nAxis];
	return FAS_MoveVelocityEx(nAxis, vel, 1, 0);
}
// 
// int CMotionPAS::moveSxy(int targetX, int targetY, int nDurTime)
// {
// 	if (!isMotion())	return 0;
// 	long curX, curY, distX, distY, velX, velY;
// 	int timeScale = 1115;
// 	curX = getCPosition(AXIS::x);
// 	curY = getCPosition(AXIS::y);
// 	distX = abs(targetX - curX);
// 	distY = abs(targetY - curY);
// 	if (distX > distY) {
// 		velX = (m_lScale[AXIS::x] * timeScale * distX / nDurTime);
// 		velY = (m_lScale[AXIS::y] * timeScale * distY / nDurTime);
// 	}
// 	else {
// 		velX = (m_lScale[AXIS::x] * timeScale * distX / nDurTime);
// 		velY = (m_lScale[AXIS::y] * timeScale * distY / nDurTime);
// 	}
// 	TRACE(_T("\nvelocity : %d / %d"), velX, velY);
// 
// 	system_clock::time_point StartTime = system_clock::now();
// 	moveS(AXIS::x, targetX, velX);
// 	moveS(AXIS::y, targetY, velY);
// 	waitAll();
// 	system_clock::time_point EndTime = system_clock::now();
// 	milliseconds ms = duration_cast<milliseconds>(EndTime - StartTime);
// 	//	writeLog(("durTime : %dms"), ms.count());
// 
// 	return 1;
// }

int CMotionPAS::moveRxy(int nX, int nY)
{
	if (!isMotion())	return 0;
	int		axis[] = { 0, 1 };
	long	posX = nX * m_lScale[0];
	long	posY = nY * m_lScale[1];
	long	lPos[] = { posX, posY };
	return FAS_MoveLinearIncPos(2, axis, lPos, m_nVelocity[0], 100);
}

int CMotionPAS::moveSDone(int nAxis, int nPos)
{
	moveS(nAxis, nPos);
	return wait(nAxis);
}

int CMotionPAS::moveRDone(int nAxis, int nPos, long ISpeed)
{
	moveR(nAxis, nPos, ISpeed);
	return wait(nAxis);
}

int CMotionPAS::moveSxyDone(int nX, int nY)
{
//	moveSxy(nX, nY);
	wait(0);
	return wait(1);
}

int CMotionPAS::moveRxyDone(int nX, int nY)
{
	moveRxy(nX, nY);
	wait(0);
	return  wait(1);
}

int CMotionPAS::moveV(int nAxis, int nDir, DWORD Velocity)
{
	return FAS_MoveVelocity(nAxis, Velocity, nDir);
}

int CMotionPAS::moveStop(int nAxis)
{
	return FAS_MoveStop(nAxis);
}
int CMotionPAS::moveEmergencyStop(int nAxis)
{
	return FAS_EmergencyStop(nAxis);
}

BOOL CMotionPAS::PeekAndPump()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			return FALSE;
		}
	}
	return TRUE;
}

int CMotionPAS::wait(int nAxis)
{
	if (!isMotion())	return 0;
	int nRet;
	DWORD dwAxisStatus;
	EZISERVO_AXISSTATUS stAxisStatus;
	do
	{
		//Sleep(1);
		nRet = FAS_GetAxisStatus(nAxis, &dwAxisStatus);
		_ASSERT(nRet == FMM_OK);
		stAxisStatus.dwValue = dwAxisStatus;
		if (!PeekAndPump())
			break;
	} while (stAxisStatus.FFLAG_MOTIONING);

	return nRet;
}

int CMotionPAS::waitAll()
{
	if (!isMotion())	return 0;
	wait(AXIS::x);
	return wait(AXIS::y);
}


float CMotionPAS::getCPosition(int nAxis)
{
	if (!isMotion())	return 0;
	float pos1;
	long pos;
	FAS_GetCommandPos(nAxis, &pos);
	pos1 = float(pos);
	float a = float(m_lScale[nAxis]);
	pos1 = pos / a;
	return pos1;
}
float CMotionPAS::getAPosition(int nAxis)
{
	if (!isMotion())	return 0;
	float pos1, PosValue;
	long pos;

	FAS_GetActualPos(nAxis, &pos);
	pos1 = float(pos);
	float a = float(m_lScale[nAxis]);
	pos1 = pos / a;
	return pos1;
}

int CMotionPAS::moveHome(int nAxis)
{
	FAS_MoveOriginSingleAxis(nAxis);

	int nRet;
	DWORD dwAxisStatus;
	EZISERVO_AXISSTATUS stAxisStatus;
	do
	{

		nRet = FAS_GetAxisStatus(nAxis, &dwAxisStatus);
		_ASSERT(nRet == FMM_OK);
		stAxisStatus.dwValue = dwAxisStatus;
		if (!PeekAndPump())
			break;
		TRACE(_T("%08X"), stAxisStatus.dwValue);
	} while (!stAxisStatus.FFLAG_ORIGINRETOK);
	return nRet;
}

BOOL CMotionPAS::isMotion()
{
	// 모터이상 확인 함수 ?

	return m_bMotion;
}
///*


template<typename ... Args>
// void MotionPAS::writeLog(const string& format, Args ... args)
// {
// 	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
// 	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
// 	unique_ptr<char[]> buf(new char[size]);
// 	snprintf(buf.get(), size, format.c_str(), args ...);
// 	string str = string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
// 	writeLog(str);
// }
int CMotionPAS::zeroPosition(int nAxis)
{
	FAS_ClearPosition(nAxis);
	return 0;

}
//*/


