/********************************************************************
	���� :	2012/10/28
	�ļ� :	.\StereoVision\StereoVision\StereoCalib.h
	���� :	StereoCalib
	���� :	��� chenyusiyuan AT 126 DOT com
	
	���� :	������궨��У����ͷ�ļ�
*********************************************************************/

#ifndef _STEREO_CALIB_H_
#define _STEREO_CALIB_H_

#pragma once

#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d//calib3d.hpp"

using namespace std;

class StereoCalib
{
public:
	StereoCalib(void);
	~StereoCalib(void);

	/***
	 *	���̽ǵ����� �ṹ��
	 */
	struct CornerDatas
	{
		int			nPoints;			// ���̽ǵ�����
		int			nImages;			// ����ͼ����
		int			nPointsPerImage;	// ÿ�����̵Ľǵ���
		cv::Size	imageSize;			// ͼ��ֱ���
		cv::Size	boardSize;			// ���̳ߴ�
		vector<vector<cv::Point3f> >	objectPoints;	// ���̽ǵ�������������
		vector<vector<cv::Point2f> >	imagePoints1;	// ����ͼ�����̽ǵ�������������
		vector<vector<cv::Point2f> >	imagePoints2;	// ����ͼ�����̽ǵ�������������
	};

	/***
	 *	��Ŀ�궨���������
	 */
	struct CameraParams
	{
		cv::Size		imageSize;				// ͼ��ֱ���
		cv::Mat			cameraMatrix;			// ���������
		cv::Mat			distortionCoefficients;	// ������������
		vector<cv::Mat> rotations;				// ����ͼƬ����ת����
		vector<cv::Mat> translations;			// ����ͼƬ��ƽ������
		int				flags;					// ��Ŀ�궨���õı�־λ
	};

	/***
	 *	˫Ŀ�궨���������
	 */
	struct StereoParams
	{
		cv::Size		imageSize;		// ͼ��ֱ���
		CameraParams	cameraParams1;	// ��������궨����
		CameraParams	cameraParams2;	// ��������궨����
		cv::Mat			rotation;		// ��ת����
		cv::Mat			translation;	// ƽ������
		cv::Mat			essential;		// ���ʾ���
		cv::Mat			foundational;	// ��������
		int				flags;			// ˫Ŀ�궨���õı�־λ
        double          alpha;          // ˫ĿУ��Ч��������ϵ����ȡֵ 0~1 �� -1
	};

	/***
	 *	˫ĿУ�����������
	 */
	struct RemapMatrixs
	{
		cv::Mat		mX1;	// ����ͼ X ��������ӳ�����
		cv::Mat		mY1;	// ����ͼ Y ��������ӳ�����
		cv::Mat		mX2;	// ����ͼ X ��������ӳ�����
		cv::Mat		mY2;	// ����ͼ Y ��������ӳ�����
		cv::Mat		Q;		// ���ڼ�����ά���Ƶ� Q ����
		cv::Rect	roi1;	// ����ͼ��Ч����ľ���
		cv::Rect	roi2;	// ����ͼ��Ч����ľ���
	};

	/***
	 *	˫ĿУ������
	 */
	enum RECTIFYMETHOD { RECTIFY_BOUGUET, RECTIFY_HARTLEY };

    void setWorkDir(const char* workDir) { m_workDir = workDir; }

	/*----------------------------
	 * ���� : ��ʼ�����̽ǵ�������Ϣ
	 *----------------------------
	 * ���� : StereoCalib::initCornerData
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : nImages		[in]	����ͼ����
	 * ���� : imageSize		[in]	ͼ��ֱ���
	 * ���� : boardSize		[in]	���̽ǵ���
	 * ���� : squareWidth	[in]	���̷�����
	 * ���� : cornerDatas	[out]	��ʼ��������̽ǵ�����
	 */
	int initCornerData(int nImages, cv::Size imageSize, cv::Size boardSize, float squareWidth, CornerDatas& cornerDatas);

	/*----------------------------
	 * ���� : ���ݳɹ�����������Ŀ�޸����̽ǵ�����
	 *----------------------------
	 * ���� : StereoCalib::resizeCornerData
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : nImages		[in]	����ͼ����
	 * ���� : cornerDatas	[out]	�޸ĺ�����̽ǵ�����
	 */
	int resizeCornerData(int nImages, CornerDatas& cornerDatas);

	/*----------------------------
	 * ���� : �������̽ǵ�������Ϣ
	 *----------------------------
	 * ���� : StereoCalib::loadCornerData
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : filename		[in]	�����ļ�·��/�ļ���
	 * ���� : cornerDatas	[out]	��������̽ǵ�����
	 */
	int loadCornerData(const char* filename, CornerDatas& cornerDatas);
	
	/*----------------------------
	 * ���� : �������̽ǵ�������Ϣ
	 *----------------------------
	 * ���� : StereoCalib::saveCornerData
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : filename		[in]	�����ļ�·��/�ļ���
	 * ���� : cornerDatas	[in]	�����������̽ǵ�����
	 */
	int saveCornerData(const char* filename, const CornerDatas& cornerDatas);

	/*----------------------------
	 * ���� : ������̽ǵ�
	 *----------------------------
	 * ���� : StereoCalib::detectCorners
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : img1			[in]	����ͼ
	 * ���� : img2			[in]	����ͼ
	 * ���� : cornerDatas	[i/o]	�����������̽ǵ�����
	 * ���� : imageCount		[in]	��ǰ���̽ǵ���ĳɹ�����
	 */
	int detectCorners(cv::Mat& img1, cv::Mat& img2, CornerDatas& cornerDatas, int imageCount);

