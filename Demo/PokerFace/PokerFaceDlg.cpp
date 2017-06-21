
// PokerFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PokerFace.h"
#include "PokerFaceDlg.h"
#include "afxdialogex.h"
#include "mformat.hpp"
#include "common_tools.hpp"
//#include "face_tools.hpp"
#include "mformat.hpp"
#include "image_tools.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPokerFaceDlg 对话框

CPokerFaceDlg::CPokerFaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_POKERFACE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPokerFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPokerFaceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_IMG_BTN_1, &CPokerFaceDlg::OnBnClickedImgBtn1)
	ON_BN_CLICKED(IDC_IMG_BTN_2, &CPokerFaceDlg::OnBnClickedImgBtn2)
	ON_BN_CLICKED(IDC_COP_BTN, &CPokerFaceDlg::OnBnClickedCopBtn)
	ON_BN_CLICKED(IDC_IMGUP_BTN_1, &CPokerFaceDlg::OnBnClickedImgupBtn1)
	ON_BN_CLICKED(IDC_IMGDOWN_BTN_1, &CPokerFaceDlg::OnBnClickedImgdownBtn1)
	ON_BN_CLICKED(IDC_IMGUP_BTN_2, &CPokerFaceDlg::OnBnClickedImgupBtn2)
	ON_BN_CLICKED(IDC_IMGDOWN_BTN_2, &CPokerFaceDlg::OnBnClickedImgdownBtn2)
END_MESSAGE_MAP()


// CPokerFaceDlg 消息处理程序

BOOL CPokerFaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// Initial Opencv Window
	imgWndStr1 = "imgWndStr1";
	cv::namedWindow(imgWndStr1, cv::WINDOW_AUTOSIZE);
	HWND hWnd1 = (HWND)cvGetWindowHandle(imgWndStr1.data());
	HWND hParent1 = ::GetParent(hWnd1);
	::SetParent(hWnd1, GetDlgItem(IDC_IMG_STC_1)->m_hWnd);
	::ShowWindow(hParent1, SW_HIDE);

	imgWndStr2 = "imgWndStr2";
	cv::namedWindow(imgWndStr2, cv::WINDOW_AUTOSIZE);
	HWND hWnd2 = (HWND)cvGetWindowHandle(imgWndStr2.data());
	HWND hParent2 = ::GetParent(hWnd2);
	::SetParent(hWnd2, GetDlgItem(IDC_IMG_STC_2)->m_hWnd);
	::ShowWindow(hParent2, SW_HIDE);


	imgIndex1 = -1;
	imgIndex2 = -1;
	appPath = tools::app_path()+"\\";
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPokerFaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPokerFaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPokerFaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPokerFaceDlg::Update(int leftOrRight)
{
	if (leftOrRight ==-1)
	{
		if (!image1.empty())
		{
			CRect rect1;
			GetDlgItem(IDC_IMG_STC_1)->GetClientRect(&rect1);
			double ratioH = (double)rect1.Height() / image1.rows;
			double ratioW = (double)rect1.Width() / image1.cols;
			double ration = std::min(ratioH, ratioW);
			cv::Size reSize(ration*image1.cols, ration*image1.rows);
			cv::Mat imagedst1;
			cv::resize(image1, imagedst1, reSize);
			cv::imshow(imgWndStr1, imagedst1);
		}
		if (!image2.empty())
		{
			CRect rect2;
			GetDlgItem(IDC_IMG_STC_2)->GetClientRect(&rect2);
			double ratioH = (double)rect2.Height() / image2.rows;
			double ratioW = (double)rect2.Width() / image2.cols;
			double ration = std::min(ratioH, ratioW);
			cv::Size reSize(ration*image2.cols, ration*image2.rows);
			cv::Mat imagedst2;
			cv::resize(image2, imagedst2, reSize);
			cv::imshow(imgWndStr2, imagedst2);
		}
	} 
	else if (leftOrRight ==0)
	{
		if (!image1.empty())
		{
			CRect rect1;
			GetDlgItem(IDC_IMG_STC_1)->GetClientRect(&rect1);
			double ratioH = (double)rect1.Height() / image1.rows;
			double ratioW = (double)rect1.Width() / image1.cols;
			double ration = std::min(ratioH, ratioW);
			cv::Size reSize(ration*image1.cols, ration*image1.rows);
			cv::Mat imagedst1;
			cv::resize(image1, imagedst1, reSize);
			cv::imshow(imgWndStr1, imagedst1);
		}
	}
	else if (leftOrRight ==1)
	{
		if (!image2.empty())
		{
			CRect rect2;
			GetDlgItem(IDC_IMG_STC_2)->GetClientRect(&rect2);
			double ratioH = (double)rect2.Height() / image2.rows;
			double ratioW = (double)rect2.Width() / image2.cols;
			double ration = std::min(ratioH, ratioW);
			cv::Size reSize(ration*image2.cols, ration*image2.rows);
			cv::Mat imagedst2;
			cv::resize(image2, imagedst2, reSize);
			cv::imshow(imgWndStr2, imagedst2);
		}
	}
}


