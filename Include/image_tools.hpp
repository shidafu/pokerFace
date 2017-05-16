/**
@file image_tools.hpp
@brief 
@author Leon Contact: towanglei@163.com
@version 1.0
@par History:
      1.0 : Leon 2017/05/11 18:48 build
*/
#pragma once
#ifndef IMAGE_TOOLS
#define IMAGE_TOOLS
#include <cv.hpp>
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
	//sizeSetting accept_size_3X2 = { 80,120,20 };
	//sizeSetting accept_size_2X3 = { 120,80,20 };
	//sizeSetting accept_size_4X3 = { 120,160,30 };
	//sizeSetting accept_size_3X4 = { 160,120,30 };
	//sizeSetting accept_size_15X9 = { 120,200,30 };
	//sizeSetting accept_size_9X15 = { 200,120,30 };
	//sizeSetting accept_size_16X9 = { 180,270,40 };
	//sizeSetting accept_size_9X16 = { 270,180,40 };
#endif
	/**
	@fn  corp_size
	@brief corp origin image to size
	@param[in/out]  inImg
	@param[out]  outImg
	*/
	bool corp_size(cv::Mat& inImg,cv::Mat& outImg, sizeSetting accept_size)
	{
		try
		{
			int in_width = inImg.cols;
			int in_height = inImg.rows;
			double in_ratio = (double)in_height / (double)in_width;
			double out_ratio = (double)accept_size.img_height / (double)accept_size.img_width;
			if (in_ratio>out_ratio)
			{
				int in_height_fix = (int)in_width * out_ratio;
				int in_top = (in_height - in_height_fix) / 2;
				cv::Rect cent_rect;
				cent_rect.x = 1;
				cent_rect.y = in_top+1;
				cent_rect.width = in_width-2;
				cent_rect.height = in_height_fix-2;
				cv::rectangle(inImg, cent_rect, CV_RGB(0, 255, 255), 2, 8, 0);
				cv::Mat tmpImg;
				tmpImg = inImg(cv::Range(in_top, in_top + in_height_fix), cv::Range(0, in_width));
				cv::Size dsize = cv::Size(accept_size.img_width, accept_size.img_height);
				outImg = cv::Mat(dsize, CV_32S);
				cv::resize(tmpImg, outImg, dsize);
			} 
			else
			{
				int in_width_fix = (int)in_height/out_ratio;
				int in_left = (in_width - in_width_fix) / 2;
				cv::Rect cent_rect;
				cent_rect.x = in_left + 1;
				cent_rect.y = 1;
				cent_rect.width = in_width_fix - 2;
				cent_rect.height = in_height - 2;
				cv::rectangle(inImg, cent_rect, CV_RGB(0, 255, 255), 2, 8, 0);
				cv::Mat tmpImg;
				tmpImg = inImg(cv::Range(0, in_height), cv::Range(in_left, in_left + in_width_fix));
				cv::Size dsize = cv::Size(accept_size.img_width, accept_size.img_height);
				outImg = cv::Mat(dsize, CV_32S);
				cv::resize(tmpImg, outImg, dsize);
			}
			return true;
		}
		catch (std::exception* e)
		{
			return false;
		}
		catch (...)
		{
			return false;
		}
	}
}
#endif
