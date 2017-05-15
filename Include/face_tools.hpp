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
#include "common.h"
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
	sizeSetting accept_size_4X3 = { 90,120,25 };
	sizeSetting accept_size_3X4 = { 120,90,25 };
	sizeSetting accept_size_15X9 = { 120,200,30 };
	sizeSetting accept_size_9X15 = { 200,120,30 };
	sizeSetting accept_size_16X9 = { 180,270,40 };
	sizeSetting accept_size_9X16 = { 270,180,40 };
	//sizeSetting accept_size_3X2 = { 80,120,20 };
	//sizeSetting accept_size_2X3 = { 120,80,20 };
	//sizeSetting accept_size_4X3 = { 120,160,30 };
	//sizeSetting accept_size_3X4 = { 160,120,30 };
	//sizeSetting accept_size_15X9 = { 120,200,30 };
	//sizeSetting accept_size_9X15 = { 200,120,30 };
	//sizeSetting accept_size_16X9 = { 180,270,40 };
	//sizeSetting accept_size_9X16 = { 270,180,40 };
#endif

	struct faceLandMark
	{
		seeta::FacialLandmark mark[5];
		faceLandMark()
		{
			for (int i=0;i<5;i++)
			{
				mark[i].x = 0;
				mark[i].y = 0;
			}
		}
	};

	/**
	@class faceDetector : 
	@brief faceDetector use fix image size, just accept presupposed image size.
	*/
	class faceDetector
	{
	public:
		seeta::FaceDetection*  p_face_detector;
		seeta::FaceAlignment* p_point_detector;
		seeta::FaceIdentification face_recognizer;
		int feat_size;
		sizeSetting accept_size;
		int face_rows;
		int face_cols;
		int face_chn;

		faceDetector()
		{
			seeta::FaceDetection*  p_face_detector=0;
			seeta::FaceAlignment* p_point_detector=0;
			accept_size.img_width = accept_size_3X2.img_width;
			accept_size.img_height = accept_size_3X2.img_height;
			accept_size.target_size = accept_size_3X2.target_size;
			feat_size = 0;
			face_rows = 0;
			face_cols = 0;
			face_chn = 0;
		}
		~faceDetector()
		{
			delete p_face_detector;
			p_face_detector = 0;
			delete p_point_detector;
			p_point_detector = 0;
			feat_size = 0;
			face_rows = 0;
			face_cols = 0;
			face_chn = 0;
		}

		bool initial(sizeSetting ac_size, 
			const char* fd, 
			const char* fa,
			const char*fi)
		{
			try
			{
				accept_size.img_width = ac_size.img_width;
				accept_size.img_height = ac_size.img_height;
				accept_size.target_size = ac_size.target_size;

				p_face_detector = new seeta::FaceDetection(fd);
				p_face_detector->SetMinFaceSize(accept_size.target_size);
				p_face_detector->SetScoreThresh(2.f);
				p_face_detector->SetImagePyramidScaleFactor(0.8f);
				p_face_detector->SetWindowStep(4, 4);

				p_point_detector  = new seeta::FaceAlignment(fa);

				face_recognizer.LoadModel(fi);
				feat_size = face_recognizer.feature_size();
				face_rows = face_recognizer.crop_height();
				face_cols = face_recognizer.crop_width();
				face_chn = face_recognizer.crop_channels();
				return true;
			}
			catch (std::exception e)
			{
				return false;
			}
			catch (...)
			{
				return false;
			}
		}

		int detect(const seeta::ImageData img,
			std::vector <seeta::FaceInfo> & face_info,
			std::vector <tools::faceLandMark> & face_marks)
		{
			if (p_face_detector == 0 || p_point_detector == 0) return false;
			try
			{
				face_info = p_face_detector->Detect(img);
				for (int i = 0; i < face_info.size(); i++)
				{
					tools::faceLandMark faceLandMark;
					p_point_detector->PointDetectLandmarks(img, face_info.at(i), faceLandMark.mark);
					face_marks.push_back(faceLandMark);
				}
				return face_info.size();
			}
			catch (std::exception e)
			{
				return -1;
			}
			catch (...)
			{
				return -1;
			}
		}

		void precorp(int& cols,int& rows,int& channel)
		{
			rows = face_rows;
			cols = face_cols;
			channel = face_chn;
		}

		// Crop face with 3-channels image and 5 located landmark points.
		// 'corp_img' can be initialized as a cv::Mat sized by precorp.
		void corp(const seeta::ImageData& img,
			tools::faceLandMark face_mark,
			const seeta::ImageData& corp_img)
		{
			if (feat_size == 0) return;
			try
			{
				face_recognizer.CropFace(img, face_mark.mark, corp_img);
			}
			catch (std::exception e)
			{
				return;
			}
			catch (...)
			{
				return;
			}
		}

		float compare(const seeta::ImageData img1,
			const seeta::ImageData img2)
		{
			if (feat_size == 0) return false;
			try
			{
				float *fea1 = new float[feat_size];
				face_recognizer.ExtractFeature(img1, fea1);

				float *fea2 = new float[feat_size];
				face_recognizer.ExtractFeature(img2, fea2);

				// Caculate similarity of two faces
				float sim = face_recognizer.CalcSimilarity(fea1, fea2);
				delete[] fea1;
				delete[] fea2;
				return sim;
			}
			catch (std::exception e)
			{
				return -1;
			}
			catch (...)
			{
				return -1;
			}
		}

		float corp_compare(const seeta::ImageData img1,
			const seeta::ImageData img2,
			seeta::FacialLandmark face1_marks[5],
			seeta::FacialLandmark face2_marks[5])
		{
			if (feat_size == 0) return false;
			try
			{
				float *fea1 = new float[feat_size];
				long t0 = cv::getTickCount();
				face_recognizer.ExtractFeatureWithCrop(img1, face1_marks, fea1);
				long t1 = cv::getTickCount();
				double secs = (t1 - t0) / cv::getTickFrequency();
				std::string debugStr = tools::f_to_s(secs);
				OutputDebugStringA(("ExtractFeatureWithCrop take time = " + debugStr + "\n").c_str());

				float *fea2 = new float[feat_size];
				t0 = cv::getTickCount();
				face_recognizer.ExtractFeatureWithCrop(img2, face2_marks, fea2);
				t1 = cv::getTickCount();
				secs = (t1 - t0) / cv::getTickFrequency();
				debugStr = tools::f_to_s(secs);
				OutputDebugStringA(("ExtractFeatureWithCrop take time = " + debugStr + "\n").c_str());

				// Caculate similarity of two faces
				t0 = cv::getTickCount();
				float sim = face_recognizer.CalcSimilarity(fea1, fea2);
				t1 = cv::getTickCount();
				secs = (t1 - t0) / cv::getTickFrequency();
				debugStr = tools::f_to_s(secs);
				OutputDebugStringA(("CalcSimilarity take time = " + debugStr + "\n").c_str());
				delete[] fea1;
				delete[] fea2;
				return sim;
			}
			catch (std::exception e)
			{
				return -1;
			}
			catch (...)
			{
				return -1;
			}
		}
	};
}
#endif
