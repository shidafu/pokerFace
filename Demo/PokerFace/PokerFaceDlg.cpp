
// PokerFaceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PokerFace.h"
#include "PokerFaceDlg.h"
#include "afxdialogex.h"
#include "mformat.hpp"
#include "common_tools.hpp"
#include "face_tools.hpp"
#include "mformat.hpp"
#include "image_tools.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPokerFaceDlg �Ի���

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
END_MESSAGE_MAP()


// CPokerFaceDlg ��Ϣ�������

BOOL CPokerFaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
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



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPokerFaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPokerFaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPokerFaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPokerFaceDlg::Update(int index)
{
	if (index==-1)
	{
		if (!image1.empty())
		{
			CRect rect1;
			GetDlgItem(IDC_IMG_STC_1)->GetClientRect(&rect1);
			//cv::Rect dstRect1(rect1.left, rect1.top, rect1.right, rect1.bottom);
			//cv::Size srcSize1(image1.cols, image1.rows);
			cv::Mat imagedst1;
			cv::resize(image1, imagedst1, cv::Size(rect1.Width(), rect1.Height()));
			cv::imshow(imgWndStr1, imagedst1);
		}
		if (!image2.empty())
		{
			CRect rect2;
			GetDlgItem(IDC_IMG_STC_2)->GetClientRect(&rect2);
			cv::Mat imagedst2;
			cv::resize(image2, imagedst2, cv::Size(rect2.Width(), rect2.Height()));
			cv::imshow(imgWndStr2, imagedst2);
		}
	} 
	else if (index==0)
	{
		if (!image1.empty())
		{
			CRect rect1;
			GetDlgItem(IDC_IMG_STC_1)->GetClientRect(&rect1);
			//cv::Rect dstRect1(rect1.left, rect1.top, rect1.right, rect1.bottom);
			//cv::Size srcSize1(image1.cols, image1.rows);
			cv::Mat imagedst1;
			cv::resize(image1, imagedst1, cv::Size(rect1.Width(), rect1.Height()));
			cv::imshow(imgWndStr1, imagedst1);
		}
	}
	else if (index==1)
	{
		if (!image2.empty())
		{
			CRect rect2;
			GetDlgItem(IDC_IMG_STC_2)->GetClientRect(&rect2);
			cv::Mat imagedst2;
			cv::resize(image2, imagedst2, cv::Size(rect2.Width(), rect2.Height()));
			cv::imshow(imgWndStr2, imagedst2);
		}
	}
}


void CPokerFaceDlg::OnBnClickedImgBtn1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString StrFile;
	CString defExe("jpg");
	CString defFileName("*.jpg");
	CString defFilter("ͼ��(*.jpg)|*.jpg|�����ļ�(*.*)|*.*|");
	CFileDialog dlg(true, defExe, defFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, defFilter);
	dlg.m_ofn.lpstrInitialDir = _T(".\\");
	if (dlg.DoModal() == IDOK)
	{
		StrFile = dlg.GetPathName();
		// do something
		imgFileStr1 = tools::ws_to_s(StrFile.GetString());
		cv::Mat tmpImg = cv::imread(imgFileStr1);
		tools::corp_size(tmpImg, image1, tools::accept_size_4X3);
		Update(0);
	}
}


void CPokerFaceDlg::OnBnClickedImgBtn2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString StrFile;
	CString defExe("jpg");
	CString defFileName("*.jpg");
	CString defFilter("ͼ��(*.jpg)|*.jpg|�����ļ�(*.*)|*.*|");
	CFileDialog dlg(true, defExe, defFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, defFilter);
	dlg.m_ofn.lpstrInitialDir = _T(".\\");
	if (dlg.DoModal() == IDOK)
	{
		StrFile = dlg.GetPathName();
		// do something
		imgFileStr2 = tools::ws_to_s(StrFile.GetString());
		cv::Mat tmpImg = cv::imread(imgFileStr2);
		tools::corp_size(tmpImg, image2, tools::accept_size_4X3);
		Update(1);
	}
}


