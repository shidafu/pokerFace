// Server.cpp : 定义控制台应用程序的入口点。
//

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/base64/base64.hpp>

//#include "cv.h"
#include <cv.hpp>
#include "image_tools.hpp"
#include "face_tools.hpp"
#include "common_tools.hpp"

#include "stdafx.h"
#include "json.h"
#include <iostream>

//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"
#include <boost/timer.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include <thread>
typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// pull out the type of messages sent by our config

// "requestType" = "askFace"
// "recognitionType" = "inOne"-"inTwo"
// "pic1Data" = ""
// "pic2Data" = ""

//replay
//"requestType" = "replyFace"
// "sourceHDL" = "0000asdff234"
// "resultState" = "ok"
// "resultValue" = "0.56"



cv::Mat base64toMat(std::string base64str)
{
	std::string mstr1;
	cv::Mat m;
	size_t Offt = base64str.find("/9j/");
	if (Offt < 0)
	{
		std::cout << "Not a Base64Image!" << std::endl;
		return m;
	}
	const char* Pos0 = base64str.c_str() + Offt;
	std::string mstr2 = Pos0;
	try
	{
		//RelicHelper::Base64Decode(mmsg2, &mstr1);
		mstr1 = websocketpp::base64_decode(mstr2);
		std::cout << "Base64Decode over!" << std::endl;
	}
	catch (...)
	{
		std::cout << "Base64Decode error!" << std::endl;
		return m;
	}
	int len = mstr1.length();
	if (len == 0) return m;
	std::vector<u_char> buff;
	for (int i = 0; i < len; i++)
	{
		buff.push_back(mstr1[i]);
	}
	try
	{
		m = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
		std::cout << "imdecode over!" << std::endl;
	}
	catch (...)
	{
		std::cout << "imdecode error!" << std::endl;
		return m;
	}
	return m;
}
cv::Mat readImgFile(std::string filePath)
{
	cv::Mat m;
	try {
		m = cv::imread(filePath);
	}
	catch(cv::Exception e)
	{
		std::cout << "读取文件出错！";
		std::cout << "文件路径：" << filePath << std::endl;
	}
	return m;
}

websocketpp::connection_hdl computServerHDL;
std::vector<websocketpp::connection_hdl> clientHDLvec;


tools::faceDetector face_detector;
//tools::faceCompare face_compare;

