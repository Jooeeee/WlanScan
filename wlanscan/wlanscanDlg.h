// wlanscanDlg.h : Headerdatei
//

#pragma once
#include "afxcmn.h"
#include "airctl.h"
#include "afxwin.h"

// CwlanscanDlg-Dialogfeld
class CwlanscanDlg : public CDialog
{
// Konstruktion
public:
	CwlanscanDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_WLANSCAN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	airctl m_airctl;
	CListCtrl m_ctlList;
public:
	CStatic LabelInfo;
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
