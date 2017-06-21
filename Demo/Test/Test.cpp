//// Test.cpp : 定义控制台应用程序的入口点。
////
//
#include "stdafx.h"
#include<iostream>
#include <opencv/cv.hpp>
#include "common_tools.hpp"
#include "face_tools.hpp"
#include "image_tools.hpp"
#include "stasm_lib.h"
//#include "json.h"

//Load faceTool.
tools::faceDetector face_detector;

bool showImg = true;

//int main_seataface(int argc, char* argv[])
//{
//	cv::destroyAllWindows();
//
//	//Load faceTool.
//	std::string exe_fullpath_str = argv[0];
//	char exe_path[512];
//	tools::full_to_path(argv[0], exe_path, 512);
//	std::string exe_path_str = exe_path;
//	face_detector.initial(tools::accept_size_3X2,
//		(exe_path_str + "seeta_fd_frontal_v1.0.bin").c_str(),
//		(exe_path_str + "seeta_fa_v1.1.bin").c_str(),
//		(exe_path_str + "seeta_fr_v1.0.bin").c_str());
//	int face_cols, face_rows, face_chn, face_feats;
//	face_detector.presize(face_cols, face_rows, face_chn, face_feats);
//
//	//Load files.
//	std::string path1 = argv[1]; 
//	std::vector<std::string> files1;
//	tools::dir_files(path1, "\\*.jpg", files1);
//	std::cout << "Find " << files1.size() << " image in " << path1 << std::endl;
//	std::string path2 = argv[2];
//	std::vector<std::string> files2;
//	tools::dir_files(path2, "\\*.jpg", files2);
//	std::cout << "Find " << files2.size() << " image in " << path2 << std::endl;
//
//	//Initial doc to saved
//	std::string out_path= path1+"\\..\\Test.json";
//	std::ofstream fout;
//	fout.open(out_path, std::ofstream::out | std::ofstream::app);
//	int* face_num1 = new int[files1.size()];
//	std::vector <tools::faceInfoEx> face_infos1;
//	std::vector <tools::faceLandMark5> face_marks5_1;
//	float** face_feas1 = new float*[files1.size()];
//	for (int i = 0; i < files1.size(); i++)
//	{
//		face_num1[i] = 0;
//		face_infos1.push_back(*(new tools::faceInfoEx()));
//		face_marks5_1.push_back(*(new tools::faceLandMark5()));		
//		face_feas1[i] = new float[face_feats];
//		for (int j = 0; j < face_feats; j++)
//		{
//			face_feas1[i][j] = 0;
//		}
//	}
//	int* face_num2 = new int[files2.size()];
//	std::vector <tools::faceInfoEx> face_infos2;
//	std::vector <tools::faceLandMark5> face_marks5_2;
//	float** face_feas2 = new float*[files2.size()];
//	for (int i = 0; i < files2.size(); i++)
//	{
//		face_num2[i] = 0;
//		face_infos2.push_back(*(new tools::faceInfoEx()));
//		face_marks5_2.push_back(*(new tools::faceLandMark5()));
//		face_feas2[i] = new float[face_feats];
//		for (int j = 0; j < face_feats; j++)
//		{
//			face_feas2[i][j] = 0;
//		}
//	}
//	double** compare_mat = new double*[files1.size()];
//	for (int i = 0; i < files1.size(); i++)
//	{
//		compare_mat[i] = new double[files2.size()];
//	}
//
//	char path1_chars[512];
//	tools::path_to_jsonpath(path1.c_str(), path1_chars,512);
//	std::cout << "{\n \"path1\":\"" << path1_chars << "\",\n";
//	fout << "{\n \"path1\":\"" << path1_chars << "\",\n";
//	std::cout << " \"files1\":[\n";
//	fout << " \"files1\":[\n";
//
//	// Detect face
//	for (int i = 0; i < files1.size(); i++)
//	{
//		// Read
//		std::string filename1 = path1 + "\\" + files1.at(i);
//		cv::Mat img = cv::imread(filename1);
//		//seeta::ImageData img_data(img.cols, img.rows, img.channels());
//		//img_data.data = img.data;
//
//		std::cout << "  {\n   \"id\":" << i << ",\n";
//		fout << "  {\n   \"id\":" << i << ",\n";
//		std::cout << "   \"name\":\"" << files1.at(i) << "\",\n";
//		fout << "   \"name\":\"" << files1.at(i) << "\",\n";
//
//		// Detect
//		face_num1[i] = face_detector.detect(filename1,
//			face_infos1.at(i), face_marks5_1.at(i));
//		std::cout << "   \"exist\":" << face_num1[i] << ",\n";
//		fout << "   \"exist\":" << face_num1[i] << ",\n";
//		std::cout << "   \"mean\":" << face_infos1.at(i).mean << ",\n";
//		fout << "   \"mean\":" << face_infos1.at(i).mean << ",\n";
//		std::cout << "   \"std\":" << face_infos1.at(i).std << ",\n";
//		fout << "   \"std\":" << face_infos1.at(i).std << ",\n";
//		//if (face_num1[i] > 0)
//		{
//			std::cout << "   \"score\":" << face_infos1.at(i).info.score << ",\n";
//			fout << "   \"score\":" << face_infos1.at(i).info.score << ",\n";
//
//			std::cout << "   \"box\":[" << face_infos1.at(i).info.bbox.x << ","
//				<< face_infos1.at(i).info.bbox.y << ","
//				<< face_infos1.at(i).info.bbox.width << ","
//				<< face_infos1.at(i).info.bbox.height << "],\n";
//			fout << "   \"box\":[" << face_infos1.at(i).info.bbox.x << ","
//				<< face_infos1.at(i).info.bbox.y << ","
//				<< face_infos1.at(i).info.bbox.width << ","
//				<< face_infos1.at(i).info.bbox.height << "],\n";
//
//			std::cout << "   \"roll_yaw_pitch\":[" << face_infos1.at(i).info.roll << ","
//				<< face_infos1.at(i).info.yaw << ","
//				<< face_infos1.at(i).info.pitch << "],\n";
//			fout << "   \"roll_yaw_pitch\":[" << face_infos1.at(i).info.roll << ","
//				<< face_infos1.at(i).info.yaw << ","
//				<< face_infos1.at(i).info.pitch << "],\n";
//
//			std::cout << "   \"marks\":[["
//				<< face_marks5_1.at(i).mark[0].x << "," << face_marks5_1.at(i).mark[0].y << "],"
//				<< "[" << face_marks5_1.at(i).mark[1].x << "," << face_marks5_1.at(i).mark[1].y << "],"
//				<< "[" << face_marks5_1.at(i).mark[2].x << "," << face_marks5_1.at(i).mark[2].y << "],"
//				<< "[" << face_marks5_1.at(i).mark[3].x << "," << face_marks5_1.at(i).mark[3].y << "],"
//				<< "[" << face_marks5_1.at(i).mark[4].x << "," << face_marks5_1.at(i).mark[4].y << "]],\n";
//			fout << "   \"marks\":[["
//				<< face_marks5_1.at(i).mark[0].x << "," << face_marks5_1.at(i).mark[0].y << "],"
//				<< "[" << face_marks5_1.at(i).mark[1].x << "," << face_marks5_1.at(i).mark[1].y << "],"
//				<< "[" << face_marks5_1.at(i).mark[2].x << "," << face_marks5_1.at(i).mark[2].y << "],"
//				<< "[" << face_marks5_1.at(i).mark[3].x << "," << face_marks5_1.at(i).mark[3].y << "],"
//				<< "[" << face_marks5_1.at(i).mark[4].x << "," << face_marks5_1.at(i).mark[4].y << "]],\n";
//			
//			if (showImg)
//			{
//				cv::Mat sImg;
//				img.copyTo(sImg);
//				cv::Rect face_rect;
//				face_rect.x = face_infos1.at(i).info.bbox.x;
//				face_rect.y = face_infos1.at(i).info.bbox.y;
//				face_rect.width = face_infos1.at(i).info.bbox.width;
//				face_rect.height = face_infos1.at(i).info.bbox.height;
//				cv::rectangle(sImg, face_rect, CV_RGB(0, 255, 255), 1, 8, 0);
//				for (int k = 0; k < 5; k++)
//				{
//					cv::circle(sImg, cv::Point(face_marks5_1.at(i).mark[k].x, face_marks5_1.at(i).mark[k].y), 4,
//						CV_RGB(0, 255, 255), 1);
//				}
//				cv::imshow("source image", sImg);
//			    cv::waitKey(1);
//			}
//
//			// Corp
//			cv::Mat corp_img = cv::Mat(face_rows, face_cols, CV_8UC(face_chn));
//			face_detector.corp(img, face_marks5_1.at(i), corp_img);
//			std::string corpfilename1 = path1 + "\\corp\\" + files1.at(i);
//			cv::imwrite(corpfilename1, corp_img);
//			std::cout << "   \"face\":\"corp\\\\" << files1.at(i) << "\",\n";
//			fout << "   \"face\":\"corp\\\\" << files1.at(i) << "\",\n";
//			if (showImg)
//			{
//				cv::imshow("corp image", corp_img);
//				cv::waitKey(1);
//			}
//
//			// Feature
//			face_detector.getfeature(corp_img, face_feas1[i]);
//			std::cout << "   \"feas\":[";
//			fout << "   \"feas\":[";
//			for (int j = 0; j < face_feats - 1; j++)
//			{
//				std::cout << face_feas1[i][j] << ",";
//				fout << face_feas1[i][j] << ",";
//			}
//			std::cout << face_feas1[i][face_feats - 1] << "]\n  }";
//			fout << face_feas1[i][face_feats - 1] << "]\n  }";
//			if (i == files1.size() - 1)
//			{
//				std::cout << "\n ],\n";
//				fout << "\n ],\n";
//			}
//			else
//			{
//				std::cout << ",\n";
//				fout << ",\n";
//			}
//		}
//		//else
//		//{
//		//	std::cout << "   \"score\":" << -1 << ",\n";
//		//	fout << "   \"score\":" << -1 << ",\n";
//		//}
//	}
//
//	char path2_chars[512];
//	tools::path_to_jsonpath(path2.c_str(), path2_chars, 512);
//	std::cout << " \"path2\":\"" << path2_chars << "\",\n";
//	fout << " \"path2\":\"" << path2_chars << "\",\n";
//	std::cout << " \"files2\":[\n";
//	fout << " \"files2\":[\n";
//
//	// Detect face
//	for (int i = 0; i < files2.size(); i++)
//	{
//		// Read
//		std::string filename2 = path2 + "\\" + files2.at(i);
//		cv::Mat img = cv::imread(filename2);
//		std::cout << "  {\n   \"id\":" << i << ",\n";
//		fout << "  {\n   \"id\":" << i << ",\n";
//		std::cout << "   \"name\":\"" << files2.at(i) << "\",\n";
//		fout << "   \"name\":\"" << files2.at(i) << "\",\n";
//
//		// Detect
//		face_num2[i] = face_detector.detect(filename2,
//			face_infos2.at(i), face_marks5_2.at(i));
//		std::cout << "   \"exist\":" << face_num2[i] << ",\n";
//		fout << "   \"exist\":" << face_num2[i] << ",\n";
//		std::cout << "   \"mean\":" << face_infos2.at(i).mean << ",\n";
//		fout << "   \"mean\":" << face_infos2.at(i).mean << ",\n";
//		std::cout << "   \"std\":" << face_infos2.at(i).std << ",\n";
//		fout << "   \"std\":" << face_infos2.at(i).std << ",\n";
//		//if (face_num2[i] > 0)
//		{
//			std::cout << "   \"score\":" << face_infos2.at(i).info.score << ",\n";
//			fout << "   \"score\":" << face_infos2.at(i).info.score << ",\n";
//
//			std::cout << "   \"box\":[" << face_infos2.at(i).info.bbox.x << ","
//				<< face_infos2.at(i).info.bbox.y << ","
//				<< face_infos2.at(i).info.bbox.width << ","
//				<< face_infos2.at(i).info.bbox.height << "],\n";
//			fout << "   \"box\":[" << face_infos2.at(i).info.bbox.x << ","
//				<< face_infos2.at(i).info.bbox.y << ","
//				<< face_infos2.at(i).info.bbox.width << ","
//				<< face_infos2.at(i).info.bbox.height << "],\n";
//
//			std::cout << "   \"roll_yaw_pitch\":[" << face_infos2.at(i).info.roll << ","
//				<< face_infos2.at(i).info.yaw << ","
//				<< face_infos2.at(i).info.pitch << "],\n";
//			fout << "   \"roll_yaw_pitch\":[" << face_infos2.at(i).info.roll << ","
//				<< face_infos2.at(i).info.yaw << ","
//				<< face_infos2.at(i).info.pitch << "],\n";
//
//			std::cout << "   \"marks\":[["
//				<< face_marks5_2.at(i).mark[0].x << "," << face_marks5_2.at(i).mark[0].y << "],"
//				<< "[" << face_marks5_2.at(i).mark[1].x << "," << face_marks5_2.at(i).mark[1].y << "],"
//				<< "[" << face_marks5_2.at(i).mark[2].x << "," << face_marks5_2.at(i).mark[2].y << "],"
//				<< "[" << face_marks5_2.at(i).mark[3].x << "," << face_marks5_2.at(i).mark[3].y << "],"
//				<< "[" << face_marks5_2.at(i).mark[4].x << "," << face_marks5_2.at(i).mark[4].y << "]],\n";
//			fout << "   \"marks\":[["
//				<< face_marks5_2.at(i).mark[0].x << "," << face_marks5_2.at(i).mark[0].y << "],"
//				<< "[" << face_marks5_2.at(i).mark[1].x << "," << face_marks5_2.at(i).mark[1].y << "],"
//				<< "[" << face_marks5_2.at(i).mark[2].x << "," << face_marks5_2.at(i).mark[2].y << "],"
//				<< "[" << face_marks5_2.at(i).mark[3].x << "," << face_marks5_2.at(i).mark[3].y << "],"
//				<< "[" << face_marks5_2.at(i).mark[4].x << "," << face_marks5_2.at(i).mark[4].y << "]],\n";
//
//			if (showImg)
//			{
//				cv::Mat sImg;
//				img.copyTo(sImg);
//				cv::Rect face_rect;
//				face_rect.x = face_infos2.at(i).info.bbox.x;
//				face_rect.y = face_infos2.at(i).info.bbox.y;
//				face_rect.width = face_infos2.at(i).info.bbox.width;
//				face_rect.height = face_infos2.at(i).info.bbox.height;
//				cv::rectangle(sImg, face_rect, CV_RGB(0, 255, 255), 1, 8, 0);
//				for (int k = 0; k < 5; k++)
//				{
//					cv::circle(sImg, cv::Point(face_marks5_2.at(i).mark[k].x, face_marks5_2.at(i).mark[k].y), 4,
//						CV_RGB(0, 255, 255), 1);
//				}
//				cv::imshow("source image", sImg);
//				cv::waitKey(1);
//			}
//
//			// Corp
//			cv::Mat corp_img = cv::Mat(face_rows, face_cols, CV_8UC(face_chn));
//			face_detector.corp(img, face_marks5_2.at(i), corp_img);
//			std::string corpfilename2 = path2 + "\\corp\\" + files2.at(i);
//			cv::imwrite(corpfilename2, corp_img);
//			std::cout << "   \"face\":\"corp\\\\" << files2.at(i) << "\",\n";
//			fout << "   \"face\":\"corp\\\\" << files2.at(i) << "\",\n";
//			if (showImg)
//			{
//				cv::imshow("corp image", corp_img);
//				cv::waitKey(1);
//			}
//
//			// Feature
//			face_detector.getfeature(corp_img, face_feas2[i]);
//			std::cout << "   \"feas\":[";
//			fout << "   \"feas\":[";
//			for (int j = 0; j < face_feats - 1; j++)
//			{
//				std::cout << face_feas2[i][j] << ",";
//				fout << face_feas2[i][j] << ",";
//			}
//			std::cout << face_feas2[i][face_feats - 1] << "]\n  }";
//			fout << face_feas2[i][face_feats - 1] << "]\n  }";
//			if (i == files2.size() - 1)
//			{
//				std::cout << "\n ],\n";
//				fout << "\n ],\n";
//			}
//			else
//			{
//				std::cout << ",\n";
//				fout << ",\n";
//			}
//		}
//		//else
//		//{
//		//	std::cout << "   \"score\":" << -1 << ",\n";
//		//	fout << "   \"score\":" << -1 << ",\n";
//		//}
//	}
//
//	std::cout << " \"compare_matrix\":[\n";
//	fout << " \"compare_matrix\":[\n";
//	// Compare face
//	for (int i = 0; i < files1.size(); i++)
//	{
//		std::cout << "  [";
//		fout << "  [";
//		for (int j = 0; j < files2.size(); j++)
//		{
//			compare_mat[i][j] = -1;
//			if (face_num1[i]>0 && face_num2[j]>0)
//			{
//				compare_mat[i][j] = face_detector.compare(face_feas1[i], face_feas2[j]);
//			}
//			std::cout << compare_mat[i][j];
//			fout << compare_mat[i][j];
//			if (j != files2.size()-1)
//			{
//				std::cout << ",";
//				fout << ",";
//			}
//		}
//		if (i != files1.size() - 1)
//		{
//			std::cout << "],\n";
//			fout << "],\n";
//		}
//		else
//		{
//			std::cout << "]";
//			fout << "]";
//		}
//	}
//	std::cout << "]\n";
//	fout << "]\n";
//	std::cout << "}";
//	fout << "}";
//
//	delete[] face_num1;
//	for (int i = 0; i < files1.size(); i++)
//	{
//		delete[] face_feas1[i];
//	}
//	delete[] face_feas1;
//	for (int i = 0; i < files2.size(); i++)
//	{
//		delete[] face_feas2[i];
//	}
//	delete[] face_num2;
//	delete[] face_feas2;
//	for (int i = 0; i < files1.size(); i++)
//	{
//		delete[] compare_mat[i];
//	}
//	delete[] compare_mat;
//
//	if (fout.is_open())
//	{
//		fout.close();
//	}
//    return 0;
//}