	/*----------------------------
	 * ���� : �����ѱ궨�õ�������ڲ�����
	 *----------------------------
	 * ���� : StereoCalib::loadCameraParams
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : filename		[in]	�����ļ�·��/�ļ���
	 * ���� : cameraParams	[out]	��������������
	 */
	int loadCameraParams(const char* filename, CameraParams& cameraParams);

	/*----------------------------
	 * ���� : �����ѱ궨�õ�������ڲ�����
	 *----------------------------
	 * ���� : StereoCalib::saveCameraParams
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : cameraParams	[in]	�ѱ궨�����������
	 * ���� : filename		[in]	�����ļ�·��/�ļ���
	 */
	int saveCameraParams(const CameraParams& cameraParams, const char* filename = "cameraParams.yml");

	/*----------------------------
	 * ���� : ִ�е�Ŀ������궨
	 *----------------------------
	 * ���� : StereoCalib::calibrateSingleCamera
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : cornerDatas			[in]	���̽ǵ�����
	 * ���� : cameraParams			[out]	˫Ŀ�궨����
	 */
	int calibrateSingleCamera(CornerDatas& cornerDatas, CameraParams& cameraParams);

	/*----------------------------
	 * ���� : ִ��˫Ŀ������궨
	 *		 ��ÿ���������δ�궨�������Ƚ��е�Ŀ�궨���ٽ���˫Ŀ�궨
	 *----------------------------
	 * ���� : StereoCalib::calibrateStereoCamera
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : cornerDatas			[in]	���̽ǵ�����
	 * ���� : stereoParams			[i/o]	˫Ŀ�궨����
	 * ���� : cameraUncalibrated		[in]	ÿ��������Ƿ��Ѿ������궨
	 */
	int calibrateStereoCamera(CornerDatas& cornerDatas, StereoParams& stereoParams, bool cameraUncalibrated = false);

	/*----------------------------
	 * ���� : ���㵥Ŀ�궨���
	 *----------------------------
	 * ���� : StereoCalib::getCameraCalibrateError
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : _objectPoints	[in]	���̽ǵ����������
	 * ���� : _imagePoints	[in]	���̽ǵ��ͼ������
	 * ���� : cameraParams	[in]	�궨�����������
	 * ���� : err			[out]	��Ŀ�궨���
	 */
	int getCameraCalibrateError(vector<vector<cv::Point3f> >& objectPoints, vector<vector<cv::Point2f> >& imagePoints, CameraParams& cameraParams, double& err);

	/*----------------------------
	 * ���� : ����˫Ŀ�궨���
	 *----------------------------
	 * ���� : StereoCalib::getStereoCalibrateError
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : cornerDatas	[in]	���̽ǵ�����
	 * ���� : stereoParams	[in]	˫Ŀ�궨����
	 * ���� : err			[out]	˫Ŀ�궨���
	 */
	int getStereoCalibrateError(CornerDatas& cornerDatas, StereoParams& sterepParams, double& err);

	/*----------------------------------
	 * ���� : ���ɵ�������ͷ��У������
	 *----------------------------------
	 * ���� : StereoCalib::rectifySingleCamera
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : cameraParams	[in]	�궨�����������
	 * ���� : remapMatrixs	[out]	��ĿУ�����
	 */
	int rectifySingleCamera(CameraParams& cameraParams, RemapMatrixs& remapMatrixs);

	/*----------------------------
	 * ���� : ִ��˫Ŀ�����У��
	 *----------------------------
	 * ���� : StereoCalib::rectifyStereoCamera
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : cornerDatas	[in]	���̽ǵ�����
	 * ���� : stereoParams	[in]	˫Ŀ�궨����
	 * ���� : remapMatrixs	[out]	˫ĿУ���������
	 * ���� : method			[in]	˫ĿУ������
	 */
	int rectifyStereoCamera(CornerDatas& cornerDatas, StereoParams& stereoParams, RemapMatrixs& remapMatrixs, RECTIFYMETHOD method);

	/*----------------------------
	 * ���� : ����˫ĿУ������
	 *----------------------------
	 * ���� : StereoCalib::saveCalibrationDatas
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : filename		[in]	����·��/�ļ���
	 * ���� : method			[in]	˫ĿУ������
	 * ���� : cornerDatas	[in]	���̽ǵ�����
	 * ���� : stereoParams	[in]	˫Ŀ�궨���
	 * ���� : remapMatrixs	[in]	ͼ��У������ӳ�����
	 */
	int saveCalibrationDatas(const char* filename, RECTIFYMETHOD method, CornerDatas& cornerDatas, StereoParams& stereoParams, RemapMatrixs& remapMatrixs);

	/*----------------------------------
	 * ���� : ��ͼ�����У��
	 *----------------------------------
	 * ���� : StereoCalib::remapImage
	 * ���� : public 
	 * ���� : 0 - ����ʧ�ܣ�1 - �����ɹ�
	 *
	 * ���� : img1			[in]	����ͼ��1
	 * ���� : img2			[in]	����ͼ��2
	 * ���� : img1r			[out]	У��ͼ��1
	 * ���� : img2r			[out]	У��ͼ��2
	 * ���� : remapMatrixs	[in]	ͼ��У��������ݣ��������������ӳ�����
	 */
	int remapImage(cv::Mat& img1, cv::Mat& img2, cv::Mat& img1r, cv::Mat& img2r, RemapMatrixs& remapMatrixs);

	/*----------------------------
	 * ���� : ��ͼ�����½���ʾָ��������Ϣ
	 *----------------------------
	 * ���� : StereoCalib::showText
	 * ���� : private 
	 * ���� : void
	 *
	 * ���� : img	[in]	ͼ��
	 * ���� : text	[in]	������Ϣ
	 */
	void showText(cv::Mat& img, string text);

private:

    string m_workDir; // ����Ŀ¼

};

#endif