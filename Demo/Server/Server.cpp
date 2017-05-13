// Server.cpp : 定义控制台应用程序的入口点。
//

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp//client.hpp>
#include <websocketpp/base64/base64.hpp>
#include <websocketpp/config/asio.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

#include "cv.h"
#include "stdafx.h"
#include "json.h"
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <boost/timer.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#include <thread>

typedef websocketpp::server<websocketpp::config::asio_tls> server_tls;
typedef websocketpp::server<websocketpp::config::asio> server_plain;
typedef websocketpp::client<websocketpp::config::asio_client> client;


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
	try
	{
		//RelicHelper::Base64Decode(mmsg2, &mstr1);
		mstr1 = websocketpp::base64_decode(base64str);
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


websocketpp::connection_hdl computServerHDL;
std::vector<websocketpp::connection_hdl> clientHDLvec;



void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::string mmsg = msg->get_payload();
	Json::Value root;
	std::stringstream ssm;
	std::string requestTypeStr;
	try
	{
		ssm.str("");
		ssm << mmsg;
		ssm >> root;
		std::cout << "get a json data!" << std::endl;
		std::cout << root << std::endl;
		requestTypeStr = root.get("requestType", "").asString();
	}
	catch (...)
	{
		std::cout << "decode jason data error!" << std::endl;
		return;
	}
	


	if (requestTypeStr == "invalid") return;
	std::string tmpstr;
	if (requestTypeStr == "askFace")
	{

		tmpstr = root.get("recognitionType", "invalid").asString();
		if (tmpstr == "inTwo")
		{
			std::string pic1str = root.get("pic1Data", "").asString();
			std::string pic2str = root.get("pic2Data", "").asString();
			cv::Mat mat1 = base64toMat(pic1str);
			cv::Mat mat2 = base64toMat(pic2str);
			Json::Value Jsvalue;
			Jsvalue["requestType"] = "replyFace";
			try
			{
				float r=0.01;
				//算法部分


				std::cout << "进行算法运算" << std::endl;
				//算法部分结束
				Jsvalue["resultState"] = "ok";
				Jsvalue["resultValue"] = r;
			}
			catch (...)
			{
				Jsvalue["resultState"] = "error";
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
void on_open(client* c, websocketpp::connection_hdl hdl)
{
	Json::Value root;
	root["requestType"] = "addServer";
	std::stringstream ssm;
	ssm << root;
	std::string strdata = ssm.str();
	try {
		c->send(hdl, strdata/*msg->get_payload()*/, websocketpp::frame::opcode::text/*msg->get_opcode()*/);
	}
	catch (const websocketpp::lib::error_code& e) {
		std::cout << "ask addServer failed because: " << e
			<< "(" << e.message() << ")" << std::endl;
	}
	std::cout << "ask add server ok!" << std::endl;
}
void connectRelayServer()
{
	client c;
	std::string uri = "ws://localhost:9003";

	try {

		// Set logging to be pretty verbose (everything except message payloads)

		c.set_access_channels(websocketpp::log::alevel::all);

		c.clear_access_channels(websocketpp::log::alevel::frame_payload);
		// Initialize ASIO
		c.init_asio();
		// Register our message handler
		c.set_message_handler(bind(&on_message, &c, ::_1, ::_2));
		c.set_open_handler(bind(&on_open, &c, ::_1));
		websocketpp::lib::error_code ec;

		client::connection_ptr con = c.get_connection(uri, ec);

		if (ec) {

			std::cout << "could not create connection because: " << ec.message() << std::endl;
			return ;
		}
		// Note that connect here only requests a connection. No network messages are
		// exchanged until the event loop starts running in the next line.
		c.connect(con);
		// Start the ASIO io_service run loop
		// this will cause a single connection to be made to the server. c.run()

		// will exit when this connection is closed.
		c.run();

	}
	catch (websocketpp::exception const & e) {

		std::cout << e.what() << std::endl;

	}

}



int main()
{
	int connetTryCuont = 0;
	while (true)
	{
		connetTryCuont++;
		std::cout << "\ntry connect to RelayServer: " << connetTryCuont << " times." << std::endl;
		connectRelayServer();

		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
	// Start the ASIO io_service run loop

	return 0;
}