int main(int argc, char* argv[])
{
	cv::destroyAllWindows();

	//Load stasm_lib.
	std::string exe_fullpath_str = argv[0];
	char exe_path[512];
	tools::full_to_path(argv[0], exe_path, 512);
	std::string exe_path_str = exe_path;
	//stasm_init((exe_path_str + "haarcascade_frontalface_alt2.xml").c_str(), 0);
	int face_feats = 8;

	//Load files.
	std::string path1 = argv[1];
	std::vector<std::string> files1;
	tools::dir_files(path1, "\\*.jpg", files1);
	std::cout << "Find " << files1.size() << " image in " << path1 << std::endl;
	std::string path2 = argv[2];
	std::vector<std::string> files2;
	tools::dir_files(path2, "\\*.jpg", files2);
	std::cout << "Find " << files2.size() << " image in " << path2 << std::endl;

	//Initial doc to saved
	std::string out_path = path1 + "\\..\\Test.json";
	std::ofstream fout;
	fout.open(out_path, std::ofstream::out | std::ofstream::app);
	int* face_num1 = new int[files1.size()];
	std::vector <tools::faceInfoEx> face_infos1;
	std::vector <tools::faceLandMark77> face_marks77_1;
	float** face_feas1 = new float*[files1.size()];
	for (int i = 0; i < files1.size(); i++)
	{
		face_num1[i] = 0;
		face_infos1.push_back(*(new tools::faceInfoEx()));
		face_marks77_1.push_back(*(new tools::faceLandMark77()));
		face_feas1[i] = new float[face_feats];
		for (int j = 0; j < face_feats; j++)
		{
			face_feas1[i][j] = 0;
		}
	}
	int* face_num2 = new int[files2.size()];
	std::vector <tools::faceInfoEx> face_infos2;
	std::vector <tools::faceLandMark77> face_marks77_2;
	float** face_feas2 = new float*[files2.size()];
	for (int i = 0; i < files2.size(); i++)
	{
		face_num2[i] = 0;
		face_infos2.push_back(*(new tools::faceInfoEx()));
		face_marks77_2.push_back(*(new tools::faceLandMark77()));
		face_feas2[i] = new float[face_feats];
		for (int j = 0; j < face_feats; j++)
		{
			face_feas2[i][j] = 0;
		}
	}
	double** compare_mat = new double*[files1.size()];
	for (int i = 0; i < files1.size(); i++)
	{
		compare_mat[i] = new double[files2.size()];
	}

	char path1_chars[512];
	tools::path_to_jsonpath(path1.c_str(), path1_chars, 512);
	std::cout << "{\n \"path1\":\"" << path1_chars << "\",\n";
	fout << "{\n \"path1\":\"" << path1_chars << "\",\n";
	std::cout << " \"files1\":[\n";
	fout << " \"files1\":[\n";

	double scale = 2;
	// Detect face
	for (int i = 0; i < files1.size(); i++)
	{
		// Read
		std::string filename1 = path1 + "\\" + files1.at(i);
		cv::Mat img1 = cv::imread(filename1, cv::IMREAD_COLOR);
		cv::Size dsize = cv::Size(img1.cols*scale, img1.rows*scale);
		cv::Mat eimg1 = cv::Mat(dsize, CV_32S);
		cv::resize(img1, eimg1, dsize);
		cv::Mat gray1;
		cv::cvtColor(eimg1, gray1, CV_RGB2GRAY);
		cv::Mat debugImg1;
		eimg1.copyTo(debugImg1);

		std::cout << "  {\n   \"id\":" << i << ",\n";
		fout << "  {\n   \"id\":" << i << ",\n";
		std::cout << "   \"name\":\"" << files1.at(i) << "\",\n";
		fout << "   \"name\":\"" << files1.at(i) << "\",\n";

		// Detect
		if (!stasm_search_single(&(face_num1[i]), face_marks77_1.at(i).marks,
			(char*)gray1.data, gray1.cols, gray1.rows, "../logs", exe_path_str.c_str()))
		{
			printf("Error in stasm_search_single: %s\n", stasm_lasterr());
			exit(1);
		}
		std::cout << "   \"exist\":" << face_num1[i] << ",\n";
		fout << "   \"exist\":" << face_num1[i] << ",\n";
		{
			std::cout << "   \"marks\":[";
			for (int ii = 0; ii < stasm_NLANDMARKS-1; ii++)
			{
				std::cout << "[" << face_marks77_1.at(i).marks[ii*2] << "," << face_marks77_1.at(i).marks[ii * 2+1] << "],";
			}
			std::cout << "[" << face_marks77_1.at(i).marks[(stasm_NLANDMARKS-1) * 2] << "," << face_marks77_1.at(i).marks[(stasm_NLANDMARKS-1) * 2 + 1] << "]],\n";
			
			fout << "   \"marks\":[";
			for (int ii = 0; ii < stasm_NLANDMARKS-1; ii++)
			{
				fout << "[" << face_marks77_1.at(i).marks[ii * 2] << "," << face_marks77_1.at(i).marks[ii * 2 + 1] << "],";
			}
			fout << "[" << face_marks77_1.at(i).marks[(stasm_NLANDMARKS-1) * 2] << "," << face_marks77_1.at(i).marks[(stasm_NLANDMARKS-1) * 2 + 1] << "]],\n";
			
			if (face_num1[i]<1)
				std::cout << "No face found in " << files1.at(i) << std::endl;
			else
			{
				stasm_force_points_into_image(face_marks77_1.at(i).marks, debugImg1.cols, debugImg1.rows);
				for (int ik = 0; ik < stasm_NLANDMARKS; ik++)
				{
					cv::drawMarker(debugImg1, cv::Point(face_marks77_1.at(i).marks[ik * 2], face_marks77_1.at(i).marks[ik * 2 + 1]),
						CV_RGB(255, 0, 0), cv::MARKER_CROSS, 2, 1);
					//cv::circle(debugImg1, cv::Point(face_marks77_1.at(i).marks[ik * 2 ], face_marks77_1.at(i).marks[ik * 2 + 1]), 4,
					//	CV_RGB(255, 0, 0), 1);
				}
			}

			if (showImg)
			{
				cv::imshow("source image", debugImg1);
				cv::waitKey(1);
			}

			// Corp
			std::string corpfilename1 = path1 + "\\corp\\" + files1.at(i);
			cv::imwrite(corpfilename1, debugImg1);
			std::cout << "   \"face\":\"corp\\\\" << files1.at(i) << "\"\n}";
			fout << "   \"face\":\"corp\\\\" << files1.at(i) << "\"\n}";

			if (i == files1.size() - 1)
			{
				std::cout << "\n ],\n";
				fout << "\n ],\n";
			}
			else
			{
				std::cout << ",\n";
				fout << ",\n";
			}
		}
	}

	char path2_chars[512];
	tools::path_to_jsonpath(path2.c_str(), path2_chars, 512);
	std::cout << " \"path2\":\"" << path2_chars << "\",\n";
	fout << " \"path2\":\"" << path2_chars << "\",\n";
	std::cout << " \"files2\":[\n";
	fout << " \"files2\":[\n";

	// Detect face
	for (int i = 0; i < files2.size(); i++)
	{
		// Read
		std::string filename2 = path2 + "\\" + files2.at(i);
		cv::Mat img2 = cv::imread(filename2, cv::IMREAD_COLOR);
		cv::Mat gray2;
		cv::cvtColor(img2, gray2, CV_RGB2GRAY);
		cv::Mat debugImg2;
		img2.copyTo(debugImg2);

		std::cout << "  {\n   \"id\":" << i << ",\n";
		fout << "  {\n   \"id\":" << i << ",\n";
		std::cout << "   \"name\":\"" << files2.at(i) << "\",\n";
		fout << "   \"name\":\"" << files2.at(i) << "\",\n";

		// Detect
		if (!stasm_search_single(&(face_num2[i]), face_marks77_2.at(i).marks,
			(char*)gray2.data, gray2.cols, gray2.rows, "../logs", exe_path_str.c_str()))
		{
			printf("Error in stasm_search_single: %s\n", stasm_lasterr());
			exit(1);
		}
		std::cout << "   \"exist\":" << face_num2[i] << ",\n";
		fout << "   \"exist\":" << face_num2[i] << ",\n";
		{
			std::cout << "   \"marks\":[";
			for (int ii = 0; ii < stasm_NLANDMARKS - 1; ii++)
			{
				std::cout << "[" << face_marks77_2.at(i).marks[ii * 2] << "," << face_marks77_2.at(i).marks[ii * 2 + 1] << "],";
			}
			std::cout << "[" << face_marks77_2.at(i).marks[(stasm_NLANDMARKS - 1) * 2] << "," << face_marks77_2.at(i).marks[(stasm_NLANDMARKS - 1) * 2 + 1] << "]],\n";

			fout << "   \"marks\":[";
			for (int ii = 0; ii < stasm_NLANDMARKS - 1; ii++)
			{
				fout << "[" << face_marks77_2.at(i).marks[ii * 2] << "," << face_marks77_2.at(i).marks[ii * 2 + 1] << "],";
			}
			fout << "[" << face_marks77_2.at(i).marks[(stasm_NLANDMARKS - 1) * 2] << "," << face_marks77_2.at(i).marks[(stasm_NLANDMARKS - 1) * 2 + 1] << "]],\n";

			if (face_num2[i]<1)
				std::cout << "No face found in " << files2.at(i) << std::endl;
			else
			{
				stasm_force_points_into_image(face_marks77_2.at(i).marks, debugImg2.cols, debugImg2.rows);
				for (int ik = 0; ik < stasm_NLANDMARKS; ik++)
				{
					cv::drawMarker(debugImg2, cv::Point(face_marks77_2.at(i).marks[ik * 2], face_marks77_2.at(i).marks[ik * 2 + 1]),
						CV_RGB(255, 0, 0), cv::MARKER_CROSS, 2, 1);
					//cv::circle(debugImg2, cv::Point(face_marks77_2.at(i).marks[ik * 2], face_marks77_2.at(i).marks[ik * 2 + 1]), 1,
					//	CV_RGB(255, 0, 0), 1);
				}
			}

			if (showImg)
			{
				cv::imshow("source image", debugImg2);
				cv::waitKey(1);
			}

			// Corp
			std::string corpfilename2 = path2 + "\\corp\\" + files2.at(i);
			cv::imwrite(corpfilename2, debugImg2);
			std::cout << "   \"face\":\"corp\\\\" << files2.at(i) << "\"\n}";
			fout << "   \"face\":\"corp\\\\" << files2.at(i) << "\"\n}";

			if (i == files2.size() - 1)
			{
				std::cout << "\n ],\n";
				fout << "\n ],\n";
			}
			else
			{
				std::cout << ",\n";
				fout << ",\n";
			}
		}
	}

	std::cout << " \"compare_matrix\":[\n";
	fout << " \"compare_matrix\":[\n";
	// Compare face
	//for (int i = 0; i < files1.size(); i++)
	//{
	//	std::cout << "  [";
	//	fout << "  [";
	//	for (int j = 0; j < files2.size(); j++)
	//	{
	//		compare_mat[i][j] = -1;
	//		if (face_num1[i]>0 && face_num2[j]>0)
	//		{
	//			compare_mat[i][j] = face_detector.compare(face_feas1[i], face_feas2[j]);
	//		}
	//		std::cout << compare_mat[i][j];
	//		fout << compare_mat[i][j];
	//		if (j != files2.size() - 1)
	//		{
	//			std::cout << ",";
	//			fout << ",";
	//		}
	//	}
	//	if (i != files1.size() - 1)
	//	{
	//		std::cout << "],\n";
	//		fout << "],\n";
	//	}
	//	else
	//	{
	//		std::cout << "]";
	//		fout << "]";
	//	}
	//}
	std::cout << "]\n";
	fout << "]\n";
	std::cout << "}";
	fout << "}";

	delete[] face_num1;
	for (int i = 0; i < files1.size(); i++)
	{
		delete[] face_feas1[i];
	}
	delete[] face_feas1;
	for (int i = 0; i < files2.size(); i++)
	{
		delete[] face_feas2[i];
	}
	delete[] face_num2;
	delete[] face_feas2;
	for (int i = 0; i < files1.size(); i++)
	{
		delete[] compare_mat[i];
	}
	delete[] compare_mat;

	if (fout.is_open())
	{
		fout.close();
	}
	return 0;
}