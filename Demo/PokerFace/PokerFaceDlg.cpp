
// PokerFaceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PokerFace.h"
#include "PokerFaceDlg.h"
#include "afxdialogex.h"

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
	ON_BN_CLICKED(IDC_PATH_BTN, &CPokerFaceDlg::OnBnClickedPathBtn)
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString StrFile;
	CString defExe("txt");
	CString defFileName("*.txt");
	CString defFilter("�ı��ĵ�(*.txt)|*.txt|�����ļ�(*.*)|*.*|");
	CFileDialog dlg(true, defExe, defFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, defFilter);
	dlg.m_ofn.lpstrInitialDir = _T(".\\");
	if (dlg.DoModal() == IDOK)
	{
		StrFile = dlg.GetFolderPath();
		// do something

	}
}
