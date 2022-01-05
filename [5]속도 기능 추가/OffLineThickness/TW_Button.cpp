// TW_Button.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "OffLineThickness.h"
#include "TW_Button.h"


// TW_Button

IMPLEMENT_DYNAMIC(TW_Button, CButton)

TW_Button::TW_Button()
{

}

TW_Button::~TW_Button()
{
}


BEGIN_MESSAGE_MAP(TW_Button, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// TW_Button �޽��� ó�����Դϴ�.




void TW_Button::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICK_DOWN), (LPARAM)m_hWnd);
	CButton::OnLButtonDown(nFlags, point);
}


void TW_Button::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICK_UP), (LPARAM)m_hWnd);

	CButton::OnLButtonUp(nFlags, point);
}
