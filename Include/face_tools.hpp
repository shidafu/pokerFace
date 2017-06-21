/**
@file face_tools.hpp
@brief 
@author Leon Contact: towanglei@163.com
@version 1.0
@par History:
      1.0 : Leon 2017/05/11 18:48 build
*/
#pragma once
#ifndef FACE_TOOLS
#define FACE_TOOLS
#include "face_detection.h"
#include "face_alignment.h"
#include "face_identification.h"
#include "stasm_lib.h"
#include "common.h"
#include "mformat.hpp"
#include "mlog.hpp"
#include <cv.hpp>
#include <iostream>
namespace tools
{
#ifndef SIZE_SETTING
#define SIZE_SETTING
	struct sizeSetting
	{
		int img_width;
		int img_height;
		int target_size;
	};
	sizeSetting accept_size_3X2 = { 80,120,20 };
	sizeSetting accept_size_2X3 = { 120,80,20 };
	sizeSetting accept_size_4X3 = { 120,160,30 };
	sizeSetting accept_size_3X4 = { 160,120,30 };
	sizeSetting accept_size_15X9 = { 120,200,30 };
	sizeSetting accept_size_9X15 = { 200,120,30 };
	sizeSetting accept_size_16X9 = { 180,270,40 };
	sizeSetting accept_size_9X16 = { 270,180,40 };
#endif

	struct faceLandMark5
	{
		seeta::FacialLandmark mark[5];
		faceLandMark5()
		{
			for (int i=0;i<5;i++)
			{
				mark[i].x = -1;
				mark[i].y = -1;
			}
		}
		faceLandMark5& operator =(const faceLandMark5& fLandMark)
		{
			for (int i = 0; i < 5; i++)
			{
				mark[i].x = fLandMark.mark[i].x;
				mark[i].y = fLandMark.mark[i].y;
			}
			return *this;
		}
	};

	struct faceLandMark77
	{
		float marks[2* stasm_NLANDMARKS];
		faceLandMark77()
		{
			for (int i = 0; i<stasm_NLANDMARKS; i++)
			{
				marks[i * 2] = -1;
				marks[i * 2 + 1] = -1;
			}
		}
		faceLandMark77& operator =(const faceLandMark77& fLandMark)
		{
			for (int i = 0; i < stasm_NLANDMARKS; i++)
			{
				marks[i * 2] = fLandMark.marks[i * 2];
				marks[i * 2 + 1] = fLandMark.marks[i * 2 + 1];
			}
			return *this;
		}
	};

	struct faceInfoEx
	{
		bool exist;
		seeta::FaceInfo info;
		double mean;
		double std;
		faceInfoEx()
		{
			exist = false;
			info.bbox.x = -1;
			info.bbox.y = -1;
			info.bbox.width = -1;
			info.bbox.height = -1;
			info.roll = 0;
			info.yaw = 0;
			info.pitch = 0;
			info.score = -1;
			mean = 0.0;
			std = 0.0;
		}
		faceInfoEx& operator =(const faceInfoEx& fInfo)
		{
			exist = fInfo.exist;
			info.bbox.x = fInfo.info.bbox.x;
			info.bbox.y = fInfo.info.bbox.y;
			info.bbox.width = fInfo.info.bbox.width;
			info.bbox.height = fInfo.info.bbox.height;
			info.roll = fInfo.info.roll;
			info.yaw = fInfo.info.yaw;
			info.pitch = fInfo.info.pitch;
			info.score = fInfo.info.score;
			mean = fInfo.mean;
			std = fInfo.std;
			return *this;
		}
		faceInfoEx& operator =(const seeta::FaceInfo& fInfo)
		{
			exist = true;
			for (int i = 0; i < 5; i++)
			info.bbox.x = fInfo.bbox.x;
			info.bbox.y = fInfo.bbox.y;
			info.bbox.width = fInfo.bbox.width;
			info.bbox.height = fInfo.bbox.height;
			info.roll = fInfo.roll;
			info.yaw = fInfo.yaw;
			info.pitch = fInfo.pitch;
			info.score = fInfo.score;
			mean = 0;
			std = 0;
			return *this;
		}
	};

