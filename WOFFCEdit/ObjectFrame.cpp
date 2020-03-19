#include "ObjectFrame.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(ObjectFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

ObjectFrame::ObjectFrame()
{
}

int ObjectFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1; 

	// Create a view to occupy the client area of the frame, directX render here
	if (!m_DirXView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n"); 
		return -1; // fail to create
	}

	m_menu.LoadMenuW(IDR_MENU2); 
	SetMenu(&m_menu); 

	return 0; 
}

