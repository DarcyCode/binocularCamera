
// binocularCameraDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "CameraDS.h"
#include "CvvImage.h"

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

	// ���ܺ���
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