	/**
	@class faceDetector : 
	@brief faceDetector use fix image size, just accept presupposed image size.
	*/
	class faceDetector
	{
	public:
		std::string path_xml;

		faceDetector(const char* fs)
		{
			path_xml = fs;
		}
		~faceDetector()
		{
		}

		int detect(
			const char*  img,        // in: gray image data, top left corner at 0,0
			int          width,      // in: image width
			int          height,     // in: image height
			tools::faceInfoEx  & face_infoex,
			tools::faceLandMark77 & face_mark)
		{
			try
			{
				if (!stasm_search_single(&(face_num1[i]), face_marks77_1.at(i).marks,
					(char*)gray1.data, gray1.cols, gray1.rows, "../logs", exe_path_str.c_str()))
				{
					MLOG << "faceDetector::detect failed:"<< stasm_lasterr();
					throw -1;
				}

				face_info = p_face_detector->Detect(img);
				for (int i = 0; i < face_info.size(); i++)
				{
					tools::faceLandMark5 faceLandMark5;
					p_point_detector->PointDetectLandmarks(img, face_info.at(i), faceLandMark5.mark);
					face_marks.push_back(faceLandMark5);
				}
				return face_info.size();
			}
			catch (std::exception e)
			{
				MLOG << "faceDetector::detect failed\n";
				return -1;
			}
			catch (...)
			{
				MLOG << "faceDetector::initial failed\n";
				return -1;
			}
		}

		int detect(const seeta::ImageData img,
			std::vector <seeta::FaceInfo> & face_info,
			std::vector <tools::faceLandMark5> & face_marks)
		{
			if (p_face_detector == 0 || p_point_detector == 0)
			{
				MLOG<< "faceDetector::detect failed\n";
				return -1;
			}
			try
			{
				face_info = p_face_detector->Detect(img);
				for (int i = 0; i < face_info.size(); i++)
				{
					tools::faceLandMark5 faceLandMark5;
					p_point_detector->PointDetectLandmarks(img, face_info.at(i), faceLandMark5.mark);
					face_marks.push_back(faceLandMark5);
				}
				return face_info.size();
			}
			catch (std::exception e)
			{
				MLOG<< "faceDetector::detect failed\n";
				return -1;
			}
			catch (...)
			{
				MLOG<< "faceDetector::initial failed\n";
				return -1;
			}
		}

		int detect(cv::Mat img,
			std::vector <seeta::FaceInfo> & face_info,
			std::vector <tools::faceLandMark5> & face_marks)
		{
			if (p_face_detector == 0 || p_point_detector == 0)
			{
				MLOG<< "faceDetector::detect failed\n";
				return -1;
			}
			try
			{
				seeta::ImageData img_data(img.cols, img.rows, img.channels());
				img_data.data = img.data;
				face_info = p_face_detector->Detect(img_data);
				for (int i = 0; i < face_info.size(); i++)
				{
					tools::faceLandMark5 faceLandMark5;
					p_point_detector->PointDetectLandmarks(img_data, face_info.at(i), faceLandMark5.mark);
					face_marks.push_back(faceLandMark5);
				}
				return face_info.size();
			}
			catch (std::exception e)
			{
				MLOG<< "faceDetector::detect failed\n";
				return -1;
			}
			catch (...)
			{
				MLOG<< "faceDetector::detect failed\n";
				return -1;
			}
		}

		int detect(cv::Mat& img,
			tools::faceInfoEx & face_infoex,
			tools::faceLandMark5 & face_mark)
		{
			if (p_face_detector == 0 || p_point_detector == 0)
			{
				MLOG << "faceDetector::detect failed\n";
				return -1;
			}
			try
			{
				cv::Mat gray_img;
				cv::cvtColor(img, gray_img, CV_RGB2GRAY);
				seeta::ImageData gray_img_data(
					gray_img.cols, gray_img.rows, gray_img.channels());
				gray_img_data.data = gray_img.data;

				cv::Mat tmp_m, tmp_sd;
				double m = 0, sd = 0;
				cv::meanStdDev(gray_img, tmp_m, tmp_sd);
				m = tmp_m.at<double>(0, 0) / 256;
				sd = tmp_sd.at<double>(0, 0) / 256;

				face_infoex.exist = false;

				std::vector <seeta::FaceInfo> face_infos;
				face_infos = p_face_detector->Detect(gray_img_data);
				if (face_infos.size() >= 1)
				{
					face_infoex.exist = true;
					face_infoex = face_infos.at(0);
					p_point_detector->PointDetectLandmarks(gray_img_data, face_infoex.info, face_mark.mark);
				}
				face_infoex.mean = m;
				face_infoex.std = sd;
				return face_infos.size();
			}
			catch (std::exception e)
			{
				MLOG << "faceDetector::detect failed\n";
				return -1;
			}
			catch (...)
			{
				MLOG << "faceDetector::detect failed\n";
				return -1;
			}
		}