void CPokerFaceDlg::OnBnClickedImgBtn1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString StrFile;
	CString defExe("jpg");
	CString defFileName("*.jpg");
	CString defFilter("图像(*.jpg)|*.jpg|所有文件(*.*)|*.*|");
	CFileDialog dlg(true, defExe, defFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, defFilter);
	dlg.m_ofn.lpstrInitialDir = _T(".\\");
	if (dlg.DoModal() == IDOK)
	{
		StrFile = dlg.GetPathName();
		// do something
		imgFileStr1.clear();
		imgfaceInfo1.clear();
		imgLandMarks1.clear();
		std::string imgFullPathStr1 = tools::full_to_path(tools::ws_to_s(StrFile.GetString()));
		imgPathStr1 = tools::full_to_path(imgFullPathStr1);
		tools::dir_files(imgPathStr1, "*.jpg", imgFileStr1);
		for (int i = 0; i < imgFileStr1.size(); i++)
		{
			faceInfoEx tmpfaceInfoEx;
			imgfaceInfo1.push_back(tmpfaceInfoEx);
			faceLandMark77 tmpfaceLandMark77;
			imgLandMarks1.push_back(tmpfaceLandMark77);
		}
		LoadImg1(0);
		Update(0);
	}
}

void CPokerFaceDlg::LoadImg1(int index1)
{
	// TODO: 在此添加控件通知处理程序代码
	if (imgFileStr1.size() == 0) return;
	imgIndex1 = std::max(index1, 0);
	imgIndex1 = std::min(imgIndex1, (int)imgFileStr1.size());
	cv::Mat tmpImg1 = cv::imread(imgPathStr1 + imgFileStr1.at(imgIndex1), cv::IMREAD_COLOR);
	cv::Size reSize = cv::Size(tmpImg1.cols*2, tmpImg1.rows*2);
	cv::resize(tmpImg1, image1, reSize);
	cv::Mat gray1;
	cv::cvtColor(image1, gray1, CV_RGB2GRAY);

	if (!stasm_search_single(&(imgfaceInfo1.at(imgIndex1).exist), imgLandMarks1.at(imgIndex1).marks,
		(char*)gray1.data, gray1.cols, gray1.rows, appPath.c_str() , appPath.c_str()))
	{
		MessageBoxA(0,stasm_lasterr(),"Error in stasm_search_single:", MB_OK);
	}
	if (imgfaceInfo1.at(imgIndex1).exist < 1)
	{
		cv::Point origin;
		origin.x = 1;
		origin.y = image1.rows/10;
		cv::putText(image1, "No face", origin, cv::FONT_HERSHEY_COMPLEX, 0.5, CV_RGB(255, 255, 255), 1, 8, 0);
	}
	else
	{
		stasm_force_points_into_image(imgLandMarks1.at(imgIndex1).marks, image1.cols, image1.rows);
		for (int ik = 0; ik < stasm_NLANDMARKS; ik++)
		{
			cv::drawMarker(image1, cv::Point(imgLandMarks1.at(imgIndex1).marks[ik * 2], imgLandMarks1.at(imgIndex1).marks[ik * 2 + 1]),
				CV_RGB(0, 0, 255), cv::MARKER_CROSS, 2, 1);
		}
	}
}

