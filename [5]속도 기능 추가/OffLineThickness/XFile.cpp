// XFile.cpp: implementation of the CXFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "controlserver.h"
#include "XFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXFile::CXFile()
{

}

CXFile::~CXFile()
{

}

BOOL CXFile::XCreateDirectory(CString strPath)
{
	CString strTmp;
	BOOL bRet = TRUE;
	int nStt = 0,nlen = strPath.GetLength();

	if(strPath.Find("\\",0)<0)
		bRet = FALSE;
	while(nlen>=nStt && bRet)
	{
		nStt = strPath.Find("\\",nStt);
		if(nStt<0)
			break;
		nStt++;
		strTmp = strPath.Left(nStt);
		CreateDirectory(strTmp,NULL);
		if(::GetFileAttributes(strTmp)==-1) //API 함수를 사용하고 리턴값이 -1 (INVALID_FILE_ATTRIBUTES)이면 파일 또는 디렉토리가 존재하지 않는 경우 입니다.
			bRet = FALSE;
	}
	return bRet;
}
