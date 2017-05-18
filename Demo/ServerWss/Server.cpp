// Server.cpp : 定义控制台应用程序的入口点。
//

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp//client.hpp>
#include <websocketpp/base64/base64.hpp>
#include <websocketpp/config/asio.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

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

typedef websocketpp::server<websocketpp::config::asio_tls> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio::message_type::ptr message_ptr;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

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
	//std::cout << mmsg << std::endl;
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
			else if(tmpstr=="data")
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
	

	//int len = mstr1.length();
	//if (len == 0)
	//{
	//	int a = 1;
	//}
	////if (len<1000)
	////{
	////	std::cout << "Too SHort !" << std::endl;
	////	return;
	////}
	//std::cout << "Length: " << len << std::endl;
	//byte* imgbuffer = new byte[len];
	//for (int i = 0; i < len; i++)
	//{
	//	buff.push_back(mstr1[i]);
	//}
	////memcpy(imgbuffer, mstr.data(), len);
	//cv::Mat mMat;
	//try
	//{
	//	mMat = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
	//	std::cout << "imdecode over!" << std::endl;
	//}
	//catch (...)
	//{
	//	std::cout << "imdecode error!" << std::endl;
	//	return;
	//}

	//int imagesize = len / 3;
	//Mat mMat(2, &imagesize, CV_8UC3, imgbuffer);
	//CImage mimg(imgbuffer,len, CXIMAGE_FORMAT_JPG)
	/*	try
	{
	imshow("接收", mMat);
	waitKey();
	}
	catch (...)
	{
	std::cout << "image error!" << std::endl;
	return;
	}*/



}
enum tls_mode {
	MOZILLA_INTERMEDIATE = 1,
	MOZILLA_MODERN = 2
};

context_ptr on_tls_init(tls_mode mode, websocketpp::connection_hdl hdl) {
	namespace asio = websocketpp::lib::asio;

	std::cout << "on_tls_init called with hdl: " << hdl.lock().get() << std::endl;
	std::cout << "using TLS mode: " << (mode == MOZILLA_MODERN ? "Mozilla Modern" : "Mozilla Intermediate") << std::endl;

	context_ptr ctx = websocketpp::lib::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);

	try {
		if (mode == MOZILLA_MODERN) {
			// Modern disables TLSv1
			ctx->set_options(asio::ssl::context::default_workarounds /*|
																	 asio::ssl::context::no_sslv2 |
																	 asio::ssl::context::no_sslv3 */
																	 //asio::ssl::context::tlsv1 |
																	 //asio::ssl::context::tlsv11 |
																	 //asio::ssl::context::tlsv12 /*|
																	 //asio::ssl::context::single_dh_use*/
				);
		}
		else {
			ctx->set_options(asio::ssl::context::default_workarounds/* |
																	asio::ssl::context::no_sslv2 |
																	asio::ssl::context::no_sslv3 */
																	//asio::ssl::context::tlsv1 |
																	//asio::ssl::context::tlsv11 |
																	//asio::ssl::context::tlsv12 /*|
																	//asio::ssl::context::single_dh_use*/
				);
		}
		//ctx->set_password_callback(bind(&get_password));
		//ctx->use_certificate_chain_file("server.pem");
		//ctx->use_private_key_file("server.pem", asio::ssl::context::pem);
		//ctx->us
		//ctx->set_default_verify_paths();
		ctx->use_certificate_file("key/public.pem", asio::ssl::context::pem);
		ctx->use_private_key_file("key/214099680310031.key", asio::ssl::context::pem);
		//ctx->use_certificate_chain_file("key/chain.pem");
		//ctx->use_certificate_file("key/214099680310031.pem", asio::ssl::context::pem);

		// Example method of generating this file:
		// `openssl dhparam -out dh.pem 2048:
		// Mozilla Intermediate suggests 1024 as the minimum size to use:
		// Mozilla Modern suggests 2048 as the minimum size to use:
		//ctx->use_tmp_dh_file("dh.pem");
		//ctx->set_options();
		std::string ciphers;

		if (mode == MOZILLA_MODERN) {
			ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK";

		}
		else {
			ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA256:AES256-SHA256:AES128-SHA:AES256-SHA:AES:CAMELLIA:DES-CBC3-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:!EDH-DSS-DES-CBC3-SHA:!EDH-RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA";
		}

		if (SSL_CTX_set_cipher_list(ctx->native_handle(), ciphers.c_str()) != 1) {
			std::cout << "Error setting cipher list" << std::endl;
		}
	}
	catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	return ctx;
}
void on_http(server * s, websocketpp::connection_hdl hdl) {
	server::connection_ptr con = s->get_con_from_hdl(hdl);

	con->set_body("Hello World!Connection is https: ");
	con->set_status(websocketpp::http::status_code::ok);
}
int main(int argc, char* argv[])
{
	//Load faceTool.
	std::string exe_fullpath_str = argv[0];
	char exe_path[512];
	tools::full_to_path(argv[0], exe_path, 512);
	std::string exe_path_str = exe_path;
	face_detector.initial(tools::accept_size_3X2,
		(exe_path_str + "seeta_fd_frontal_v1.0.bin").c_str(),
		(exe_path_str + "seeta_fa_v1.1.bin").c_str(),
		(exe_path_str + "seeta_fr_v1.0.bin").c_str());
	//tools::faceCompare face_compare;
	//face_compare.initial(tools::accept_size_3X2);

	server wssServer;

	// Initialize ASIO
	wssServer.init_asio();



	wssServer.set_access_channels(websocketpp::log::alevel::all);
	wssServer.clear_access_channels(websocketpp::log::alevel::frame_payload);
	// Register our message handler
	wssServer.set_message_handler(bind(&on_message, &wssServer, ::_1, ::_2));
	wssServer.set_http_handler(bind(&on_http, &wssServer, ::_1));
	wssServer.set_tls_init_handler(bind(&on_tls_init, MOZILLA_INTERMEDIATE, ::_1));


	int port = 9002;
	wssServer.listen(port);
	std::cout << "Listening Port:" << port << std::endl;
	// Listen on port 9002
	//wssServer.listen(port);

	// Start the server accept loop
	wssServer.start_accept();

	// Start the ASIO io_service run loop
	wssServer.run(); //he ASIO io_service run loop

	return 0;
}