void CPokerFaceDlg::OnBnClickedImgupBtn1()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadImg1(imgIndex1 - 1);
	Update(0);
}


void CPokerFaceDlg::OnBnClickedImgdownBtn1()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadImg1(imgIndex1 + 1);
	Update(0);
}



void CPokerFaceDlg::OnBnClickedImgBtn2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString StrFile;
	CString defExe("jpg");
	CString defFileName("*.jpg");
	CString defFilter("图像(*.jpg)|*.jpg|所有文件(*.*)|*.*|");
	CFileDialog dlg(true, defExe, defFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, defFilter);
	dlg.m_ofn.lpstrInitialDir = _T(".\\");
	if (dlg.DoModal() == IDOK)
	{
		StrFile = dlg.GetPathName();
		// do something
		imgFileStr2.clear();
		imgfaceInfo2.clear();
		imgLandMarks2.clear();
		std::string imgFullPathStr2 = tools::full_to_path(tools::ws_to_s(StrFile.GetString()));
		imgPathStr2 = tools::full_to_path(imgFullPathStr2);
		tools::dir_files(imgPathStr2, "*.jpg", imgFileStr2);
		for (int i = 0; i < imgFileStr2.size(); i++)
		{
			faceInfoEx tmpfaceInfoEx;
			imgfaceInfo2.push_back(tmpfaceInfoEx);
			faceLandMark77 tmpfaceLandMark77;
			imgLandMarks2.push_back(tmpfaceLandMark77);
		}
		LoadImg2(0);
		Update(1);
	}
}

void CPokerFaceDlg::LoadImg2(int index2)
{
	// TODO: 在此添加控件通知处理程序代码
	if (imgFileStr2.size() == 0) return;
	imgIndex2 = std::max(index2, 0);
	imgIndex2 = std::min(imgIndex2, (int)imgFileStr2.size());
	cv::Mat tmpImg2 = cv::imread(imgPathStr2 + imgFileStr2.at(imgIndex2), cv::IMREAD_COLOR);
	cv::Size reSize = cv::Size(tmpImg2.cols * 2, tmpImg2.rows * 2);
	cv::resize(tmpImg2, image2, reSize);
	cv::Mat gray2;
	cv::cvtColor(image2, gray2, CV_RGB2GRAY);

	if (!stasm_search_single(&(imgfaceInfo2.at(imgIndex2).exist), imgLandMarks2.at(imgIndex2).marks,
		(char*)gray2.data, gray2.cols, gray2.rows, appPath.c_str(), appPath.c_str()))
	{
		MessageBoxA(0, stasm_lasterr(), "Error in stasm_search_single:", MB_OK);
	}
	if (imgfaceInfo2.at(imgIndex2).exist < 1)
	{
		cv::Point origin;
		origin.x = 1;
		origin.y = image2.rows / 10;
		cv::putText(image2, "No face", origin, cv::FONT_HERSHEY_COMPLEX, 2, CV_RGB(255, 255, 255), 2, 8, 0);
	}
	else
	{
		stasm_force_points_into_image(imgLandMarks2.at(imgIndex2).marks, image2.cols, image2.rows);
		for (int ik = 0; ik < stasm_NLANDMARKS; ik++)
		{
			cv::drawMarker(image2, cv::Point(imgLandMarks2.at(imgIndex2).marks[ik * 2], imgLandMarks2.at(imgIndex2).marks[ik * 2 + 1]),
				CV_RGB(0, 0, 255), cv::MARKER_CROSS, 8, 2);
		}
	}
}

void CPokerFaceDlg::OnBnClickedImgupBtn2()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadImg2(imgIndex2 - 1);
	Update(1);
}


void CPokerFaceDlg::OnBnClickedImgdownBtn2()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadImg2(imgIndex2 + 1);
	Update(1);
}



