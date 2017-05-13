
// PokerFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PokerFace.h"
#include "PokerFaceDlg.h"
#include "afxdialogex.h"

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
	ON_BN_CLICKED(IDC_PATH_BTN, &CPokerFaceDlg::OnBnClickedPathBtn)
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

void CPokerFaceDlg::Update()
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

void CPokerFaceDlg::OnBnClickedPathBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString StrFile;
	CString defExe("txt");
	CString defFileName("*.txt");
	CString defFilter("文本文档(*.txt)|*.txt|所有文件(*.*)|*.*|");
	CFileDialog dlg(true, defExe, defFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, defFilter);
	dlg.m_ofn.lpstrInitialDir = _T(".\\");
	if (dlg.DoModal() == IDOK)
	{
		StrFile = dlg.GetFolderPath();
		// do something

	}
}
