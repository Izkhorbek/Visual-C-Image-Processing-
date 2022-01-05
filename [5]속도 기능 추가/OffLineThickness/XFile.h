// XFile.h: interface for the CXFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XFILE_H__D434BB99_F902_4034_80B3_7A4CF067FC29__INCLUDED_)
#define AFX_XFILE_H__D434BB99_F902_4034_80B3_7A4CF067FC29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXFile  
{
public:
	BOOL XCreateDirectory(CString strPath);		//파일 전체경로를 지정하면 하위경로까지 모두 생성한다.
	CXFile();
	virtual ~CXFile();

};

#endif // !defined(AFX_XFILE_H__D434BB99_F902_4034_80B3_7A4CF067FC29__INCLUDED_)
