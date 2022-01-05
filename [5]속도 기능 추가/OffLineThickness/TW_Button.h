#pragma once

#define BN_CLICK_DOWN 30
#define BN_CLICK_UP 31

// TW_Button

class TW_Button : public CButton
{
	DECLARE_DYNAMIC(TW_Button)

public:
	TW_Button();
	virtual ~TW_Button();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


