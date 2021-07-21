
// ZWOViewFinderDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "ZWOViewFinder.h"
#include "ZWOViewFinderDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


#ifdef _DEBUG
#define new DEBUG_NEW
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <crtdbg.h>
#include <assert.h>
#endif
#include "include/ASICamera2.h"


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CZWOViewFinderDlg dialog

struct Dlg_Thread
{
	CZWOViewFinderDlg* pDlg;
	int ID;
	HWND cvHandle;

};


CZWOViewFinderDlg::CZWOViewFinderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZWOVIEWFINDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_crtBreakAlloc = 161;
	_CrtDumpMemoryLeaks();
}

void CZWOViewFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZWOViewFinderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RESCAN, &CZWOViewFinderDlg::OnBnClickedButtonRescan)
	ON_BN_CLICKED(IDC_BUTTON_START, &CZWOViewFinderDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CZWOViewFinderDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CZWOViewFinderDlg::OnBnClickedButtonStop)
	ON_MESSAGE(WM_MY_MSG, OnUpdateData)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CZWOViewFinderDlg message handlers


BOOL CZWOViewFinderDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CZWOViewFinderDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	CMenu* pPopMenu;
	CMenu menu;
	POINT pt;
	CRect rc;

	switch (wParam)
	{
	case WM_RBUTTONDOWN:

		//menu.LoadMenu(IDR_MENU_RBDOWN);
		//pPopMenu = menu.GetSubMenu(0);
		//GetCursorPos(&pt);
		//pPopMenu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);

		break;
	case WM_UPDATEUISTATE:
		if (lParam)
		{
			GetDlgItem(lParam)->GetWindowRect(&rc);
			ScreenToClient(&rc);
			InvalidateRect(rc);
		}
		else
			UpdateData(false);
		break;
	case 3210:
		//DrawControl();
		break;
	}

	return 0;
}


void CZWOViewFinderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CZWOViewFinderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CZWOViewFinderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//void CZWOViewFinderDlg::Preview(bool color)
//{
//
//    cv::namedWindow(PREVIEW_WINDOW_NAME, cv::WINDOW_NORMAL);
//    cv::resizeWindow(PREVIEW_WINDOW_NAME, 640, 480);
//    //cv::namedWindow(HISTOGRAM_WINDOW_NAME, 1);
//
//    //cv::createTrackbar(
//    //    "agc mode",
//    //    HISTOGRAM_WINDOW_NAME,
//    //    nullptr,
//    //    1,
//    //    agc_mode_trackbar_callback,
//    //    nullptr
//    //);
//
//    gain_trackbar_pos = camera_gain;
//    cv::createTrackbar(
//        "gain",
//        PREVIEW_WINDOW_NAME,
//        &gain_trackbar_pos,
//        camera::GAIN_MAX,
//        gain_trackbar_callback,
//        nullptr
//    );
//
//    exposure_trackbar_pos = camera_exposure_us;
//    cv::createTrackbar(
//        "exposure time [us]",
//        PREVIEW_WINDOW_NAME,
//        &exposure_trackbar_pos,
//        camera::EXPOSURE_MAX_US,
//        exposure_trackbar_callback,
//        nullptr
//    );
//
//    // deque of 10 timestamps for use in calculating frame rate
//    constexpr int NUM_FRAMERATE_FRAMES = 10;
//    std::deque<steady_clock::time_point> timestamps(NUM_FRAMERATE_FRAMES, steady_clock::now());
//
//    auto last_histogram_update = steady_clock::now();
//
//    while (!end_program)
//    {
//        // Get frame from deque
//        std::unique_lock<std::mutex> to_preview_deque_lock(to_preview_deque_mutex);
//        to_preview_deque_cv.wait(
//            to_preview_deque_lock,
//            [&] {return !to_preview_deque.empty() || end_program;}
//        );
//        if (end_program)
//        {
//            break;
//        }
//        while (to_preview_deque.size() > 1)
//        {
//            // Discard all but most recent frame
//            to_preview_deque.back()->decrRefCount();
//            to_preview_deque.pop_back();
//        }
//        Frame* frame = to_preview_deque.back();
//        to_preview_deque.pop_back();
//        to_preview_deque_lock.unlock();
//
//        // Calculate framerate over last NUM_FRAMERATE_FRAMES
//        auto now = steady_clock::now();
//        timestamps.push_front(now);
//        auto then = timestamps.back();
//        timestamps.pop_back();
//        duration<double> elapsed = now - then;
//        double framerate = (double)NUM_FRAMERATE_FRAMES / elapsed.count();
//        char window_title[512];
//        if (disk_file_exists)
//        {
//            sprintf(window_title, "%s %.1f FPS %s", PREVIEW_WINDOW_NAME, framerate, (disk_write_enabled) ? ("writing to disk") : ("disk write paused"));
//        }
//        else
//        {
//            sprintf(window_title, "%s %.1f FPS", PREVIEW_WINDOW_NAME, framerate);
//        }
//        cv::setWindowTitle(PREVIEW_WINDOW_NAME, window_title);
//
//        try
//        {
//            // This throws an exception if the window is closed
//            cv::getWindowProperty(PREVIEW_WINDOW_NAME, 0);
//        }
//        catch (cv::Exception& e)
//        {
//            // window was closed by user
//            frame->decrRefCount();
//            break;
//        }
//
//        cv::Mat img_raw(Frame::HEIGHT, Frame::WIDTH, CV_8UC1, (void*)(frame->frame_buffer_));
//
//        // Debayer if color camera
//        cv::Mat img_preview;
//        
//        if (color)
//        {
//            cv::cvtColor(img_raw, img_preview, cv::COLOR_BayerBG2BGR);
//        }
//        else
//        {
//            // Must make a copy so that crosshairs added later do not modify the original frame.
//            // Modifications to the original frame could end up being written to disk.
//            img_preview = img_raw.clone();
//        }
//
//        // Add grey crosshairs
//        cv::line(
//            img_preview,
//            cv::Point(Frame::WIDTH / 2, 0),
//            cv::Point(Frame::WIDTH / 2, Frame::HEIGHT - 1),
//            cv::Scalar(50, 50, 50),
//            1
//        );
//        cv::line(
//            img_preview,
//            cv::Point(0, Frame::HEIGHT / 2),
//            cv::Point(Frame::WIDTH - 1, Frame::HEIGHT / 2),
//            cv::Scalar(50, 50, 50),
//            1
//        );
//
//        // Show image with crosshairs in a window
//        cv::imshow(PREVIEW_WINDOW_NAME, img_preview);
//
//
//        // Display histogram
//        now = steady_clock::now();
//        //elapsed = now - last_histogram_update;
//        //if (elapsed.count() >= HISTOGRAM_UPDATE_PERIOD_S)
//        //{
//        //    make_histogram(img_raw);
//        //    last_histogram_update = now;
//        //}
//
//        if (agc_enabled)
//        {
//            cv::setTrackbarPos("exposure time", PREVIEW_WINDOW_NAME, camera_exposure_us);
//            cv::setTrackbarPos("gain", PREVIEW_WINDOW_NAME, camera_gain);
//        }
//
//        char key = (char)cv::waitKey(1);
//
//        if (key == 's')
//        {
//            disk_write_enabled = !disk_write_enabled;
//            if (disk_write_enabled)
//            {
//                printf("Writing frames to disk if filename provided (press s to stop)\n");
//            }
//            else
//            {
//                printf("Not writing frames to disk (press s to start)\n");
//            }
//        }
//
//        frame->decrRefCount();
//    }
//
//}

//static void set_thread_priority(pthread_t thread, int policy, int priority)
//{
//	pthread_self();
//
//	sched_param sch_params;
//	sch_params.sched_priority = priority;
//	if ((errno = pthread_setschedparam(thread, policy, &sch_params)))
//	{
//		err(1, "Failed to set thread priority (%d, %d)", policy, priority);
//	}
//}
//
//static void set_thread_name(pthread_t thread, const char* name)
//{
//	if ((errno = pthread_setname_np(thread, name)))
//	{
//		warn("Failed to set thread name (\"%s\")", name);
//	}
//}


void CZWOViewFinderDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* ACSliderCtrl = (CSliderCtrl*)pScrollBar;
	int nID = ACSliderCtrl->GetDlgCtrlID();
	int NewPos = ((CSliderCtrl*)pScrollBar)->GetPos();
	CWnd* ACWnd = GetDlgItem(nID);

	CString str;
	int val =0;

	switch (nID)
	{
	default:
		break;

	case IDC_SLIDER_EXPOSURE:
		val = ACSliderCtrl->GetPos();
		str.Format(L"%d", val);
		((CStatic*)GetDlgItem(IDC_STATIC_EXP))->SetWindowTextW(str);
		ASISetControlValue(iSelectedID, ASI_EXPOSURE, val * 1000, ASI_FALSE);
		UpdateData(FALSE);
		break;
	case IDC_SLIDER_GAIN:
		val = ACSliderCtrl->GetPos();
		str.Format(L"%d", val);
		((CStatic*)GetDlgItem(IDC_STATIC_GAIN))->SetWindowTextW(str);
		ASISetControlValue(iSelectedID, ASI_GAIN, val, ASI_FALSE);
		UpdateData(FALSE);
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CZWOViewFinderDlg::OnBnClickedButtonRescan()
{
	// TODO: Add your control notification handler code here
	iCamNum = ASIGetNumOfConnectedCameras();

	((CComboBox*)GetDlgItem(IDC_COMBO_CAMERAS))->ResetContent();

	int iSelectedIndex = -1, i;

	ASI_CAMERA_INFO CamInfoTemp;
	bool bDevOpened[ASICAMERA_ID_MAX] = { false };

	char buf[64];
	for (i = 0; i < iCamNum; i++)
	{
		ASIGetCameraProperty(&CamInfoTemp, i);

		if (iSelectedID == CamInfoTemp.CameraID)
			iSelectedIndex = i;
		if (ASIGetNumOfControls(CamInfoTemp.CameraID, &ConnectCamera[CamInfoTemp.CameraID].iCtrlNum) != ASI_ERROR_CAMERA_CLOSED)//the camera maybe have be closed, say be unplugged and plugged
			bDevOpened[CamInfoTemp.CameraID] = true;
		if (bDevOpened[CamInfoTemp.CameraID])
			sprintf_s(buf, "%s(ID %d opened)", CamInfoTemp.Name, CamInfoTemp.CameraID);
		else
			sprintf_s(buf, "%s(ID %d closed)", CamInfoTemp.Name, CamInfoTemp.CameraID);
		((CComboBox*)GetDlgItem(IDC_COMBO_CAMERAS))->AddString(CA2CT(buf));

	}
	for (i = 0; i < ASICAMERA_ID_MAX; i++)
	{
		if (!bDevOpened[i])
			CloseCam(i);
	}
	if (iSelectedIndex == -1)//
	{
		if (iCamNum > 0)
			iSelectedIndex = 0;
		else
			iSelectedIndex = -1;
		iSelectedID = -1;

	}

	((CComboBox*)GetDlgItem(IDC_COMBO_CAMERAS))->SetCurSel(iSelectedIndex);
	OnCbnSelchangeComboCameras();

}

void CZWOViewFinderDlg::CloseCam(int iID)
{

	if (iID < 0)
		return;

	if (ConnectCamera[iID].Status != closed)
	{
		StopCam(iID);
		ASICloseCamera(iID);
		ConnectCamera[iID].Status = closed;
		ReleaseImg(iID);
	}
	ReleaseMem(iID);

}

void CZWOViewFinderDlg::StopCam(int iID)
{
	if (iID < 0)
		return;
	//if (ConnectCamera[iID].Status == snaping)
	//{
	//	AbortSnap(iID);
	//}
	if (ConnectCamera[iID].Status == capturing)
	{
		ConnectCamera[iID].Status = opened;
		WaitForSingleObject(ConnectCamera[iID].Thr_CapVideo, 1000);
	}
	if (ConnectCamera[iID].Status == capturing || ConnectCamera[iID].Status == snaping)
	{
		ConnectCamera[iID].Status = opened;
		WaitForSingleObject(ConnectCamera[iID].Thr_Display, 1000);
		ReleaseImg(iID);
	}
	//if (iSelectedID == iID)
	//	ChangeButtonAppearence(opened);
}


void CZWOViewFinderDlg::OnCbnSelchangeComboCameras()
{
	int iCamIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_CAMERAS))->GetCurSel();

	//	ASIGetCameraID(iCamIndex, &iSelectedID);
	ASI_CAMERA_INFO CamInfoTemp;

	if (ASIGetCameraProperty(&CamInfoTemp, iCamIndex) == ASI_SUCCESS && CamInfoTemp.CameraID > -1 && CamInfoTemp.CameraID < ASICAMERA_ID_MAX)
	{
		iSelectedID = CamInfoTemp.CameraID;
		if (!ConnectCamera[iSelectedID].pASICameraInfo)
			ConnectCamera[iSelectedID].pASICameraInfo = new ASI_CAMERA_INFO;
		if (!ConnectCamera[iSelectedID].pASISupportedMode)
			ConnectCamera[iSelectedID].pASISupportedMode = new ASI_SUPPORTED_MODE;
		memcpy(ConnectCamera[iSelectedID].pASICameraInfo, &CamInfoTemp, sizeof(ASI_CAMERA_INFO));

		//ChangeButtonAppearence(ConnectCamera[iSelectedID].Status);
	}
	else
	{
		//ChangeButtonAppearence(closed);
	}
		


	//	ASIGetCameraProperty(iSelectedID, ConnectCamera[iSelectedID].pASICameraInfo);

	//RefreshInfoCtrl();
	//DrawControl();

}



