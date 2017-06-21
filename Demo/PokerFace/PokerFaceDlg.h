
// PokerFaceDlg.h : 头文件
//

#pragma once
#include <string>
#include <vector>
#include <cv.hpp>
#include "stasm_lib.h"

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
	std::string appPath;
	std::string imgPathStr1, imgPathStr2;
	int imgIndex1, imgIndex2;
	std::vector<std::string> imgFileStr1, imgFileStr2;
	std::vector<faceInfoEx> imgfaceInfo1, imgfaceInfo2;
	std::vector<faceLandMark77> imgLandMarks1, imgLandMarks2;
	cv::Mat image1, image2;
	std::string imgWndStr1, imgWndStr2;


	void Update(int leftOrRight =-1);

	afx_msg void OnBnClickedImgBtn1();
	void LoadImg1(int index1=0);
	afx_msg void OnBnClickedImgupBtn1();
	afx_msg void OnBnClickedImgdownBtn1();
	afx_msg void OnBnClickedImgBtn2();
	void LoadImg2(int index2 = 0);
	afx_msg void OnBnClickedImgupBtn2();
	afx_msg void OnBnClickedImgdownBtn2();
	afx_msg void OnBnClickedCopBtn();
};