void CPokerFaceDlg::OnBnClickedCopBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//Load faceTool.
	std::string exe_path_str = tools::app_path();
	tools::faceDetector face_detector;
	face_detector.initial(tools::accept_size_3X2,
		(exe_path_str + "\\seeta_fd_frontal_v1.0.bin").c_str(),
		(exe_path_str + "\\seeta_fa_v1.1.bin").c_str(),
		(exe_path_str + "\\seeta_fr_v1.0.bin").c_str());

	int imageNum = 0;
	if (!image1.empty()) imageNum++;
	if (!image2.empty()) imageNum++;

	//Deal different call.
	if (imageNum == 0) return;
	else if (imageNum == 1) return;
	else if (imageNum == 2)
	{
		//Load images
		seeta::ImageData img_data1(image1.cols, image1.rows, image1.channels());
		img_data1.data = image1.data;
		cv::Mat gray_img1;
		cv::cvtColor(image1, gray_img1, CV_RGB2GRAY);
		seeta::ImageData gray_img_data1(gray_img1.cols, gray_img1.rows, gray_img1.channels());
		gray_img_data1.data = gray_img1.data;

		seeta::ImageData img_data2(image2.cols, image2.rows, image2.channels());
		img_data2.data = image2.data;
		cv::Mat gray_img2;
		cv::cvtColor(image2, gray_img2, CV_RGB2GRAY);
		seeta::ImageData gray_img_data2(gray_img2.cols, gray_img2.rows, gray_img2.channels());
		gray_img_data2.data = gray_img2.data;

		// Detect face.
		std::vector <seeta::FaceInfo> face_info1;
		std::vector <tools::faceLandMark> face_marks1;
		long t0 = cv::getTickCount();
		bool isOK = face_detector.detect(gray_img_data1, face_info1, face_marks1);
		long t1 = cv::getTickCount();
		double secs = (t1 - t0) / cv::getTickFrequency();
		std::string debugStr = tools::f_to_s(secs);
		OutputDebugStringA(("Detect take time = " + debugStr + "\n").c_str());

		std::vector <seeta::FaceInfo> face_info2;
		std::vector <tools::faceLandMark> face_marks2;
		t0 = cv::getTickCount();
		isOK = face_detector.detect(gray_img_data2, face_info2, face_marks2);
		t1 = cv::getTickCount();
		secs = (t1 - t0) / cv::getTickFrequency();
		debugStr = tools::f_to_s(secs);
		OutputDebugStringA(("Detect take time = " + debugStr + "\n").c_str());

		float similar=0;
		std::string similarStr;
		if (face_marks1.size() > 0 && face_marks2.size() > 0)
		{
			//int face_rows = 0;
			//int face_cols = 0;
			//int face_chn = 0;
			//face_detector.precorp(face_cols, face_rows, face_chn);

			//cv::Mat corp_img1=cv::Mat(face_rows,face_cols,CV_8UC(face_chn));
			//seeta::ImageData corp_img_data1(corp_img1.cols, corp_img1.rows, corp_img1.channels());
			//corp_img_data1.data = corp_img1.data;
			//t0 = cv::getTickCount();
			//face_detector.corp(img_data1, face_marks1.at(0), corp_img_data1);
			//t1 = cv::getTickCount();
			//secs = (t1 - t0) / cv::getTickFrequency();
			//debugStr = tools::f_to_s(secs);
			//OutputDebugStringA(("Crop take time = " + debugStr + "\n").c_str());

			//cv::Mat corp_img2 = cv::Mat(face_rows, face_cols, CV_8UC(face_chn));
			//seeta::ImageData corp_img_data2(corp_img2.cols, corp_img2.rows, corp_img2.channels());
			//corp_img_data2.data = corp_img2.data;
			//t0 = cv::getTickCount();
			//face_detector.corp(img_data2, face_marks2.at(0), corp_img_data2);
			//t1 = cv::getTickCount();
			//secs = (t1 - t0) / cv::getTickFrequency();
			//debugStr = tools::f_to_s(secs);
			//OutputDebugStringA(("Crop take time = " + debugStr + "\n").c_str());

			t0 = cv::getTickCount();
			similar = face_detector.corp_compare(img_data1, img_data2, face_marks1.at(0).mark, face_marks2.at(0).mark);
			t1 = cv::getTickCount();
			secs = (t1 - t0) / cv::getTickFrequency();
			similar -= 0.40;
			similar /= 0.1;
			similar = 1 / (1 + exp(-similar));
			debugStr = tools::f_to_s(secs);
			similarStr = tools::f_to_s(similar);
			OutputDebugStringA(("Corp Compare take time = " + debugStr + ",similar=" + similarStr +"\n").c_str());
			//MessageBoxA(0, ("Similaraty=" + similarStr).c_str(), "Similaraty", MB_OK);
		}

		// Show detected face
		for (int i = 0; i < face_info1.size(); i++)
		{
			cv::Rect face_rect;
			face_rect.x = face_info1[i].bbox.x;
			face_rect.y = face_info1[i].bbox.y;
			face_rect.width = face_info1[i].bbox.width;
			face_rect.height = face_info1[i].bbox.height;
			cv::rectangle(image1, face_rect, CV_RGB(0, 255, 255), 1, 8, 0);
			//for (int k = 0; k < 5; k++)
			//{
			//	cv::circle(image1, cv::Point(face_marks1.at(i).mark[k].x, face_marks1.at(i).mark[k].y), 4,
			//		CV_RGB(0, 255, 255), 1);
			//}
				cv::circle(image1, cv::Point(face_marks1.at(i).mark[0].x, face_marks1.at(i).mark[0].y), 4,
					CV_RGB(255, 0, 0), 1);
				cv::circle(image1, cv::Point(face_marks1.at(i).mark[1].x, face_marks1.at(i).mark[1].y), 4,
					CV_RGB(0, 255, 0), 1);
				cv::circle(image1, cv::Point(face_marks1.at(i).mark[2].x, face_marks1.at(i).mark[2].y), 4,
					CV_RGB(0, 0, 255), 1);
				cv::circle(image1, cv::Point(face_marks1.at(i).mark[3].x, face_marks1.at(i).mark[3].y), 4,
					CV_RGB(255, 255, 0), 1);
				cv::circle(image1, cv::Point(face_marks1.at(i).mark[4].x, face_marks1.at(i).mark[4].y), 4,
					CV_RGB(255, 0, 255), 1);
				cv::circle(image1, cv::Point(face_marks1.at(i).mark[5].x, face_marks1.at(i).mark[5].y), 4,
					CV_RGB(0, 255, 255), 1);
		}
		Update(0);
		for (int i = 0; i < face_info2.size(); i++)
		{
			cv::Rect face_rect;
			face_rect.x = face_info2[i].bbox.x;
			face_rect.y = face_info2[i].bbox.y;
			face_rect.width = face_info2[i].bbox.width;
			face_rect.height = face_info2[i].bbox.height;
			cv::rectangle(image2, face_rect, CV_RGB(0, 255, 255), 1, 8, 0);
			for (int k = 0; k < 5; k++)
			{
				cv::circle(image2, cv::Point(face_marks2.at(i).mark[k].x, face_marks2.at(i).mark[k].y), 4,
					CV_RGB(0, 255, 255), 1);
			}
		}
		Update(1);

		//std::string similarStr = tools::f_to_s(similar);
		MessageBoxA(0, ("Similaraty=" + similarStr).c_str(), "Similaraty.", MB_OK);
		//cv::waitKey(0);
	}
}
