// Test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include <opencv/cv.hpp>
//#include "face_detection.h"
//#include "face_alignment.h"
//#include "face_identification.h"
//#include "common.h"
#include "common_tools.hpp"
#include "face_tools.hpp"
#include "image_tools.hpp"

int main(int argc, char* argv[])
{
	cv::destroyAllWindows();
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
			pIsMat[i] = false;
			std::cout << ") = empty mat." << std::endl;
		}
		else
		{
			pIsMat[i] = true;
			std::cout << "){rows=" << pMat[i].rows << ",cols=" << pMat[i].cols << "}" << std::endl;
		}
	}

	//Load faceTool.
	std::string exe_fullpath_str = argv[0];
	char exe_path[512];
	tools::full_to_path(argv[0], exe_path, 512);
	std::string exe_path_str = exe_path;
	tools::faceDetector face_detector;
	face_detector.initial(tools::accept_size_3X2, 
		(exe_path_str+"seeta_fd_frontal_v1.0.bin").c_str(),
		(exe_path_str + "seeta_fa_v1.1.bin").c_str());
	tools::faceCompare face_compare;
	face_compare.initial(tools::accept_size_3X2,
		(exe_path_str + "seeta_fr_v1.0.bin").c_str());


	//Deal different call.
	if (imageNum == 0) return 0;
	else if (imageNum == 1)
	{
		//Load images
		std::string image_path = argv[1];
		cv::Mat img = cv::imread(image_path, cv::IMREAD_ANYCOLOR);
		cv::Mat gray_img = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
		seeta::ImageData gray_img_data(gray_img.cols, gray_img.rows, gray_img.channels());
		gray_img_data.data = gray_img.data;

		// Detect face.
		std::vector <seeta::FaceInfo> face_info;
		std::vector <tools::faceLandMark> face_marks;
		long t0 = cv::getTickCount();
		bool isOK = face_detector.detect(gray_img_data, face_info, face_marks);
		long t1 = cv::getTickCount();
		double secs = (t1 - t0) / cv::getTickFrequency();
		std::cout << "Detect: "<< face_info.size() << " face,take " << secs << " seconds." << std::endl;

		// Show detected face
		for (int i=0;i<face_info.size();i++)
		{
			cv::Rect face_rect;
			face_rect.x = face_info[i].bbox.x;
			face_rect.y = face_info[i].bbox.y;
			face_rect.width = face_info[i].bbox.width;
			face_rect.height = face_info[i].bbox.height;
			cv::rectangle(img, face_rect, CV_RGB(0, 255, 255), 2, 8, 0);
			for (int k = 0; k < 5; k++)
			{
				cv::circle(img, cv::Point(face_marks.at(i).mark[k].x, face_marks.at(i).mark[k].y), 4,
					CV_RGB(0, 255, 255), 2);
			}
		}
		cv::imshow("First Face", img);
		cv::waitKey(100);
		cv::destroyAllWindows();
	}
	else if (imageNum >= 2)
	{
		//Load images
		std::string image_path1 = argv[1];
		cv::Mat img1 = cv::imread(image_path1, cv::IMREAD_ANYCOLOR);
		//cv::Mat img11;
		//tools::corp_size(img1, img11, tools::accept_size_3X2);
		seeta::ImageData img_data1(img1.cols, img1.rows, img1.channels());
		img_data1.data = img1.data;
		cv::Mat gray_img1;
		cv::cvtColor(img1, gray_img1, CV_RGB2GRAY);
		seeta::ImageData gray_img_data1(gray_img1.cols, gray_img1.rows, gray_img1.channels());
		gray_img_data1.data = gray_img1.data;

		std::string image_path2 = argv[2];
		cv::Mat img2 = cv::imread(image_path2, cv::IMREAD_ANYCOLOR);
		//cv::Mat img22;
		//tools::corp_size(img2, img22, tools::accept_size_3X2);
		seeta::ImageData img_data2(img2.cols, img2.rows, img2.channels());
		img_data2.data = img2.data;
		cv::Mat gray_img2;
		cv::cvtColor(img2, gray_img2, CV_RGB2GRAY);
		seeta::ImageData gray_img_data2(gray_img2.cols, gray_img2.rows, gray_img2.channels());
		gray_img_data2.data = gray_img2.data;

		// Detect face.
		std::vector <seeta::FaceInfo> face_info1;
		std::vector <tools::faceLandMark> face_marks1;
		long t0 = cv::getTickCount();
		bool isOK = face_detector.detect(gray_img_data1, face_info1, face_marks1);
		long t1 = cv::getTickCount();
		double secs = (t1 - t0) / cv::getTickFrequency();
		std::cout << "Detect: " << face_info1.size() << " face,take " << secs << " seconds." << std::endl;

		std::vector <seeta::FaceInfo> face_info2;
		std::vector <tools::faceLandMark> face_marks2;
		t0 = cv::getTickCount();
		isOK = face_detector.detect(gray_img_data2, face_info2, face_marks2);
		t1 = cv::getTickCount();
		secs = (t1 - t0) / cv::getTickFrequency();
		std::cout << "Detect: " << face_info2.size() << " face,take " << secs << " seconds." << std::endl;

		if (face_marks1.size()>0 && face_marks2.size()>0)
		{
			t0 = cv::getTickCount();
			float similar = face_compare.compare(img_data1, img_data2, face_marks1.at(0).mark, face_marks2.at(0).mark);
			t1 = cv::getTickCount();
			secs = (t1 - t0) / cv::getTickFrequency();
			std::cout << "Compare: similar=" << similar << " ,take " << secs << " seconds." << std::endl;
		}

		// Show detected face
		for (int i = 0; i < face_info1.size(); i++)
		{
			cv::Rect face_rect;
			face_rect.x = face_info1[i].bbox.x;
			face_rect.y = face_info1[i].bbox.y;
			face_rect.width = face_info1[i].bbox.width;
			face_rect.height = face_info1[i].bbox.height;
			cv::rectangle(img1, face_rect, CV_RGB(0, 255, 255), 2, 8, 0);
			for (int k=0;k<5;k++)
			{
				cv::circle(img1, cv::Point(face_marks1.at(i).mark[k].x, face_marks1.at(i).mark[k].y), 4,
					CV_RGB(0, 255, 255), 2);
			}
		}
		cv::imshow("First Face", img1);
		for (int i = 0; i < face_info2.size(); i++)
		{
			cv::Rect face_rect;
			face_rect.x = face_info2[i].bbox.x;
			face_rect.y = face_info2[i].bbox.y;
			face_rect.width = face_info2[i].bbox.width;
			face_rect.height = face_info2[i].bbox.height;
			cv::rectangle(img2, face_rect, CV_RGB(0, 255, 255), 2, 8, 0);
			for (int k = 0; k < 5; k++)
			{
				cv::circle(img2, cv::Point(face_marks2.at(i).mark[k].x, face_marks2.at(i).mark[k].y), 4,
					CV_RGB(0, 255, 255), 2);
			}
		}
		cv::imshow("Second Face", img2);
		cv::waitKey(0);
	}
	else return 0;
    return 0;
}