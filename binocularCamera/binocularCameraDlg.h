
// binocularCameraDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CameraDS.h"
#include "CvvImage.h"
#include "StereoCalib.h"
#include "StereoMatch.h"

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
	CComboBox m_CBNResolution;

	// 功能函数
	void DoShowOrigFrame(void);
	void F_ShowImage(Mat& src, Mat& des, UINT ID);
public:
	afx_msg void OnCbnSelchangeComboCameral();
	afx_msg void OnCbnSelchangeComboCamerar();
	afx_msg void OnCbnSelchangeCbnResolution();
private:
	typedef enum { CALIB_LOAD_CAMERA_PARAMS, CALIB_SINGLE_CAMERA_FIRST, CALIB_STEREO_CAMERAS_DIRECTLY } CALIB_ORDER;
	struct OptionCameraCalib
	{
		int				numberBoards;		//棋盘检测次数
		int				flagCameraCalib;	//单目定标标志符
		int				flagStereoCalib;	//双目定标标志符
		int				numberFrameSkip;	//角点检测的帧间间隔数
		bool			doStereoCalib;		//是否进行双目标定
		bool			readLocalImage;		//是否从本地读入棋盘图片
		bool			loadConerDatas;		//是否从本地读入角点坐标数据
		double			squareSize;			//棋盘方块大小
		cv::Size		cornerSize;			//棋盘角点数
		CALIB_ORDER		calibOrder;			//摄像机定标次序
		double          alpha;              //双目校正缩放系数
		StereoCalib::RECTIFYMETHOD	rectifyMethod;		//选择的双目校正算法
	};
public:
	afx_msg void OnBnClickedBn1runcam();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBn2stopcam();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBn2stereocalib();
private:
	int m_nCornerSize_X;
	int m_nCornerSize_Y;
	float m_nSquareSize;
	int m_nBoards;
	UINT m_nID_RAD;
	double m_dAlpha;
	StereoCalib m_stereoCalibrator;
	CString m_workDir;

	CButton* m_pCheck;
private:
	bool DoParseOptionsOfCameraCalib(OptionCameraCalib& opt);
	vector<CStringA> DoSelectFiles(LPCTSTR	lpszDefExt, DWORD	dwFlags, LPCTSTR	lpszFilter, LPCWSTR	lpstrTitle, LPCWSTR	lpstrInitialDir);
public:
	afx_msg void OnBnClickedBnExitcameracalib();
};
