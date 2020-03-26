#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "MFCRenderFrame.h"
#include "SceneObject.h"
#include "ModelInfo.h"
#include "ToolMain.h"
#include <vector>

class ObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDialogue);

public:
	ObjectDialogue(CWnd* pParent = NULL);
	virtual ~ObjectDialogue(); 
	void Init(std::vector<ModelInfo>* models_, int* objectIndex, ToolMain* toolSystem);

	CChildRender m_DirXView; 

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif // AFX_DESIGN_TIME

protected: 
	int* currentObject_index;
	std::vector<ModelInfo>* currentModelInfo;
	ToolMain* tool_system;

	SceneObject* new_object; 

	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void End(); 
	afx_msg void Select(); 
	DECLARE_MESSAGE_MAP()

private: 
	CRect dirXViewRect; 
	std::wstring ConvertString(std::string s); 

public: 
	CListBox m_listBox; 
	virtual BOOL OnInitDialog() override; 
	virtual void PostNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk(); 
	afx_msg void OnBnClickUpdate();
};

INT_PTR CALLBACK SelectProc(HWND   hwndDlg, UINT   uMsg, WPARAM wParam, LPARAM lParam);