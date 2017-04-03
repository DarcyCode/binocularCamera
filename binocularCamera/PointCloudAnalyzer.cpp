/********************************************************************
	���� :	2012/10/28
	�ļ� :	.\StereoVision\StereoVision\PointCloudAnalyzer.cpp
	���� :	PointCloudAnalyzer
	���� :	��� chenyusiyuan AT 126 DOT com
	
	���� :	���Ʒ����������������ʵ�ִ���
*********************************************************************/

#include "StdAfx.h"
#include "PointCloudAnalyzer.h"


PointCloudAnalyzer::PointCloudAnalyzer(void)
{
}


PointCloudAnalyzer::~PointCloudAnalyzer(void)
{
}


/*----------------------------
 * ���� : ������Ŀ�꣬���Ŀ����Ϣ����
 *----------------------------
 * ���� : PointCloudAnalyzer::detectNearObject
 * ���� : public 
 * ���� : void
 *
 * ���� : image			[io]	���������ͼ�������ԭλ����������Ŀ��ߴ�λ��
 * ���� : pointCloud		[in]	��ά����
 * ���� : objectInfos	[out]	Ŀ����Ϣ����
 */
void PointCloudAnalyzer::detectNearObject(cv::Mat& image, cv::Mat& pointCloud, vector<ObjectInfo>& objectInfos)
{
	if (image.empty() || pointCloud.empty())
	{
		return;
	}

	// ��ȡ���ͼ��
	vector<cv::Mat> xyzSet;
	split(pointCloud, xyzSet);
	cv::Mat depth;
	xyzSet[2].copyTo(depth);

	// ���������ֵ���ж�ֵ������
	double maxVal = 0, minVal = 0;
	cv::Mat depthThresh = cv::Mat::zeros(depth.rows, depth.cols, CV_8UC1);
	cv::minMaxLoc(depth, &minVal, &maxVal);
	double thrVal = minVal * 1.5;
	threshold(depth, depthThresh, thrVal, 255, CV_THRESH_BINARY_INV);
	depthThresh.convertTo(depthThresh, CV_8UC1);
	imageDenoising(depthThresh, 3);

	// ��ȡ������ͷ�Ͻ���������Ϣ
	parseCandidates(depthThresh, depth, objectInfos);

	// ��������ֲ�
	showObjectInfo(objectInfos, image);
}


/*----------------------------
 * ���� : ͼ��ȥ��
 *----------------------------
 * ���� : PointCloudAnalyzer::imageDenoising
 * ���� : private 
 * ���� : void
 *
 * ���� : img	[in]	������ͼ��ԭλ����
 * ���� : iters	[in]	��̬ѧ�������
 */
void PointCloudAnalyzer::imageDenoising( cv::Mat& img, int iters )
{
	cv::Mat pyr = cv::Mat(img.cols/2, img.rows/2, img.type());

	IplImage iplImg = img;
	cvSmooth(&iplImg, &iplImg, CV_GAUSSIAN, 3, 3);	// ƽ���˲�

	pyrDown(img, pyr);	// ��ƽ�����ͼ����ж�������
	pyrUp(pyr, img);

	erode(img, img, 0, cv::Point(-1,-1), iters);	// ͼ��ʴ
	dilate(img, img, 0, cv::Point(-1,-1), iters);	// ͼ������
}


/*----------------------------
 * ���� : ���ɽ���������Ϣ����
 *----------------------------
 * ���� : PointCloudAnalyzer::parseCandidates
 * ���� : private 
 * ���� : void
 *
 * ���� : objects		[in]	�����ֵ����Ķ�ֵͼ����ʾ�˽�������ķֲ�
 * ���� : depthMap		[in]	����ά���ƾ����г�ȡ��������ݾ���
 * ���� : objectInfos	[out]	Ŀ����Ϣ����
 */
void PointCloudAnalyzer::parseCandidates(cv::Mat& objects, cv::Mat& depthMap, vector<ObjectInfo>& objectInfos)
{
	// ��ȡ��������
	vector<vector<cv::Point> > contours;	// ������������
	findContours(objects, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// ��������
	double areaThresh = 0.005 * depthMap.rows * depthMap.cols;
	cv::Mat mask = cv::Mat::zeros(objects.size(), CV_8UC1);
	bool useMeanDepth = false;

	for( UINT objID = 0; objID < contours.size(); objID++ )
	{
		cv::Mat contour = cv::Mat( contours[objID] );
		double area = contourArea( contour );
		if ( area > areaThresh ) // ������� 100 ����ʱ������Ч
		{
			ObjectInfo object;

			// ��������ڲ�������Ϊ��������
			mask = cv::Scalar(0);
			drawContours(mask, contours, objID, cv::Scalar(255), -1);

			// ������������
			object.boundRect = boundingRect( contour );
			object.minRect = minAreaRect( contour );
			object.center = object.minRect.center;

			// �����������
			if (useMeanDepth) //ȡƽ�����
			{
				cv::Scalar meanVal = cv::mean(depthMap, mask);
				object.distance = meanVal[0];
                object.nearest = object.center;
			} 
			else	//ȡ������
			{
				double minVal = 0, maxVal = 0;
				cv::Point minPos;
                cv::minMaxLoc(depthMap, &minVal, &maxVal, &minPos, NULL, mask);
                object.nearest = minPos;
				object.distance = depthMap.at<float>(minPos.y, minPos.x);
			}

			// ��������������Ϣ
			objectInfos.push_back( object );
		}
	}

	// �����������������
	std::sort( objectInfos.begin(), objectInfos.end(), std::less<ObjectInfo>() );
}


/*----------------------------
 * ���� : ���ƽ�������ߴ��λ��
 *----------------------------
 * ���� : PointCloudAnalyzer::showObjectInfo
 * ���� : private 
 * ���� : void
 *
 * ���� : objectInfos	[in]	����������Ϣ����
 * ���� : outImage		[io]	������ͷͼ���ڸ�ͼ���ϻ��ƽ���������Ϣ
 */
void PointCloudAnalyzer::showObjectInfo(vector<ObjectInfo>& objectInfos, cv::Mat& outImage)
{
	int showCount = objectInfos.size() < 5 ? objectInfos.size() : 5;
	
	// �������п���������
	for (int i = 0; i < showCount; i++)
	{
		//��������
		circle(outImage, objectInfos[i].center, 3, CV_RGB(0,0,255), 2);
		
		//������С����
		cv::Point2f rect_points[4]; 
		objectInfos[i].minRect.points( rect_points );
		if (i==0)
		{
			for( int j = 0; j < 4; j++ )
				line( outImage, rect_points[j], rect_points[(j+1)%4], CV_RGB(255,0,0), 4 );
		} 
		else
		{
			for( int j = 0; j < 4; j++ )
				line( outImage, rect_points[j], rect_points[(j+1)%4], CV_RGB(255-i*40,i*40,0), 2 );
		}
	}
}

