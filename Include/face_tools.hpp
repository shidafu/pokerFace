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
	sizeSetting accept_size_3X2 = { 160,240,40 };
	sizeSetting accept_size_2X3 = { 240,160,40 };
	sizeSetting accept_size_4X3 = { 240,320,60 };
	sizeSetting accept_size_3X4 = { 320,240,60 };
	sizeSetting accept_size_15X9 = { 240,400,60 };
	sizeSetting accept_size_9X15 = { 400,240,60 };
	sizeSetting accept_size_16X9 = { 360,540,80 };
	sizeSetting accept_size_9X16 = { 540,360,80 };
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
		sizeSetting accept_size;

		faceDetector()
		{
			seeta::FaceDetection*  p_face_detector=0;
			seeta::FaceAlignment* p_point_detector=0;
			accept_size.img_width = accept_size_3X2.img_width;
			accept_size.img_height = accept_size_3X2.img_height;
			accept_size.target_size = accept_size_3X2.target_size;
		}
		~faceDetector()
		{
			delete p_face_detector;
			p_face_detector = 0;
			delete p_point_detector;
			p_point_detector = 0;
		}

		bool initial(sizeSetting ac_size, 
			const char* fd, 
			const char* fa)
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

		int detect(seeta::ImageData img,
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
	};

	/**
	@class faceCompare :
	@brief faceCompare use fix image size, just accept presupposed image size.
	*/
	class faceCompare
	{
	public:
		seeta::FaceIdentification face_recognizer;
		int feat_size;
		sizeSetting accept_size;

		faceCompare()
		{
			feat_size = 0;
			accept_size.img_width = accept_size_3X2.img_width;
			accept_size.img_height = accept_size_3X2.img_height;
			accept_size.target_size = accept_size_3X2.target_size;
		}
		~faceCompare()
		{
			feat_size = 0;
		}

		bool initial(sizeSetting ac_size, const char*fi)
		{
			try
			{
				accept_size.img_width = ac_size.img_width;
				accept_size.img_height = ac_size.img_height;
				accept_size.target_size = ac_size.target_size;

				face_recognizer.LoadModel(fi);
				feat_size = face_recognizer.feature_size();
				return true;
			}
			catch (...)
			{
				return false;
			}
		}

		float compare(seeta::ImageData img1,
			seeta::ImageData img2,
			seeta::FacialLandmark face1_marks[5],
			seeta::FacialLandmark face2_marks[5])
		{
			if (feat_size == 0) return false;
			try
			{
				float *fea1 = new float[feat_size];
				face_recognizer.ExtractFeatureWithCrop(img1, face1_marks, fea1);
				float *fea2 = new float[feat_size];
				face_recognizer.ExtractFeatureWithCrop(img2, face2_marks, fea2);

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
	};
}
#endif
