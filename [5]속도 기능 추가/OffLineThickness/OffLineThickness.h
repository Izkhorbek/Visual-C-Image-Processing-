
// OffLineThickness.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// COffLineThicknessApp:
// �� Ŭ������ ������ ���ؼ��� OffLineThickness.cpp�� �����Ͻʽÿ�.
//

class COffLineThicknessApp : public CWinApp
{
public:
	COffLineThicknessApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern COffLineThicknessApp theApp;