/********************************************************************
	���� :	2012/10/28
	�ļ� :	.\StereoVision\StereoVision\PointCloudAnalyzer.h
	���� :	PointCloudAnalyzer
	���� :	��� chenyusiyuan AT 126 DOT com
	
	���� :	���Ʒ����������������ͷ�ļ�
*********************************************************************/

#ifndef _POINT_CLOUD_ANALYZER_H_
#define _POINT_CLOUD_ANALYZER_H_

#pragma once

#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/imgproc/imgproc_c.h>
#include "opencv2/highgui/highgui.hpp"

using namespace std;


class PointCloudAnalyzer
{
public:
	PointCloudAnalyzer(void);
	~PointCloudAnalyzer(void);

	// ������Ϣ �ṹ��
	struct ObjectInfo
	{
		cv::Point		center;		//����
        cv::Point       nearest;    //�����
		double			distance;	//����
		double			area;		//���
		cv::Rect		boundRect;	//��Ӿ���
		cv::RotatedRect minRect;	//��С����

		// ���帳ֵ����
		void operator = (const ObjectInfo& rhs) 
		{
            center = rhs.center;
            nearest = rhs.nearest;
			distance = rhs.distance;
			area = rhs.area;
			boundRect = rhs.boundRect;
			minRect = rhs.minRect;
		}

		// ���վ��붨���������
		bool operator < (const ObjectInfo& rhs ) const   //��������ʱ����д�ĺ���
		{   
			return distance < rhs.distance; 
		}
		bool operator > (const ObjectInfo& rhs ) const   //��������ʱ����д�ĺ���
		{   
			return distance > rhs.distance; 
		}

	};

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
	void detectNearObject(cv::Mat& image, cv::Mat& pointCloud, vector<ObjectInfo>& objectInfos);

private:

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
	void imageDenoising( cv::Mat& img, int iters );

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
	void parseCandidates(cv::Mat& objects, cv::Mat& depthMap, vector<ObjectInfo>& objectInfos);

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
	void showObjectInfo(vector<ObjectInfo>& objectInfos, cv::Mat& outImage);

};

#endif