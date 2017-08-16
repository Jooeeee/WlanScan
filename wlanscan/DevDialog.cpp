// DevDialog.cpp : implementation file
//

#include "stdafx.h"
#include "wlanscan.h"
#include "DevDialog.h"
#include "airctl.h"
#include "wlanscanDlg.h"
// DevDialog dialog

IMPLEMENT_DYNAMIC(DevDialog, CDialog)

DevDialog::DevDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DevDialog::IDD, pParent)
{

}

DevDialog::~DevDialog()
{
}

void DevDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, List);
}


BEGIN_MESSAGE_MAP(DevDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO2, &DevDialog::OnCbnSelchangeCombo2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &DevDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// DevDialog message handlers

void DevDialog::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
}


BOOL DevDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CwlanscanDlg *win = (CwlanscanDlg *)this->GetParent();
	deviceInfo* dv = win->m_airctl.getDevList();

	if (dv == NULL)
	{
		List.AddString("No available interface found !");
	}else{
		while (dv != NULL)
		{
			List.AddString(dv->description);
			dv = dv->next;
		}
	}
	List.SetCurSel(0);
	return TRUE;

}
void DevDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	this->LastSel = List.GetCurSel();
	CDialog::OnClose();
}

void DevDialog::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	this->LastSel = List.GetCurSel();
	this->OnOK();

}
