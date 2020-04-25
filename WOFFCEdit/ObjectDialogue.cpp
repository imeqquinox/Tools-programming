#include "stdafx.h"
#include "ObjectDialogue.h"

IMPLEMENT_DYNAMIC(ObjectDialogue, CDialogEx) 

BEGIN_MESSAGE_MAP(ObjectDialogue, CDialogEx)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK2, &ObjectDialogue::OnBnClickedOk)
	ON_COMMAND(IDOK2, &ObjectDialogue::End)
	ON_BN_CLICKED(IDC_BUTTON1, &ObjectDialogue::OnBnClickUpdate)
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

void ObjectDialogue::Init(std::vector<ModelInfo>* models, int * objectIndex, ToolMain* toolSystem)
{
	currentObject_index = &toolSystem->m_currentObject;
	currentModelInfo = models;
	tool_system = toolSystem;

	new_object = &toolSystem->m_sceneObject;

	for (int i = 0; i < currentModelInfo->size(); i++)
	{
		std::wstring listBoxEntry = ConvertString(currentModelInfo->at(i).GetName());
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
	tool_system->onActionAddSceneObject(); 

	CDialogEx::OnOK(); 
}

void ObjectDialogue::OnBnClickUpdate()
{
	new_object->model_path = currentModelInfo->at(*currentObject_index).GetModelPath();
	new_object->tex_diffuse_path = currentModelInfo->at(*currentObject_index).GetTexturePath();
	new_object->render = 0;
	new_object->light_type = 1; 
	new_object->light_diffuse_r = 2; 
	new_object->light_diffuse_g = 3; 
	new_object->light_diffuse_b = 4; 
	new_object->light_specular_r = 5; 
	new_object->light_specular_g = 6; 
	new_object->light_diffuse_b = 7; 
	new_object->light_spot_cutoff = 8; 
	new_object->light_constant = 9; 
	new_object->light_linear = 0; 
	new_object->light_quadratic = 1; 

	CString tempNum;
	GetDlgItemText(IDC_EDIT1, tempNum);
	int x_pos = _ttoi(tempNum); 
	new_object->posX = _ttoi(tempNum); 
	GetDlgItemText(IDC_EDIT2, tempNum); 
	int y_pos = _ttoi(tempNum); 
	new_object->posY = _ttoi(tempNum); 
	GetDlgItemText(IDC_EDIT3, tempNum); 
	int z_pos = _ttoi(tempNum); 
	new_object->posZ = _ttoi(tempNum); 
	currentModelInfo->at(*currentObject_index).SetPosition(x_pos, y_pos, z_pos);

	GetDlgItemText(IDC_EDIT4, tempNum);
	int x_rot = _ttoi(tempNum);
	new_object->rotX = _ttoi(tempNum); 
	GetDlgItemText(IDC_EDIT5, tempNum);
	int y_rot = _ttoi(tempNum);
	new_object->rotY = _ttoi(tempNum); 
	GetDlgItemText(IDC_EDIT6, tempNum);
	int z_rot = _ttoi(tempNum);
	new_object->rotZ = _ttoi(tempNum); 
	currentModelInfo->at(*currentObject_index).SetOrientation(x_rot, y_rot, z_rot);

	GetDlgItemText(IDC_EDIT7, tempNum);
	int x_scale = _ttoi(tempNum);
	new_object->scaX = _ttoi(tempNum); 
	GetDlgItemText(IDC_EDIT8, tempNum);
	int y_scale = _ttoi(tempNum);
	new_object->scaY = _ttoi(tempNum); 
	GetDlgItemText(IDC_EDIT9, tempNum);
	int z_scale = _ttoi(tempNum);
	new_object->scaZ = _ttoi(tempNum); 
	currentModelInfo->at(*currentObject_index).SetScale(x_scale, y_scale, z_scale);

	tool_system->onActionUpdateModels(); 
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

