
// addition3Dlg.h : header file
//

#pragma once


// Caddition3Dlg dialog
class Caddition3Dlg : public CDialogEx
{
// Construction
public:
	Caddition3Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ADDITION3_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedSumStatic();
	double m_editSummand;
	double m_editAddend;
	double m_editSum;
//	afx_msg void OnBnClickedAddButton1();
	afx_msg void OnBnClickedAddButton1();
};