		int detect(std::string img_path,
			tools::faceInfoEx & face_infoex,
			tools::faceLandMark5 & face_mark)
		{
			if (p_face_detector == 0 || p_point_detector == 0)
			{
				MLOG << "faceDetector::detect failed\n";
				return -1;
			}
			try
			{
				cv::Mat img = cv::imread(img_path, cv::IMREAD_COLOR);
				cv::Mat gray_img;
				cv::cvtColor(img, gray_img, CV_RGB2GRAY);
				seeta::ImageData gray_img_data(
					gray_img.cols, gray_img.rows, gray_img.channels());
				gray_img_data.data = gray_img.data;

				cv::Mat tmp_m, tmp_sd;
				double m = 0, sd = 0;
				cv::meanStdDev(gray_img, tmp_m, tmp_sd);
				m = tmp_m.at<double>(0, 0) / 256;
				sd = tmp_sd.at<double>(0, 0) / 256;

				face_infoex.exist = false;

				std::vector <seeta::FaceInfo> face_infos;
				face_infos = p_face_detector->Detect(gray_img_data);
				if (face_infos.size()>=1)
				{
					face_infoex.exist = true;
					face_infoex = face_infos.at(0);
					p_point_detector->PointDetectLandmarks(gray_img_data, face_infoex.info, face_mark.mark);
				}
				face_infoex.mean = m;
				face_infoex.std = sd;
				return face_infos.size();
			}
			catch (std::exception e)
			{
				MLOG << "faceDetector::detect failed\n";
				return -1;
			}
			catch (...)
			{
				MLOG << "faceDetector::detect failed\n";
				return -1;
			}
		}

		/**
		@fn  presize
		@brief get compute buffer size of.
		*/
		void presize(int& cols,int& rows,int& channel,int& feats)
		{
			rows = face_rows;
			cols = face_cols;
			channel = face_chn;
			feats = feat_size;
		}

		/**
		@fn  corp
		@brief use buffer sized by presize.
		@param[out]  corp_img: buffer presized.
		*/
		void corp(const seeta::ImageData& img,
			tools::faceLandMark5 face_mark,
			const seeta::ImageData& corp_img)
		{
			if (feat_size == 0)
			{
				MLOG<< "faceDetector::corp failed\n";
				return;
			}
			try
			{
				face_recognizer.CropFace(img, face_mark.mark, corp_img);
			}
			catch (std::exception e)
			{
				MLOG<< "faceDetector::corp failed\n";
				return;
			}
			catch (...)
			{
				MLOG<< "faceDetector::corp failed\n";
				return;
			}
		}

		void corp(cv::Mat& img,
			tools::faceLandMark5 face_mark,
			cv::Mat& corp_img)
		{
			if (feat_size == 0)
			{
				MLOG<< "faceDetector::corp failed\n";
				return;
			}
			try
			{
				seeta::ImageData img_data(img.cols, img.rows, img.channels());
				img_data.data = img.data;
				corp_img = cv::Mat(face_rows,face_cols,CV_8UC(face_chn));
				seeta::ImageData corp_img_data(corp_img.cols, corp_img.rows, corp_img.channels());
				corp_img_data.data = corp_img.data;
				face_recognizer.CropFace(img_data, face_mark.mark, corp_img_data);
			}
			catch (std::exception e)
			{
				MLOG<< "faceDetector::corp failed\n";
				return;
			}
			catch (...)
			{
				MLOG<< "faceDetector::corp failed\n";
				return;
			}
		}

	};
}
#endif
