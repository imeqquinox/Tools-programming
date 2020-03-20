#pragma once
#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "MFCRenderFrame.h"
#include "SceneObject.h"
#include <vector>

class ObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDialogue);

public:
	ObjectDialogue(CWnd* pParent = NULL);
	virtual ~ObjectDialogue(); 

	CChildRender m_DirXView; 

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif // AFX_DESIGN_TIME

protected: 
	virtual void DoDataExchange(CDataExchange* pDX); 
	afx_msg void End(); 
	DECLARE_MESSAGE_MAP()

private: 
	CRect dirXViewRect; 

public: 
	virtual BOOL OnInitDialog() override; 
	virtual void PostNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk(); 
};
