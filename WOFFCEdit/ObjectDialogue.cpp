#include "stdafx.h"
#include "ObjectDialogue.h"

IMPLEMENT_DYNAMIC(ObjectDialogue, CDialogEx) 

BEGIN_MESSAGE_MAP(ObjectDialogue, CDialogEx)
	ON_WM_CREATE()
	ON_COMMAND(IDOK, &ObjectDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectDialogue::OnBnClickedOk)
END_MESSAGE_MAP()

ObjectDialogue::ObjectDialogue(CWnd* pParent)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	dirXViewRect = CRect(250, 25, 675, 450);
}

ObjectDialogue::~ObjectDialogue()
{
}

void ObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX); 
}

int ObjectDialogue::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Create a view to occupy the client area of the frame, directX render here
	if (!m_DirXView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, dirXViewRect, this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1; // fail to create
	}

	return 0;
}

void ObjectDialogue::End()
{
	DestroyWindow(); 
}

void ObjectDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK(); 
}

BOOL ObjectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void ObjectDialogue::PostNcDestroy()
{
}