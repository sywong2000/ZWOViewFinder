
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
	char* cam_name = nullptr;
	int binning = 1;
	DECLARE_MESSAGE_MAP()
	
public:

	enum CamStatus {
		closed = 0,
		opened,
		capturing,
		snaping
	};

	struct ConnectedCam 
	{
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
		IplImage* pROIImg;
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
	double fFullDisplayScale = 1.0;
	double fROIZoomRatio = 50.0;
	bool bDebayer = true;
	CString m_static_msg;
	int m_CamMode = 1;
	bool bLockROIPosition = false;
	bool bZoomMoving = false;


	int icvFullImageWidth;
	int icvFullImageHeight;
	int icvROIImageWidth;
	int icvROIImageHeight;

	CRect ROIRect;
	int ROIWidth = 64;
	int ROIHeight = 40;
	int ROIPosXMax= 0;
	int ROIPosYMax = 0;


	bool bTransparentDlg = false;
	bool bLBDown;
	CPoint pt0;
	CPoint pt1;
	CPoint roi_start_moving_pt;
	bool bPausedVideo = false;

	bool bCameraConnected = false;

	HWND hWnd_roi;


	afx_msg void OnBnClickedButtonStart();
	void MallocImg(int iID);
	void ReleaseImg(int iID);
	void ReleaseMem(int iID);
	void MallocMem(int iID);
	void CloseCam(int iID);
	void StopCam(int iID);
	afx_msg void OnBnClickedButtonStop();
//	afx_msg void OnTRBNThumbPosChangingSliderExposure(NMHDR* pNMHDR, LRESULT* pResult);
	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	CStatic StaticDraw;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonScan();
	afx_msg bool ConnectSelectedCamera(int iSelectedID);
	afx_msg void OnCbnSelchangeComboCameras();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeComboZoomLevel();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedCheckLockpos1();
	afx_msg void OnBnClickedCheckLockpos2();
};

unsigned __stdcall Display(void* pArguments);
unsigned __stdcall CaptureVideo(void* pArguments);
void onMouseFullDisplay(int Event, int x, int y, int flags, void* param);
void onMouseROIDisplay(int Event, int x, int y, int flags, void* param);