void CZWOViewFinderDlg::MallocImg(int iID)
{
	int width, height, bin;
	long lBuffSize;
	ASI_IMG_TYPE image_type;
	ASIGetROIFormat(iID, &width, &height, &bin, &image_type);
	ConnectCamera[iID].ImageType = image_type;
	ConnectCamera[iID].width = width;
	ConnectCamera[iID].height = height;

	ReleaseImg(iID);
	switch (image_type)
	{
	case ASI_IMG_Y8:
	case ASI_IMG_RAW8:
		ConnectCamera[iID].pRgb = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
		lBuffSize = width * height;
		break;

	case ASI_IMG_RGB24:
		ConnectCamera[iID].pRgb = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
		lBuffSize = width * height * 3;
		break;

	case ASI_IMG_RAW16:
		ConnectCamera[iID].pRgb = cvCreateImage(cvSize(width, height), IPL_DEPTH_16U, 1);
		lBuffSize = width * height * 2;
		break;
	}
	ConnectCamera[iID].pTempImg = cvCreateImage(cvSize(width, height), ConnectCamera[iID].pRgb->depth, ConnectCamera[iID].pRgb->nChannels);
}

void CZWOViewFinderDlg::ReleaseImg(int iID)
{
	if (ConnectCamera[iID].pRgb)
		cvReleaseImage(&ConnectCamera[iID].pRgb);
	if (ConnectCamera[iID].pTempImg)
		cvReleaseImage(&ConnectCamera[iID].pTempImg);
	if (ConnectCamera[iID].pTempImgScaled)
		cvReleaseImage(&ConnectCamera[iID].pTempImgScaled);
}


void CZWOViewFinderDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (iSelectedID > -1 && iSelectedID < ASICAMERA_ID_MAX && ConnectCamera[iSelectedID].Status != closed)
	{
		ASIStartVideoCapture(iSelectedID);
		ConnectCamera[iSelectedID].Status = capturing;
		MallocImg(iSelectedID);
		ConnectCamera[iSelectedID].Thr_Display = (HANDLE)_beginthread(Display, NULL, this);
		ConnectCamera[iSelectedID].Thr_CapVideo = (HANDLE)_beginthread(CaptureVideo, NULL, this);
	}
	UpdateData(FALSE);
}

void CZWOViewFinderDlg::ReleaseMem(int iID)
{
	if (iID < 0)
		return;
	if (ConnectCamera[iID].pControlCaps)
	{
		delete[] ConnectCamera[iID].pControlCaps;
		ConnectCamera[iID].pControlCaps = NULL;
	}
	if (ConnectCamera[iID].pRefreshedIndex)
	{
		delete ConnectCamera[iID].pRefreshedIndex;
		ConnectCamera[iID].pRefreshedIndex = NULL;
	}
	if (ConnectCamera[iID].pASICameraInfo)
	{
		delete ConnectCamera[iID].pASICameraInfo;
		ConnectCamera[iID].pASICameraInfo = NULL;
	}
	if (ConnectCamera[iID].pASISupportedMode)
	{
		delete ConnectCamera[iID].pASISupportedMode;
		ConnectCamera[iID].pASISupportedMode = NULL;
	}
}

void CZWOViewFinderDlg::MallocMem(int iID)
{
	int i;
	if (ConnectCamera[iID].pControlCaps == NULL)
	{
		ConnectCamera[iID].pControlCaps = new ASI_CONTROL_CAPS[ConnectCamera[iID].iCtrlNum];
		for (i = 0; i < ConnectCamera[iID].iCtrlNum; i++)
			ASIGetControlCaps(iSelectedID, i, &ConnectCamera[iID].pControlCaps[i]);
	}
	if (ConnectCamera[iID].pRefreshedIndex == NULL)
		ConnectCamera[iID].pRefreshedIndex = new bool[ConnectCamera[iID].iCtrlNum];
	if (ConnectCamera[iID].pASICameraInfo == NULL)
		ConnectCamera[iID].pASICameraInfo = new ASI_CAMERA_INFO;
	if (ConnectCamera[iID].pASISupportedMode == NULL)
		ConnectCamera[iID].pASISupportedMode = new ASI_SUPPORTED_MODE;
}


