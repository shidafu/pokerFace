
// PokerFaceDlg.h : ͷ�ļ�
//

#pragma once
#include <string>
#include <vector>
#include <cv.hpp>

// CPokerFaceDlg �Ի���
class CPokerFaceDlg : public CDialog
{
// ����
public:
	CPokerFaceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POKERFACE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	std::string imgFileStr1, imgFileStr2;
	cv::Mat image1, image2;
	std::string imgWndStr1, imgWndStr2;


	void Update(int index=-1);

	afx_msg void OnBnClickedImgBtn1();
	afx_msg void OnBnClickedImgBtn2();
	afx_msg void OnBnClickedCopBtn();
};
