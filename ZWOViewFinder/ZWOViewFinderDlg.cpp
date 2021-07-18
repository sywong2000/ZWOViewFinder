
// ZWOViewFinderDlg.cpp : implementation file
//
#include "pch.h"
#include "framework.h"
#include "ZWOViewFinder.h"
#include "ZWOViewFinderDlg.h"
#include "afxdialogex.h"
#include "include/preview.h"
#include "include/agc.h"
#include <cmath>
#include <deque>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "include/Frame.h"
#include "include/camera.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "include/ASICamera2.h"

using namespace std::chrono;

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



CZWOViewFinderDlg::CZWOViewFinderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ZWOVIEWFINDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZWOViewFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZWOViewFinderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
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
	//int nNumOfConnectedCameras = ASIGetNumOfConnectedCameras();

	return TRUE;  // return TRUE  unless you set the focus to a control
}



void CZWOViewFinderDlg::StartPreview()
{
	ASI_CAMERA_INFO CamInfo;
	camera::init_camera(CamInfo, cam_name, binning);

	// Create pool of frame buffers
	Frame::WIDTH = CamInfo.MaxWidth / binning;
	Frame::HEIGHT = CamInfo.MaxHeight / binning;
	Frame::IMAGE_SIZE_BYTES = Frame::WIDTH * Frame::HEIGHT;
	constexpr size_t FRAME_POOL_SIZE = 64;
	static std::deque<Frame> frames;

	for (size_t i = 0; i < FRAME_POOL_SIZE; i++)
	{
		// Frame objects add themselves to unused_deque on construction
		frames.emplace_back();
	}
	static std::thread preview_thread(preview, CamInfo.IsColorCam == ASI_TRUE);
	static std::thread agc_thread(agc);

	camera::run_camera(CamInfo);

	preview_thread.join();
	agc_thread.join();
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
