#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "ObjectFrame.h"
#include "ObjectDialogue.h"
#include "SelectDialogue.h"

class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();

private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	ObjectFrame* m_objectFrame; // testing new preview window to render and handle objects 
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here
	HWND m_objectHandle; // Handle for object window
	ObjectDialogue m_objectDialogue;

	int m_width;		
	int m_height;

	CRect dialogueRect; 
	int dialogue_width; 
	int dialogue_height; 
	
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuEditSelect();
	afx_msg	void ToolBarButton1();

	afx_msg void MenuObjectPreview(); 


	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
};
