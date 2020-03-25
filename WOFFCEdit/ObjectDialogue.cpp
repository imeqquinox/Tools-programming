#include "stdafx.h"
#include "ObjectDialogue.h"

IMPLEMENT_DYNAMIC(ObjectDialogue, CDialogEx) 

BEGIN_MESSAGE_MAP(ObjectDialogue, CDialogEx)
	ON_WM_CREATE()
	ON_COMMAND(IDOK, &ObjectDialogue::End)
	ON_BN_CLICKED(IDOK, &ObjectDialogue::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST2, &ObjectDialogue::Select)
END_MESSAGE_MAP()

ObjectDialogue::ObjectDialogue(CWnd* pParent)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	dirXViewRect = CRect(250, 25, 675, 450);
}

ObjectDialogue::~ObjectDialogue()
{
}

void ObjectDialogue::ModelList(std::vector<ModelInfo>* models, int * objectIndex)
{
	currentObject_index = objectIndex;

	for (int i = 0; i < models->size(); i++)
	{
		std::wstring listBoxEntry = ConvertString(models->at(i).name);
		m_listBox.AddString(listBoxEntry.c_str());
	}
}

std::wstring ObjectDialogue::ConvertString(std::string s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void ObjectDialogue::Select()
{
	int index = m_listBox.GetCurSel();
	CString currentSelection; 

	m_listBox.GetText(index, currentSelection);

	*currentObject_index = index; 
}

void ObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX); 
	DDX_Control(pDX, IDC_LIST2, m_listBox);
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