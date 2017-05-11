// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include <opencv/cv.hpp>
#include "face_detection.h"
#include "face_alignment.h"
#include "face_identification.h"
#include "common.h"
#include "common_tools.hpp"

int main(int argc, char* argv[])
{
	//Load para
	int imageNum = argc - 1;
	std::string *pImage_path = new std::string[imageNum];
	cv::Mat *pMat = new cv::Mat[imageNum];
	bool *pIsMat = new bool[imageNum];
	for (int i = 0; i < imageNum; i++)
	{
		pImage_path[i] = argv[i + 1];
		std::cout << "Mat[" << i << "]=imread(" << pImage_path[i];
		pMat[i] = cv::imread(pImage_path[i]);
		if (pMat[i].empty())
		{
			pIsMat[i] == false;
			std::cout << ") = empty mat." << std::endl;
		}
		else
		{
			pIsMat[i] == true;
			std::cout << "){rows=" << pMat[i].rows << ",cols=" << pMat[i].cols << "}" << std::endl;
		}
	}

	//Load FaceIdentification.
	std::string exe_fullpath_str = argv[0];
	char exe_path[512];
	full_to_path(argv[0], exe_path, 512);
	std::string exe_path_str = exe_path;
	seeta::FaceDetection  face_detector((exe_path_str + "..\\..\\..\\Model\\seeta_fd_frontal_v1.0.bin").c_str());
	face_detector.SetMinFaceSize(100);
	face_detector.SetScoreThresh(2.f);
	face_detector.SetImagePyramidScaleFactor(0.8f);
	face_detector.SetWindowStep(4, 4);

	seeta::FaceAlignment point_detector((exe_path_str + "..\\..\\..\\Model\\seeta_fa_v1.1.bin").c_str());

	seeta::FaceIdentification face_recognizer((exe_path_str + "..\\..\\..\\Model\\seeta_fr_v1.0.bin").c_str());
	int feat_size = face_recognizer.feature_size();

	//Deal different call.
	if (imageNum == 0) return 0;
	else if (imageNum == 1|| imageNum == 2)
	{
		//Load images
		std::string image_path = argv[2];
		cv::Mat gray_img = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
		//cv::imshow("Detect Face", src_img);
		//cv::waitKey(1000);
		seeta::ImageData gray_img_data(gray_img.cols, gray_img.rows, gray_img.channels());
		gray_img_data.data = gray_img.data;

		// Detect face.
		long t0 = cv::getTickCount();
		std::vector<seeta::FaceInfo> faces = face_detector.Detect(gray_img_data);
		long t1 = cv::getTickCount();
		double secs = (t1 - t0) / cv::getTickFrequency();
		std::cout << "Detect: "<< faces.size() << " face,take " << secs << " seconds." << std::endl;

		// Show detected face
		for (int i=0;i<faces.size();i++)
		{
			cv::Rect face_rect;
			face_rect.x = faces[i].bbox.x;
			face_rect.y = faces[i].bbox.y;
			face_rect.width = faces[i].bbox.width;
			face_rect.height = faces[i].bbox.height;
			cv::rectangle(gray_img, face_rect, CV_RGB(0, 0, 255), 4, 8, 0);
		}
		cv::imshow("Detect Face", gray_img);
		cv::waitKey(1);
		cv::destroyAllWindows();
	}
	//else
	//{
	//	std::string *pImage_path = new std::string[imageNum];
	//	cv::Mat *pMat = new cv::Mat[imageNum];
	//	bool *pIsMat = new bool[imageNum];
	//	for (int i = 0; i < imageNum; i++)
	//	{
	//		pImage_path[i] = argv[i + 1];
	//		std::cout << "Mat[" << i << "]=imread(" << pImage_path[i];
	//		pMat[i] = cv::imread(pImage_path[i]);
	//		if (pMat[i - 1].empty())
	//		{
	//			pIsMat[i - 1] == false;
	//			std::cout << ") = empty mat." << std::endl;
	//		}
	//		else
	//		{
	//			pIsMat[i - 1] == true;
	//			std::cout << "){rows=" << pMat[i - 1].rows << ",cols=" << pMat[i - 1].cols << "}" << std::endl;
	//		}
	//	}
	//}
    return 0;
}