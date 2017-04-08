
// binocularCameraDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "CameraDS.h"
#include "CvvImage.h"
#include "StereoCalib.h"
#include "StereoMatch.h"
#include "PointCloudAnalyzer.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "afxcmn.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <io.h>
#include <direct.h>

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
	VideoCapture lfCam;  // 左相机
	VideoCapture riCam;  // 右相机
	cv::Mat m_lfImage;   // 左相机图像
	cv::Mat m_riImage;   // 右相机图像
	cv::Mat m_disparity;
	int m_lfCamID;       // 左相机ID
	int m_riCamID;       // 右相机ID
	int m_nCamCount;     // 相机总个数
	int m_nImageWidth;   // 图像宽度
	int m_nImageHeight;  // 图像高度
	int m_nImageChannels;// 图像通道数

	// 控件变量	
	CComboBox m_ComboBoxCameraR;  // 右相机列表	
	CComboBox m_ComboBoxCameraL;  // 左相机列表
	CComboBox m_CBNResolution;    // 分辨率

	typedef enum { STEREO_BM, STEREO_SGBM } STEREO_METHOD;
	typedef enum { CALIB_LOAD_CAMERA_PARAMS, CALIB_SINGLE_CAMERA_FIRST, CALIB_STEREO_CAMERAS_DIRECTLY } CALIB_ORDER;
	struct OptionStereoMatch
	{
		bool			readLocalImage;		//是否读入本地图像进行立体匹配
		bool			generatePointCloud;	//是否生成三维点云
		bool			useStereoRectify;	//是否使用双目校正算法
		bool			saveResults;		//是否保存每帧图像匹配结果到本地文件
		bool			delayEachFrame;		//是否延时显示匹配效果
		STEREO_METHOD	stereoMethod;		//选择的立体匹配算法
		StereoCalib::RECTIFYMETHOD	rectifyMethod;		//选择的双目校正算法
	};
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
	afx_msg void OnBnClickedBn1runcam();        // 打开相机按钮响应
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBn2stopcam();       // 关闭相机按钮响应
	afx_msg void OnClose();
	afx_msg void OnBnClickedBn2stereocalib();   // 标定按钮响应
	afx_msg void OnBnClickedBnExitcameracalib();// 退出标定响应
	afx_msg void OnCbnSelchangeComboCameral();  // 左相机复选框响应
	afx_msg void OnCbnSelchangeComboCamerar();  // 右相机复选框响应
	afx_msg void OnCbnSelchangeCbnResolution(); // 分辨率复选框响应
private:
	int m_nCornerSize_X;			// 棋盘角点横向个数
	int m_nCornerSize_Y;			// 棋盘角点纵向个数
	float m_nSquareSize;			// 棋盘长度
	double m_ObjectWidth;
	double m_ObjectHeight;
	double m_ObjectDistance;
	double m_ObjectDisparity;
	int m_nBoards;					// 检测次数
	
	UINT m_nID_RAD;					// 标定选取方式
	double m_dAlpha;				// 校正系数
	StereoCalib m_stereoCalibrator;
	StereoMatch m_stereoMatcher;
	CString m_workDir;

	CButton* m_pCheck;

	// 功能函数
	CString F_InitWorkDir();
	bool F_CheckDir(const string dir, bool creatDir = false);
	void DoShowOrigFrame(void);    
	void F_ShowImage(Mat& src, Mat& des, UINT ID);  // 显示函数
	void F_Saveframe(Mat& lfImg, Mat&riImg, Mat& lfDisp);
	bool DoParseOptionsOfCameraCalib(OptionCameraCalib& opt);
	bool DoParseOptionsOfStereoMatch(OptionStereoMatch& opt);
	void DoClearParamsOfStereoMatch(void);
	vector<CStringA> DoSelectFiles(LPCTSTR	lpszDefExt, DWORD	dwFlags, LPCTSTR	lpszFilter, LPCWSTR	lpstrTitle, LPCWSTR	lpstrInitialDir);
	void DoUpdateStateBM(void);
	void DoUpdateStateSGBM(int imgChannels);

public:
	afx_msg void OnBnClickedBnStereodefparam();
private:
	// 最小视差
	int m_nMinDisp;
	// 最大视差
	int m_nMaxDisp;
	// 双目匹配窗口大小
	int m_nSADWinSiz;
	int m_nTextThres;
	// 左右视差阈值
	int m_nDisp12MaxDiff;
	int m_nPreFiltCap;
	int m_nUniqRatio;
	int m_nSpeckRange;
	int m_nSpeckWinSiz;
	CSpinButtonCtrl m_spinMinDisp;
	CSpinButtonCtrl m_spinMaxDisp;
	CSpinButtonCtrl m_spinSADWinSiz;
	CSpinButtonCtrl m_spinTextThres;
	CSpinButtonCtrl m_spinDisp12MaxDiff;
	CSpinButtonCtrl m_spinPreFiltCap;
	CSpinButtonCtrl m_spinUniqRatio;
	CSpinButtonCtrl m_spinSpeckRange;
	CSpinButtonCtrl m_spinSpeckWinSiz;
	BOOL m_bFullDP;
	int m_nDelayTime;
	BOOL m_bSaveFrame;
public:
	afx_msg void OnBnClickedBnCompdisp();
	afx_msg void OnBnClickedBnStopdispcomp();
	afx_msg void OnBnClickedRadBm();
	afx_msg void OnBnClickedRadSgbm();
	afx_msg void OnBnClickedBtnDefaultviewfield();
private:
	int m_nViewWidth;
	int m_nViewHeight;
	int m_nViewDepth;
	CSpinButtonCtrl m_spinViewWidth;
	CSpinButtonCtrl m_spinViewHeight;
	CSpinButtonCtrl m_spinViewDepth;
public:
	afx_msg void OnDeltaposSpinmaxdisp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinSadwinsiz(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinspeckrange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinviewwidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinviewheight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinviewdepth(NMHDR *pNMHDR, LRESULT *pResult);
};
