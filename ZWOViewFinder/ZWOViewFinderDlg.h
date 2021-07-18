
// ZWOViewFinderDlg.h : header file
//

#pragma once


// CZWOViewFinderDlg dialog
class CZWOViewFinderDlg : public CDialogEx
{
// Construction
public:
	CZWOViewFinderDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZWOVIEWFINDER_DIALOG };
#endif

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
	afx_msg void StartPreview();
	char* cam_name = nullptr;
	int binning = 1;
	DECLARE_MESSAGE_MAP()
};
