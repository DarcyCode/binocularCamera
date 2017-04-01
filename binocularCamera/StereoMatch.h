/********************************************************************
	��������:	2012/10/28
	�ļ�����: .\StereoVision\StereoVision\StereoMatch.h
	����:	StereoMatch
	����:	���
	
	����:	˫Ŀƥ������ά�ؽ���
*********************************************************************/
#ifndef _STEREO_MATCH_H_
#define _STEREO_MATCH_H_

#pragma once

#include <vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;

class StereoMatch
{
public:
	StereoMatch(void);
	virtual ~StereoMatch(void);

	/*----------------------------
	 * ���� : ��ʼ���ڲ�����������˫Ŀ����������
	 *----------------------------
	 * ���� : StereoMatch::init
	 * ���� : public 
	 * ���� : 1		�ɹ�
	 *		 0		����У������ʧ��
	 *		 -1		���������ͼ��ߴ��뵱ǰ���õ�ͼ��ߴ粻һ��
	 *		 -2		У���������� BOUGUET ����
	 *		 -99	δ֪����
	 *
	 * ���� : imgWidth		[in]	ͼ����
	 * ���� : imgHeight		[in]	ͼ��߶�
	 * ���� : xmlFilePath	[in]	˫Ŀ�����������ļ�
	 */
	int init(int imgWidth, int imgHeight, const char* xmlFilePath);

	/*----------------------------
	 * ���� : ���� BM �㷨�����Ӳ�
	 *----------------------------
	 * ���� : StereoMatch::bmMatch
	 * ���� : public 
	 * ���� : 0 - ʧ�ܣ�1 - �ɹ�
	 *
	 * ���� : frameLeft		[in]	�������֡ͼ
	 * ���� : frameRight		[in]	�������֡ͼ
	 * ���� : disparity		[out]	�Ӳ�ͼ
	 * ���� : imageLeft		[out]	����������ͼ��������ʾ
	 * ���� : imageRight		[out]	����������ͼ��������ʾ
	 */
	int bmMatch(cv::Mat& frameLeft, cv::Mat& frameRight, cv::Mat& disparity, cv::Mat& imageLeft, cv::Mat& imageRight);

	/*----------------------------
	 * ���� : ���� SGBM �㷨�����Ӳ�
	 *----------------------------
	 * ���� : StereoMatch::sgbmMatch
	 * ���� : public 
	 * ���� : 0 - ʧ�ܣ�1 - �ɹ�
	 *
	 * ���� : frameLeft		[in]	�������֡ͼ
	 * ���� : frameRight		[in]	�������֡ͼ
	 * ���� : disparity		[out]	�Ӳ�ͼ
	 * ���� : imageLeft		[out]	����������ͼ��������ʾ
	 * ���� : imageRight		[out]	����������ͼ��������ʾ
	 */
	int sgbmMatch(cv::Mat& frameLeft, cv::Mat& frameRight, cv::Mat& disparity, cv::Mat& imageLeft, cv::Mat& imageRight);

	/*----------------------------
	 * ���� : ������ά����
	 *----------------------------
	 * ���� : StereoMatch::getPointClouds
	 * ���� : public 
	 * ���� : 0 - ʧ�ܣ�1 - �ɹ�
	 *
	 * ���� : disparity		[in]	�Ӳ�����
	 * ���� : pointClouds	[out]	��ά����
	 */
	int getPointClouds(cv::Mat& disparity, cv::Mat& pointClouds);

	/*----------------------------
	 * ���� : ��ȡα��ɫ�Ӳ�ͼ
	 *----------------------------
	 * ���� : StereoMatch::getDisparityImage
	 * ���� : public 
	 * ���� : 0 - ʧ�ܣ�1 - �ɹ�
	 *
	 * ���� : disparity		[in]	ԭʼ�Ӳ�����
	 * ���� : disparityImage	[out]	�Ӳ�ͼ��
	 * ���� : isColor		[in]	�Ƿ����α��ɫ��Ĭ��Ϊ true����Ϊ false ʱ���ػҶ��Ӳ�ͼ
	 */
	int getDisparityImage(cv::Mat& disparity, cv::Mat& disparityImage, bool isColor = true);

