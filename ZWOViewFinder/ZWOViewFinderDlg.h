
// ZWOViewFinderDlg.h : header file
//

#pragma once
#include <string>
#include "ASICamera2.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"


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
	afx_msg void ScanConnectedCameras();
	char* cam_name = nullptr;
	int binning = 1;
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedButtonRescan();
	afx_msg void OnBnClickedButton1();

	enum CamStatus {
		closed = 0,
		opened,
		capturing,
		snaping
	};

	struct ConnectedCam {
		HANDLE Thr_Display, Thr_Snap, Thr_CapVideo;
		CamStatus Status;
		int iCtrlNum;
		//		bool bAvail;
		ASI_CONTROL_CAPS* pControlCaps;
		ASI_CAMERA_INFO* pASICameraInfo;
		ASI_SUPPORTED_MODE* pASISupportedMode;
		bool* pRefreshedIndex;
		IplImage* pRgb;
		IplImage* pTempImg;
		IplImage* pTempImgScaled;
		int iSnapTime;
		ASI_IMG_TYPE ImageType;
		int width, height;
		bool bSnap;
		float fOldScale;
		bool bNewImg;
		bool bSnapContinuous;
		LONG lTrigOutputADelay;
		LONG lTrigOutputADuration;
		bool bHighLevelValidA;
		LONG lTrigOutputBDelay;
		LONG lTrigOutputBDuration;
		bool bHighLevelValidB;
	};

	int iSelectedID;
	int iCamNum;
	ConnectedCam ConnectCamera[ASICAMERA_ID_MAX];
	float fScale = 1;
	CString m_static_msg;
	int m_CamMode;



	afx_msg void OnBnClickedButtonStart();
	void MallocImg(int iID);
	void ReleaseImg(int iID);
	void ReleaseMem(int iID);
	void MallocMem(int iID);
	void OnCbnSelchangeComboCameras();
	void CloseCam(int iID);
	void StopCam(int iID);
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnTRBNThumbPosChangingSliderExposure(NMHDR* pNMHDR, LRESULT* pResult);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
};

void Display(LPVOID params);
void CaptureVideo(LPVOID params);
void onMouse(int Event, int x, int y, int flags, void* param);