#pragma once


// CTipDlg dialog

class CTipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTipDlg)

public:
	CTipDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTipDlg();

// Dialog Data
	enum { IDD = IDD_DIP_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