	/*----------------------------
	 * ���� : ������ά���Ƶ����� txt �ļ�
	 *----------------------------
	 * ���� : StereoMatch::savePointClouds
	 * ���� : public 
	 * ���� : void
	 *
	 * ���� : pointClouds	[in]	��ά��������
	 * ���� : filename		[in]	�ļ�·��
	 */
	void savePointClouds(cv::Mat& pointClouds, const char* filename);
    
	/*----------------------------
	 * ���� : �����ӳ���Χ
	 *----------------------------
	 * ���� : StereoMatch::setViewField
	 * ���� : public 
	 * ���� : void
     *
     * ���� : viewWidth   [in]	�ӳ����
     * ���� : viewHeight  [in]	�ӳ��߶�
     * ���� : viewDepth   [in]	�ӳ����
	 */
    void setViewField(int viewWidth, int viewHeight, int viewDepth)
    {
        m_nViewWidth = viewWidth;
        m_nViewHeight = viewHeight;
        m_nViewDepth = viewDepth;
    }

	/*----------------------------
	 * ���� : ��ȡ��������ͼ
	 *----------------------------
	 * ���� : StereoMatch::savePointClouds
	 * ���� : public 
	 * ���� : void
     *
     * ���� : pointClouds	[in]	��ά��������
     * ���� : topDownView	[out]	��������ͼ
     * ���� : image       	[in]	����ͼ��
	 */
    void getTopDownView(cv::Mat& pointClouds, cv::Mat& topDownView, cv::Mat& image = cv::Mat());
    
	/*----------------------------
	 * ���� : ��ȡ��������ͼ
	 *----------------------------
	 * ���� : StereoMatch::savePointClouds
	 * ���� : public 
	 * ���� : void
     *
     * ���� : pointClouds	[in]	��ά��������
     * ���� : sideView    	[out]	��������ͼ
     * ���� : image       	[in]	����ͼ��
	 */
    void getSideView(cv::Mat& pointClouds, cv::Mat& sideView, cv::Mat& image = cv::Mat());

	/***
	 *	��������
	 */
	cv::StereoBM	m_BM;				// ����ƥ�� BM ����
	cv::StereoSGBM	m_SGBM;				// ����ƥ�� SGBM ����
	double			m_FL;				// �������У����Ľ���ֵ

private:

	/*----------------------------
	 * ���� : ����˫Ŀ����������
	 *----------------------------
	 * ���� : StereoMatch::loadCalibData
	 * ���� : public 
	 * ���� : 1		�ɹ�
	 *		 0		����У������ʧ��
	 *		 -1		���������ͼ��ߴ��뵱ǰ���õ�ͼ��ߴ粻һ��
	 *		 -2		У���������� BOUGUET ����
	 *		 -99	δ֪����
	 * 
	 * ���� : xmlFilePath	[in]	˫Ŀ�����������ļ�
	 */
	int loadCalibData(const char* xmlFilePath);

	/***
	 *	˽�б���
	 */
	bool	m_Calib_Data_Loaded;		// �Ƿ�ɹ����붨�����
	cv::Mat m_Calib_Mat_Q;				// Q ����
	cv::Mat m_Calib_Mat_Remap_X_L;		// ����ͼ����У����������ӳ����� X
	cv::Mat m_Calib_Mat_Remap_Y_L;		// ����ͼ����У����������ӳ����� Y
	cv::Mat m_Calib_Mat_Remap_X_R;		// ����ͼ����У����������ӳ����� X
	cv::Mat m_Calib_Mat_Remap_Y_R;		// ����ͼ����У����������ӳ����� Y
	cv::Mat m_Calib_Mat_Mask_Roi;		// ����ͼУ�������Ч����
	cv::Rect m_Calib_Roi_L;				// ����ͼУ�������Ч�������
	cv::Rect m_Calib_Roi_R;				// ����ͼУ�������Ч�������

	int m_frameWidth;					// ֡��
	int m_frameHeight;					// ֡��
	int m_numberOfDisparies;			// �Ӳ�仯��Χ

    int m_nViewWidth;                   // �ӳ����
    int m_nViewHeight;                  // �ӳ��߶�
    int m_nViewDepth;                   // �ӳ����

};

#endif