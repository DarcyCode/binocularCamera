
// binocularCameraDlg.h : ͷ�ļ�
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

	// �ؼ�����
	// ������б�
	CComboBox m_ComboBoxCameraR;
	// ������б�
	CComboBox m_ComboBoxCameraL;
	CComboBox m_CBNResolution;

	// ���ܺ���
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
