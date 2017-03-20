
// binocularCameraDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CameraDS.h"
#include "CvvImage.h"

#include <opencv.hpp>

using namespace cv;
using namespace std;
// CbinocularCameraDlg 对话框
class CbinocularCameraDlg : public CDialogEx
{
// 构造
public:
	CbinocularCameraDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BINOCULARCAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	VideoCapture lfCam;
	VideoCapture riCam;
	cv::Mat m_lfImage;
	cv::Mat m_riImage;
	int m_lfCamID;
	int m_riCamID;
	int m_nCamCount;
	int m_nImageWidth;
	int m_nImageHeight;
	int m_nImageChannels;

	// 控件变量
	// 右相机列表
	CComboBox m_ComboBoxCameraR;
	// 左相机列表
	CComboBox m_ComboBoxCameraL;

	// 功能函数
	void DoShowOrigFrame(void);
	void F_ShowImage(Mat& src, Mat& des, UINT ID);
public:
	afx_msg void OnCbnSelchangeComboCameral();
	afx_msg void OnCbnSelchangeComboCamerar();
	afx_msg void OnCbnSelchangeCbnResolution();
private:
	CComboBox m_CBNResolution;
public:
	afx_msg void OnBnClickedBn1runcam();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBn2stopcam();
	afx_msg void OnClose();
};
