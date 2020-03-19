#pragma once
#include <afxwin.h>
#include <afxext.h>
#include "MFCRenderFrame.h"

class ObjectFrame : public CFrameWnd
{
public:
	CMenu m_menu; 
	CStatusBar m_wndStatusBar; 
	CChildRender m_DirXView;

	ObjectFrame();

private: 
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP() // required macro for message map functionality One per class
};

