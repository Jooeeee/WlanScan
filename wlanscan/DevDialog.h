#pragma once
#include "afxwin.h"


// DevDialog dialog

class DevDialog : public CDialog
{
	DECLARE_DYNAMIC(DevDialog)

public:
	DevDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~DevDialog();
	
	int LastSel;
	// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo2();
public:
	CComboBox List;
public:
	afx_msg void OnClose();
public:
	afx_msg void OnBnClickedButton1();
};