void on_message(server* c, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::string mmsg = msg->get_payload();
	//std::cout << mmsg<<std::endl;
	Json::Value root;
	std::stringstream ssm;
	std::string requestTypeStr;
	try
	{
		ssm.str("");
		ssm << mmsg;
		ssm >> root;
		std::cout << "get a json data!" << std::endl;
		//std::cout << root << std::endl;
		requestTypeStr = root.get("requestType", "").asString();
	}
	catch (...)
	{
		std::cout << "decode jason data error!" << std::endl;
		std::cout << mmsg << std::endl;
		return;
	}
	
	Json::Value Jsvalue;
	Jsvalue["requestType"] = "replyFace";

	if (requestTypeStr == "invalid") return;
	std::string tmpstr;
	if (requestTypeStr == "askFace")
	{

		tmpstr = root.get("recognitionType", "invalid").asString();
		if (tmpstr == "inTwo")
		{
			std::string pic1str = root.get("pic1Data", "").asString();
			std::string pic2str = root.get("pic2Data", "").asString();
			tmpstr = root.get("imgType", "invalid").asString();

			cv::Mat img1;
			cv::Mat img2;
			if (tmpstr=="address")
			{
				img1 = readImgFile(pic1str);
				img2 = readImgFile(pic2str);
			}
			else if(tmpstr == "data")
			{
				img1 = base64toMat(pic1str);
				img2 = base64toMat(pic2str);
			}
			//cv::imshow("img1", img1);
			//cv::imshow("img2", img2);
			//cv::waitKey(1);
			if (img1.empty()|img2.empty())
			{
				Jsvalue["resultState"] = "error";
			}
			else
			{
				try
				{
					float r = 0.01;
					//算法部分


					std::cout << "进行算法运算" << std::endl;

					//Load images
					seeta::ImageData img_data1(img1.cols, img1.rows, img1.channels());
					img_data1.data = img1.data;
					cv::Mat gray_img1;
					cv::cvtColor(img1, gray_img1, CV_RGB2GRAY);
					seeta::ImageData gray_img_data1(gray_img1.cols, gray_img1.rows, gray_img1.channels());
					gray_img_data1.data = gray_img1.data;
					std::cout << "Load image1 width: " << img1.cols << " ,height: " << img1.rows << std::endl;

					seeta::ImageData img_data2(img2.cols, img2.rows, img2.channels());
					img_data2.data = img2.data;
					cv::Mat gray_img2;
					cv::cvtColor(img2, gray_img2, CV_RGB2GRAY);
					seeta::ImageData gray_img_data2(gray_img2.cols, gray_img2.rows, gray_img2.channels());
					gray_img_data2.data = gray_img2.data;
					std::cout << "Load image2 width: " << img2.cols << " ,height: " << img2.rows << std::endl;

					// Detect face.
					std::vector <seeta::FaceInfo> face_info1;
					std::vector <tools::faceLandMark5> face_marks1;
					long t0 = cv::getTickCount();
					bool isOK = face_detector.detect(gray_img_data1, face_info1, face_marks1);
					long t1 = cv::getTickCount();
					double secs = (t1 - t0) / cv::getTickFrequency();
					std::cout << "Detect: " << face_info1.size() << " face,take " << secs << " seconds." << std::endl;

					std::vector <seeta::FaceInfo> face_info2;
					std::vector <tools::faceLandMark5> face_marks2;
					t0 = cv::getTickCount();
					isOK = face_detector.detect(gray_img_data2, face_info2, face_marks2);
					t1 = cv::getTickCount();
					secs = (t1 - t0) / cv::getTickFrequency();
					std::cout << "Detect: " << face_info2.size() << " face,take " << secs << " seconds." << std::endl;

					if (face_marks1.size() > 0 && face_marks2.size() > 0)
					{
						t0 = cv::getTickCount();
						r = face_detector.corp_compare(img_data1, img_data2, face_marks1.at(0).mark, face_marks2.at(0).mark);
						t1 = cv::getTickCount();
						secs = (t1 - t0) / cv::getTickFrequency();
						std::cout << "Compare: similar=" << r << " ,take " << secs << " seconds." << std::endl;
					}


					//算法部分结束
					Jsvalue["resultState"] = "ok";
					Jsvalue["resultValue"] = r;
				}
				catch (...)
				{
					Jsvalue["resultState"] = "error";
				}

			}
			Jsvalue["sourceHDL"] = root.get("sourceHDL", "").asString();

			//algorithm
			
			std::stringstream ssm;
			ssm << Jsvalue;
			std::string buffstr =ssm.str();
			try {
				c->send(hdl, buffstr/*msg->get_payload()*/, websocketpp::frame::opcode::text/*msg->get_opcode()*/);
			}
			catch (const websocketpp::lib::error_code& e) {
				std::cout << "Echo failed because: " << e
					<< "(" << e.message() << ")" << std::endl;
			}

		}
	}
	




}
void on_http(server * s, websocketpp::connection_hdl hdl) {
	server::connection_ptr con = s->get_con_from_hdl(hdl);

	con->set_body("Hello World! Connection is http:");
	con->set_status(websocketpp::http::status_code::ok);
}
int main(int argc, char* argv[])
{
	//Load faceTool.    server echo_server;
	std::string exe_fullpath_str = argv[0];
	char exe_path[512];
	tools::full_to_path(argv[0], exe_path, 512);
	std::string exe_path_str = exe_path;
	face_detector.initial(tools::accept_size_3X2,
		(exe_path_str + "seeta_fd_frontal_v1.0.bin").c_str(),
		(exe_path_str + "seeta_fa_v1.1.bin").c_str(),
		(exe_path_str + "seeta_fr_v1.0.bin").c_str());
	//tools::faceCompare face_compare;
	//face_compare.initial(t


	server wsServer;

	try {
		// Set logging settings
		wsServer.set_access_channels(websocketpp::log::alevel::all);
		wsServer.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize Asio
		wsServer.init_asio();

		// Register our message handler
		wsServer.set_message_handler(bind(&on_message, &wsServer, ::_1, ::_2));
		wsServer.set_http_handler(bind(&on_http, &wsServer, ::_1));
		//echo_server
		// Listen on port 9002
		int port = 9003;
		wsServer.listen(port);
		std::cout << "Listening Port:" << port << std::endl;
		// Start the server accept loop
		wsServer.start_accept();

		// Start the ASIO io_service run loop
		wsServer.run();
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "other exception" << std::endl;
	}

	return 0;
}