void CPokerFaceDlg::OnBnClickedCopBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	//Load faceTool.
	//std::string exe_path_str = tools::app_path();
	//tools::faceDetector face_detector;
	//face_detector.initial(tools::accept_size_3X2,
	//	(exe_path_str + "\\seeta_fd_frontal_v1.0.bin").c_str(),
	//	(exe_path_str + "\\seeta_fa_v1.1.bin").c_str(),
	//	(exe_path_str + "\\seeta_fr_v1.0.bin").c_str());

	//int imageNum = 0;
	//if (!image1.empty()) imageNum++;
	//if (!image2.empty()) imageNum++;

	////Deal different call.
	//if (imageNum == 0) return;
	//else if (imageNum == 1) return;
	//else if (imageNum == 2)
	//{
	//	//Load images
	//	seeta::ImageData img_data1(image1.cols, image1.rows, image1.channels());
	//	img_data1.data = image1.data;
	//	cv::Mat gray_img1;
	//	cv::cvtColor(image1, gray_img1, CV_RGB2GRAY);
	//	seeta::ImageData gray_img_data1(gray_img1.cols, gray_img1.rows, gray_img1.channels());
	//	gray_img_data1.data = gray_img1.data;

	//	seeta::ImageData img_data2(image2.cols, image2.rows, image2.channels());
	//	img_data2.data = image2.data;
	//	cv::Mat gray_img2;
	//	cv::cvtColor(image2, gray_img2, CV_RGB2GRAY);
	//	seeta::ImageData gray_img_data2(gray_img2.cols, gray_img2.rows, gray_img2.channels());
	//	gray_img_data2.data = gray_img2.data;

	//	// Detect face.
	//	std::vector <seeta::FaceInfo> face_info1;
	//	std::vector <tools::faceLandMark> face_marks1;
	//	long t0 = cv::getTickCount();
	//	bool isOK = face_detector.detect(gray_img_data1, face_info1, face_marks1);
	//	long t1 = cv::getTickCount();
	//	double secs = (t1 - t0) / cv::getTickFrequency();
	//	std::string debugStr = tools::f_to_s(secs);
	//	OutputDebugStringA(("Detect take time = " + debugStr + "\n").c_str());

	//	std::vector <seeta::FaceInfo> face_info2;
	//	std::vector <tools::faceLandMark> face_marks2;
	//	t0 = cv::getTickCount();
	//	isOK = face_detector.detect(gray_img_data2, face_info2, face_marks2);
	//	t1 = cv::getTickCount();
	//	secs = (t1 - t0) / cv::getTickFrequency();
	//	debugStr = tools::f_to_s(secs);
	//	OutputDebugStringA(("Detect take time = " + debugStr + "\n").c_str());

	//	float similar=0;
	//	std::string similarStr;
	//	if (face_marks1.size() > 0 && face_marks2.size() > 0)
	//	{
	//		//int face_rows = 0;
	//		//int face_cols = 0;
	//		//int face_chn = 0;
	//		//face_detector.precorp(face_cols, face_rows, face_chn);

	//		//cv::Mat corp_img1=cv::Mat(face_rows,face_cols,CV_8UC(face_chn));
	//		//seeta::ImageData corp_img_data1(corp_img1.cols, corp_img1.rows, corp_img1.channels());
	//		//corp_img_data1.data = corp_img1.data;
	//		//t0 = cv::getTickCount();
	//		//face_detector.corp(img_data1, face_marks1.at(0), corp_img_data1);
	//		//t1 = cv::getTickCount();
	//		//secs = (t1 - t0) / cv::getTickFrequency();
	//		//debugStr = tools::f_to_s(secs);
	//		//OutputDebugStringA(("Crop take time = " + debugStr + "\n").c_str());

	//		//cv::Mat corp_img2 = cv::Mat(face_rows, face_cols, CV_8UC(face_chn));
	//		//seeta::ImageData corp_img_data2(corp_img2.cols, corp_img2.rows, corp_img2.channels());
	//		//corp_img_data2.data = corp_img2.data;
	//		//t0 = cv::getTickCount();
	//		//face_detector.corp(img_data2, face_marks2.at(0), corp_img_data2);
	//		//t1 = cv::getTickCount();
	//		//secs = (t1 - t0) / cv::getTickFrequency();
	//		//debugStr = tools::f_to_s(secs);
	//		//OutputDebugStringA(("Crop take time = " + debugStr + "\n").c_str());

	//		t0 = cv::getTickCount();
	//		similar = face_detector.corp_compare(img_data1, img_data2, face_marks1.at(0).mark, face_marks2.at(0).mark);
	//		t1 = cv::getTickCount();
	//		secs = (t1 - t0) / cv::getTickFrequency();
	//		similar -= 0.40;
	//		similar /= 0.1;
	//		similar = 1 / (1 + exp(-similar));
	//		debugStr = tools::f_to_s(secs);
	//		similarStr = tools::f_to_s(similar);
	//		OutputDebugStringA(("Corp Compare take time = " + debugStr + ",similar=" + similarStr +"\n").c_str());
	//		//MessageBoxA(0, ("Similaraty=" + similarStr).c_str(), "Similaraty", MB_OK);
	//	}

	//	// Show detected face
	//	for (int i = 0; i < face_info1.size(); i++)
	//	{
	//		cv::Rect face_rect;
	//		face_rect.x = face_info1[i].bbox.x;
	//		face_rect.y = face_info1[i].bbox.y;
	//		face_rect.width = face_info1[i].bbox.width;
	//		face_rect.height = face_info1[i].bbox.height;
	//		cv::rectangle(image1, face_rect, CV_RGB(0, 255, 255), 1, 8, 0);
	//		//for (int k = 0; k < 5; k++)
	//		//{
	//		//	cv::circle(image1, cv::Point(face_marks1.at(i).mark[k].x, face_marks1.at(i).mark[k].y), 4,
	//		//		CV_RGB(0, 255, 255), 1);
	//		//}
	//			cv::circle(image1, cv::Point(face_marks1.at(i).mark[0].x, face_marks1.at(i).mark[0].y), 4,
	//				CV_RGB(255, 0, 0), 1);
	//			cv::circle(image1, cv::Point(face_marks1.at(i).mark[1].x, face_marks1.at(i).mark[1].y), 4,
	//				CV_RGB(0, 255, 0), 1);
	//			cv::circle(image1, cv::Point(face_marks1.at(i).mark[2].x, face_marks1.at(i).mark[2].y), 4,
	//				CV_RGB(0, 0, 255), 1);
	//			cv::circle(image1, cv::Point(face_marks1.at(i).mark[3].x, face_marks1.at(i).mark[3].y), 4,
	//				CV_RGB(255, 255, 0), 1);
	//			cv::circle(image1, cv::Point(face_marks1.at(i).mark[4].x, face_marks1.at(i).mark[4].y), 4,
	//				CV_RGB(255, 0, 255), 1);
	//			cv::circle(image1, cv::Point(face_marks1.at(i).mark[5].x, face_marks1.at(i).mark[5].y), 4,
	//				CV_RGB(0, 255, 255), 1);
	//	}
	//	Update(0);
	//	for (int i = 0; i < face_info2.size(); i++)
	//	{
	//		cv::Rect face_rect;
	//		face_rect.x = face_info2[i].bbox.x;
	//		face_rect.y = face_info2[i].bbox.y;
	//		face_rect.width = face_info2[i].bbox.width;
	//		face_rect.height = face_info2[i].bbox.height;
	//		cv::rectangle(image2, face_rect, CV_RGB(0, 255, 255), 1, 8, 0);
	//		for (int k = 0; k < 5; k++)
	//		{
	//			cv::circle(image2, cv::Point(face_marks2.at(i).mark[k].x, face_marks2.at(i).mark[k].y), 4,
	//				CV_RGB(0, 255, 255), 1);
	//		}
	//	}
	//	Update(1);

	//	//std::string similarStr = tools::f_to_s(similar);
	//	MessageBoxA(0, ("Similaraty=" + similarStr).c_str(), "Similaraty.", MB_OK);
	//	//cv::waitKey(0);
	//}
}

