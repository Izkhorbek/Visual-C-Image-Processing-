#pragma once

#include "FAS_EziMOTIONPlusE.h"

#define MAX_AXIS	4

using namespace PE;
using namespace std;
// CMotionPAS

class CMotionPAS : public CWnd
{
	DECLARE_DYNAMIC(CMotionPAS)

public:
	CMotionPAS(CWnd* pParent = NULL);
	virtual ~CMotionPAS();

protected:
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bMotion;
	CWnd* m_pParent;

public:

	DWORD	m_nVelocity[MAX_AXIS];
	long	m_lScale[MAX_AXIS];
	float getAPosition(int nAxis);
	float getCPosition(int nAxis);
	int InitMotion(BYTE ip1, BYTE ip2, BYTE ip3, BYTE ip4, int m_iBdID);	//n : axis count
	BOOL isMotion();

	int moveHome(int nAxis);
	int moveJog(int nAxis, int nPos, int vel);
	int moveS(int nAxis, int nPos, int vel = 0);		//pos um
	int moveR(int nAxis, int nPos, long lSpeed);
	int moveSxy(int mode, int pos1, int pos2);
	int moveRxy(int nX, int nY);
//	int moveSxy(int nX, int nY, int nDurTime);
	int moveV(int nAxis, int nDir, DWORD Velocity);
	int moveStop(int nAxis);
	int moveEmergencyStop(int nAxis);
	int moveSxyOneMotion(int mode, int pos1);

	int moveSDone(int nAxis, int nPos);		//pos um
	int moveRDone(int nAxis, int nPos, long ISpeed);
	int moveSxyDone(int nX, int nY);
	int moveRxyDone(int nX, int nY);

	int wait(int nAxis);
	int waitAll();


	BOOL PeekAndPump();
	template<typename ... Args>
	//void writeLog(const string& format, Args ... args);
	int zeroPosition(int nAxis);
	BOOL m_bMotion_Signal;
};


