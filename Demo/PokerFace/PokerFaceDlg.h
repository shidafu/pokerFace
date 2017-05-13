
// PokerFaceDlg.h : 头文件
//

#pragma once
#include <string>
#include <vector>
#include <cv.hpp>

// CPokerFaceDlg 对话框
class CPokerFaceDlg : public CDialog
{
// 构造
public:
	CPokerFaceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POKERFACE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