void Display(LPVOID params)
{
	CZWOViewFinderDlg* dlg = (CZWOViewFinderDlg*)params;
	int iCamIDInThread;
	iCamIDInThread = dlg->iSelectedID;

	char buf[128] = { 0 };

	char name[64], temp[64];
	char name_roi[64];

	if (dlg->ConnectCamera[iCamIDInThread].Status == dlg->snaping)
		sprintf_s(name, "snap");
	else if (dlg->ConnectCamera[iCamIDInThread].Status == dlg->capturing)
	{
		sprintf_s(name_roi, "video_roi");
		sprintf_s(name, "video");
	}
	strcat_s(name, dlg->ConnectCamera[iCamIDInThread].pASICameraInfo->Name);
	sprintf_s(temp, " ID%d", iCamIDInThread);
	strcat_s(name, temp);

	cvNamedWindow(name, 1);
	HWND hWnd = (HWND)cvGetWindowHandle(name);
	HWND hParent = GetParent(hWnd);
	::SetParent(hWnd, dlg->GetDlgItem(IDC_STATIC_DRAW)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);


	cvNamedWindow(name_roi, 1);
	HWND hWnd_roi = (HWND)cvGetWindowHandle(name_roi);
	HWND hParent_roi = GetParent(hWnd_roi);
	::SetParent(hWnd_roi, dlg->GetDlgItem(IDC_STATIC_ROI)->m_hWnd);
	::ShowWindow(hParent_roi, SW_HIDE);


	Dlg_Thread dlgindex;
	dlgindex.pDlg = dlg;
	dlgindex.ID = iCamIDInThread;

	cvSetMouseCallback(name, onMouse, (void*)&dlgindex);

	//	RECT rectCenter;
	int x, y, centerX, centerY;
	centerX = dlg->ConnectCamera[iCamIDInThread].width / 2;
	centerY = dlg->ConnectCamera[iCamIDInThread].height / 2;
	CvPoint cvPt0, cvPt1;
	
	while (dlg->ConnectCamera[iCamIDInThread].Status == dlg->snaping || dlg->ConnectCamera[iCamIDInThread].Status == dlg->capturing)
	{

		if (dlg->ConnectCamera[iCamIDInThread].bNewImg)
		{


			//if (iCamIDInThread == dlg->iSelectedID && dlg->bDrawRect)
			//{
			//	cvPt0 = cvPoint(dlg->pt0.x, dlg->pt0.y);
			//	cvPt1 = cvPoint(dlg->pt1.x, dlg->pt0.y);
			//	CvScalar color = cvScalar((double)255.0, (double)255.0, (double)0.0, (double)0.0);
			//	cvLine(dlg->ConnectCamera[iCamIDInThread].pTempImg, cvPt0, cvPt1, color, 1);
			//	cvPt0 = cvPoint(dlg->pt1.x, dlg->pt0.y);
			//	cvPt1 = cvPoint(dlg->pt1.x, dlg->pt1.y);
			//	cvLine(dlg->ConnectCamera[iCamIDInThread].pTempImg, cvPt0, cvPt1, color, 1);
			//	cvPt0 = cvPoint(dlg->pt1.x, dlg->pt1.y);
			//	cvPt1 = cvPoint(dlg->pt0.x, dlg->pt1.y);
			//	cvLine(dlg->ConnectCamera[iCamIDInThread].pTempImg, cvPt0, cvPt1, color, 1);
			//	cvPt0 = cvPoint(dlg->pt0.x, dlg->pt0.y);
			//	cvPt1 = cvPoint(dlg->pt0.x, dlg->pt1.y);
			//	cvLine(dlg->ConnectCamera[iCamIDInThread].pTempImg, cvPt0, cvPt1, color, 1);
			//}
		}
		if (dlg->fScale < 1)
		{

			if (!dlg->ConnectCamera[iCamIDInThread].pTempImgScaled
				|| dlg->ConnectCamera[iCamIDInThread].fOldScale != dlg->fScale
				|| dlg->ConnectCamera[iCamIDInThread].bNewImg)
			{
				dlg->ConnectCamera[iCamIDInThread].bNewImg = false;
				int WidthScaled, HeightScaled;
				WidthScaled = dlg->ConnectCamera[iCamIDInThread].width * dlg->fScale;
				HeightScaled = dlg->ConnectCamera[iCamIDInThread].height * dlg->fScale;

				dlg->ConnectCamera[iCamIDInThread].fOldScale = dlg->fScale;

				cvReleaseImage(&dlg->ConnectCamera[iCamIDInThread].pTempImgScaled);
				//cv::Mat img_raw(Frame::HEIGHT, Frame::WIDTH, CV_8UC1, (void*)(frame->frame_buffer_));
				//cvtColor(img_raw, img_preview, cv::COLOR_BayerBG2BGR);

				dlg->ConnectCamera[iCamIDInThread].pTempImgScaled = cvCreateImage(cvSize(WidthScaled, HeightScaled), dlg->ConnectCamera[iCamIDInThread].pTempImg->depth, dlg->ConnectCamera[iCamIDInThread].pTempImg->nChannels);

				cvResize(dlg->ConnectCamera[iCamIDInThread].pTempImg, dlg->ConnectCamera[iCamIDInThread].pTempImgScaled, CV_INTER_AREA);
				cvSetImageROI(dlg->ConnectCamera[iCamIDInThread].pRgb, cvRect(0, 0, WidthScaled, HeightScaled));
				cvCopy(dlg->ConnectCamera[iCamIDInThread].pTempImgScaled, dlg->ConnectCamera[iCamIDInThread].pRgb);

				cvShowImage(name, dlg->ConnectCamera[iCamIDInThread].pRgb);
				//cvSaveImage("E:\\aaTest.jpg", dlg->ConnectCamera[iCamIDInThread].pRgb);//If it works, This operation will save a jpg.
			}
		}
		else
		{

			if (dlg->ConnectCamera[iCamIDInThread].bNewImg || dlg->ConnectCamera[iCamIDInThread].fOldScale != dlg->fScale)
			{
				dlg->ConnectCamera[iCamIDInThread].fOldScale = dlg->fScale;
				//cvResetImageROI(dlg->ConnectCamera[iCamIDInThread].pRgb);
				// need to find a way not to release the image and use the same memory area again
				cvReleaseImage(&dlg->ConnectCamera[iCamIDInThread].pRgb);

				//memcpy((BYTE*)(dlg->ConnectCamera[iCamIDInThread].pRgb->imageData), (BYTE*)(dlg->ConnectCamera[iCamIDInThread].pTempImg->imageData),dlg->ConnectCamera[iCamIDInThread].pTempImg->width * dlg->ConnectCamera[iCamIDInThread].pTempImg->height * dlg->ConnectCamera[iCamIDInThread].pTempImg->depth * dlg->ConnectCamera[iCamIDInThread].pTempImg->nChannels / 8);

				dlg->ConnectCamera[iCamIDInThread].pRgb = cvCreateImage(cvSize(dlg->ConnectCamera[iCamIDInThread].width, dlg->ConnectCamera[iCamIDInThread].height), 8,3);
				cvCvtColor(dlg->ConnectCamera[iCamIDInThread].pTempImg, dlg->ConnectCamera[iCamIDInThread].pRgb, CV_BayerBG2BGR);
				dlg->ConnectCamera[iCamIDInThread].bNewImg = false;
				cvRectangle(dlg->ConnectCamera[iCamIDInThread].pRgb, cvPoint(50,50), cvPoint(200, 200), cvScalar(0, 255, 0, 0), 2);

				cvShowImage(name, dlg->ConnectCamera[iCamIDInThread].pRgb);
				//cvShowImage(name, pDebayeredImg);

				// ROI
				CvRect rect = cvRect(50, 50, 200, 200);
				cvSetImageROI(dlg->ConnectCamera[iCamIDInThread].pRgb, rect);
				// 300%
				IplImage* tmp = cvCreateImage(cvSize(dlg->ConnectCamera[iCamIDInThread].pRgb->roi->width*3, dlg->ConnectCamera[iCamIDInThread].pRgb->roi->height * 3), dlg->ConnectCamera[iCamIDInThread].pRgb->depth, dlg->ConnectCamera[iCamIDInThread].pRgb->nChannels);
				cvResize(dlg->ConnectCamera[iCamIDInThread].pRgb, tmp, CV_INTER_AREA);
				cvShowImage(name_roi, tmp);
				
				cvResetImageROI(dlg->ConnectCamera[iCamIDInThread].pRgb);
				
			}
		}

		//	cvText(dlg->ConnectCamera[iCamIDInThread].pRgb, buf, iTextX, iTextY);
		//	cvShowImage(WindowName, dlg->ConnectCamera[iCamIDInThread].pRgb);
		cvWaitKey(1);
	}
	cvDestroyWindow(name);
	_endthread();

}
void CaptureVideo(LPVOID params)
{
	CZWOViewFinderDlg* dlg = (CZWOViewFinderDlg*)params;
	int iCamIDInThread = dlg->iSelectedID;
	int time1, time2;
	time1 = GetTickCount();
	int iDropFrame;
	long lBuffSize;
	char buf[128] = { 0 };
	int count = 0;

	lBuffSize = dlg->ConnectCamera[iCamIDInThread].width * dlg->ConnectCamera[iCamIDInThread].height *
		dlg->ConnectCamera[iCamIDInThread].pRgb->depth * dlg->ConnectCamera[iCamIDInThread].pRgb->nChannels / 8;

	long expMs;
	ASI_BOOL bAuto = ASI_FALSE;
	if (dlg->m_CamMode == 0)
	{
		while (dlg->ConnectCamera[iCamIDInThread].Status == dlg->capturing)
		{
			time2 = GetTickCount();
			if (time2 - time1 > 1000)
			{
				ASIGetDroppedFrames(iCamIDInThread, &iDropFrame);
				sprintf_s(buf, "fps:%d dropped frames:%d", count, iDropFrame);
				count = 0;
				time1 = GetTickCount();
			}
			ASIGetControlValue(iCamIDInThread, ASI_EXPOSURE, &expMs, &bAuto);
			expMs /= 1000;
			if (ASIGetVideoData(iCamIDInThread, (BYTE*)(dlg->ConnectCamera[iCamIDInThread].pTempImg->imageData), lBuffSize, expMs * 2 + 500) == ASI_SUCCESS)
			{
				dlg->ConnectCamera[iCamIDInThread].bNewImg = true;
				count++;
			}

			if (iCamIDInThread == dlg->iSelectedID && dlg->ConnectCamera[iCamIDInThread].Status == dlg->capturing)
			{
				dlg->m_static_msg = buf;
				SendMessage(dlg->m_hWnd, WM_MY_MSG, WM_UPDATEUISTATE, FALSE);
			}
		}
	}
	else
	{
		while (dlg->ConnectCamera[iCamIDInThread].Status == dlg->capturing)
		{
			time2 = GetTickCount();
			if (time2 - time1 > 1000)
			{
				//ASIGetDroppedFrames(iCamIDInThread, &iDropFrame);
				sprintf_s(buf, "fps:%d", count);
				count = 0;
				time1 = GetTickCount();
			}
			if (ASIGetVideoData(iCamIDInThread, (BYTE*)(dlg->ConnectCamera[iCamIDInThread].pTempImg->imageData), lBuffSize, 1000) == ASI_SUCCESS)
			{
				dlg->ConnectCamera[iCamIDInThread].bNewImg = true;
				count++;
			}

			if (iCamIDInThread == dlg->iSelectedID && dlg->ConnectCamera[iCamIDInThread].Status == dlg->capturing)
			{
				dlg->m_static_msg = buf;
				SendMessage(dlg->m_hWnd, WM_MY_MSG, WM_UPDATEUISTATE, FALSE);
			}
		}
	}
	dlg->m_static_msg = "capture stop";
	SendMessage(dlg->m_hWnd, WM_MY_MSG, WM_UPDATEUISTATE, FALSE);

	ASIStopVideoCapture(iCamIDInThread);

	_endthread();


}


