
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
	afx_msg void OnBnClickedButtonRescan();

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
	float fScale = 1;
	bool bDebayer = true;
	CString m_static_msg;
	int m_CamMode;

	int iFullImageWidth;
	int iFullImageHeight;

	CRect ROIRect;

	int iROIImageWidth;
	int iROIImageHeight;

	int ROIWidth = 100;
	int ROIHeight = 100;


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
	afx_msg void OnClose();
	CStatic StaticDraw;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

void Display(LPVOID params);
void CaptureVideo(LPVOID params);
void onMouse(int Event, int x, int y, int flags, void* param);