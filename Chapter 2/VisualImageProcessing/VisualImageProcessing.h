
// VisualImageProcessing.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CVisualImageProcessingApp:
// �� Ŭ������ ������ ���ؼ��� VisualImageProcessing.cpp�� �����Ͻʽÿ�.
//

class CVisualImageProcessingApp : public CWinApp
{
public:
	CVisualImageProcessingApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CVisualImageProcessingApp theApp;