void onMouse(int Event, int x, int y, int flags, void* param)
{
	Dlg_Thread* pDlgThread = (Dlg_Thread*)param;
	CZWOViewFinderDlg* dlg;
	dlg = pDlgThread->pDlg;
	//switch (Event)
	//{
	//case CV_EVENT_LBUTTONDOWN:
	//	if (dlg->ConnectCamera[pDlgThread->ID].Status != dlg->closed && pDlgThread->ID != dlg->iSelectedID)
	//	{
	//		dlg->bDrawRect = false;
	//		dlg->iSelectedID = pDlgThread->ID;
	//		dlg->bThreadCall = true;//call UpdateData() in thread  will cause error
	//		if (dlg->ConnectCamera[dlg->iSelectedID].Status == dlg->capturing)
	//			dlg->ChangeButtonAppearence(dlg->capturing);
	//		else if (dlg->ConnectCamera[dlg->iSelectedID].Status == dlg->snaping)
	//			dlg->ChangeButtonAppearence(dlg->snaping);

	//		dlg->RefreshInfoCtrl();
	//		SendMessage(dlg->m_hWnd, WM_MY_MSG, 3210, FALSE);//call DrawControl()
	//		dlg->bThreadCall = false;
	//		ASI_CAMERA_INFO CamInfoTemp;
	//		for (int i = 0; i < dlg->iCamNum; i++)
	//		{
	//			//				ASIGetCameraID(i, &id);
	//			ASIGetCameraProperty(&CamInfoTemp, i);
	//			if (CamInfoTemp.CameraID == dlg->iSelectedID)
	//			{
	//				dlg->CComboCamera.SetCurSel(i);
	//				break;
	//			}
	//		}
	//		SendMessage(dlg->m_hWnd, WM_MY_MSG, WM_UPDATEUISTATE, FALSE);
	//	}
	//	else
	//	{
	//		if (dlg->bDrawRect)
	//			dlg->bDrawRect = false;
	//		dlg->bLBDown = true;
	//		dlg->pt0.x = x;
	//		dlg->pt0.y = y;
	//	}
	//	break;

	//case CV_EVENT_MOUSEMOVE:
	//	if (dlg->bLBDown)
	//	{
	//		dlg->pt1.x = x;
	//		dlg->pt1.y = y;
	//		dlg->bDrawRect = true;
	//	}

	//	break;

	//case CV_EVENT_LBUTTONUP:

	//	dlg->bLBDown = false;
	//	break;

	//case CV_EVENT_RBUTTONDOWN:
	//	dlg->SaveImage.CamID = pDlgThread->ID;
	//	dlg->SaveImage.cvHwnd = pDlgThread->cvHandle;//used to locate messagebox window
	//	PostMessage(dlg->m_hWnd, WM_MY_MSG, WM_RBUTTONDOWN, FALSE);//not wait message process function return 
	//	break;
	//}


}



void CZWOViewFinderDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	int iCamIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_CAMERAS))->GetCurSel();
	if (iCamIndex == -1)
		return;

	if (iCamIndex >= ASICAMERA_ID_MAX)
	{
		CString cs;
		cs.Format(L"Can't open cameras more than %d", sizeof(ConnectCamera) / sizeof(ConnectedCam));
		MessageBox(cs);
		return;
	}
	int i = 0, count = 0;

	//	ASIGetCameraID(iCamIndex, &iSelectedID);
	ASI_CAMERA_INFO CamInfoTemp;
	ASI_SUPPORTED_MODE CamSupportedModeTemp;
	ASIGetCameraProperty(&CamInfoTemp, iCamIndex);
	iSelectedID = CamInfoTemp.CameraID;

	if (ASIOpenCamera(iSelectedID) == ASI_SUCCESS)
	{
		ASIInitCamera(iSelectedID);
		ConnectCamera[iSelectedID].Status = opened;
		ConnectCamera[iSelectedID].iSnapTime = 0;


		ASIGetNumOfControls(iSelectedID, &ConnectCamera[iSelectedID].iCtrlNum);

		ReleaseMem(iSelectedID);
		MallocMem(iSelectedID);

		ASIGetCameraSupportMode(iSelectedID, &CamSupportedModeTemp);
		memcpy(ConnectCamera[iSelectedID].pASISupportedMode, &CamSupportedModeTemp, sizeof(ASI_SUPPORTED_MODE));

		ASISetControlValue(iSelectedID, ASI_BANDWIDTHOVERLOAD, 60, ASI_TRUE);
		for (i = 0; i < ConnectCamera[iSelectedID].iCtrlNum; i++)
		{
			switch (ConnectCamera[iSelectedID].pControlCaps[i].ControlType)
			{
				case (ASI_GAIN):
					((CSliderCtrl*)GetDlgItem(IDC_SLIDER_GAIN))->SetRangeMin(ConnectCamera[iSelectedID].pControlCaps[i].MinValue);
					((CSliderCtrl*)GetDlgItem(IDC_SLIDER_GAIN))->SetRangeMax(ConnectCamera[iSelectedID].pControlCaps[i].MaxValue);
					break;
				case (ASI_EXPOSURE):
					((CSliderCtrl*)GetDlgItem(IDC_SLIDER_EXPOSURE))->SetRangeMin(0);
					((CSliderCtrl*)GetDlgItem(IDC_SLIDER_EXPOSURE))->SetRangeMax(10000);

					//((CSliderCtrl*)GetDlgItem(IDC_SLIDER_EXPOSURE))->SetRangeMin(ConnectCamera[iSelectedID].pControlCaps[i].MinValue);
					//((CSliderCtrl*)GetDlgItem(IDC_SLIDER_EXPOSURE))->SetRangeMax(ConnectCamera[iSelectedID].pControlCaps[i].MaxValue);
					break;

			}
		}
			

	}
	else
		MessageBox(L"reopen or open fail");

	OnBnClickedButtonRescan();


}


void CZWOViewFinderDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	StopCam(iSelectedID);
}




void CZWOViewFinderDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	for (int i = 0; i < sizeof(ConnectCamera) / sizeof(ConnectedCam); i++)
		CloseCam(i);

	CDialogEx::OnClose();
}
