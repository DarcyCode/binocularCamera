
// binocularCameraDlg.h : ͷ�ļ�
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
// CbinocularCameraDlg �Ի���
class CbinocularCameraDlg : public CDialogEx
{
// ����
public:
	CbinocularCameraDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BINOCULARCAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	VideoCapture lfCam;  // �����
	VideoCapture riCam;  // �����
	cv::Mat m_lfImage;   // �����ͼ��
	cv::Mat m_riImage;   // �����ͼ��
	cv::Mat m_disparity;
	int m_lfCamID;       // �����ID
	int m_riCamID;       // �����ID
	int m_nCamCount;     // ����ܸ���
	int m_nImageWidth;   // ͼ����
	int m_nImageHeight;  // ͼ��߶�
	int m_nImageChannels;// ͼ��ͨ����

	// �ؼ�����	
	CComboBox m_ComboBoxCameraR;  // ������б�	
	CComboBox m_ComboBoxCameraL;  // ������б�
	CComboBox m_CBNResolution;    // �ֱ���

	typedef enum { STEREO_BM, STEREO_SGBM } STEREO_METHOD;
	typedef enum { CALIB_LOAD_CAMERA_PARAMS, CALIB_SINGLE_CAMERA_FIRST, CALIB_STEREO_CAMERAS_DIRECTLY } CALIB_ORDER;
	struct OptionStereoMatch
	{
		bool			readLocalImage;		//�Ƿ���뱾��ͼ���������ƥ��
		bool			generatePointCloud;	//�Ƿ�������ά����
		bool			useStereoRectify;	//�Ƿ�ʹ��˫ĿУ���㷨
		bool			saveResults;		//�Ƿ񱣴�ÿ֡ͼ��ƥ�����������ļ�
		bool			delayEachFrame;		//�Ƿ���ʱ��ʾƥ��Ч��
		STEREO_METHOD	stereoMethod;		//ѡ�������ƥ���㷨
		StereoCalib::RECTIFYMETHOD	rectifyMethod;		//ѡ���˫ĿУ���㷨
	};
	struct OptionCameraCalib
	{
		int				numberBoards;		//���̼�����
		int				flagCameraCalib;	//��Ŀ�����־��
		int				flagStereoCalib;	//˫Ŀ�����־��
		int				numberFrameSkip;	//�ǵ����֡������
		bool			doStereoCalib;		//�Ƿ����˫Ŀ�궨
		bool			readLocalImage;		//�Ƿ�ӱ��ض�������ͼƬ
		bool			loadConerDatas;		//�Ƿ�ӱ��ض���ǵ���������
		double			squareSize;			//���̷����С
		cv::Size		cornerSize;			//���̽ǵ���
		CALIB_ORDER		calibOrder;			//������������
		double          alpha;              //˫ĿУ������ϵ��
		StereoCalib::RECTIFYMETHOD	rectifyMethod;		//ѡ���˫ĿУ���㷨
	};
public:
	afx_msg void OnBnClickedBn1runcam();        // �������ť��Ӧ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBn2stopcam();       // �ر������ť��Ӧ
	afx_msg void OnClose();
	afx_msg void OnBnClickedBn2stereocalib();   // �궨��ť��Ӧ
	afx_msg void OnBnClickedBnExitcameracalib();// �˳��궨��Ӧ
	afx_msg void OnCbnSelchangeComboCameral();  // �������ѡ����Ӧ
	afx_msg void OnCbnSelchangeComboCamerar();  // �������ѡ����Ӧ
	afx_msg void OnCbnSelchangeCbnResolution(); // �ֱ��ʸ�ѡ����Ӧ
private:
	int m_nCornerSize_X;			// ���̽ǵ�������
	int m_nCornerSize_Y;			// ���̽ǵ��������
	float m_nSquareSize;			// ���̳���
	double m_ObjectWidth;
	double m_ObjectHeight;
	double m_ObjectDistance;
	double m_ObjectDisparity;
	int m_nBoards;					// ������
	
	UINT m_nID_RAD;					// �궨ѡȡ��ʽ
	double m_dAlpha;				// У��ϵ��
	StereoCalib m_stereoCalibrator;
	StereoMatch m_stereoMatcher;
	CString m_workDir;

	CButton* m_pCheck;

	// ���ܺ���
	CString F_InitWorkDir();
	bool F_CheckDir(const string dir, bool creatDir = false);
	void DoShowOrigFrame(void);    
	void F_ShowImage(Mat& src, Mat& des, UINT ID);  // ��ʾ����
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
	// ��С�Ӳ�
	int m_nMinDisp;
	// ����Ӳ�
	int m_nMaxDisp;
	// ˫Ŀƥ�䴰�ڴ�С
	int m_nSADWinSiz;
	int m_nTextThres;
	// �����Ӳ���ֵ
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
