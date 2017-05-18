// Server.cpp : 定义控制台应用程序的入口点。
//

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/base64/base64.hpp>
#include <websocketpp/config/asio.hpp>

#include "cv.h"
#include "stdafx.h"
#include "json.h"
#include <iostream>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <boost/timer.hpp>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

typedef websocketpp::server<websocketpp::config::asio_tls> server_tls;
typedef websocketpp::server<websocketpp::config::asio> server_plain;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio::message_type::ptr message_ptr;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

// "requestType" = "askFace"
// "recognitionType" = "inTwo"
// "imgType" = "address" / "data"
// "pic1Data" = ""
// "pic2Data" = ""

//replay
//"requestType" = "replyFace"
// "sourceHDL" = "0000asdff234"
// "resultState" = "ok"
// "resultValue" = "0.56"


websocketpp::connection_hdl computServerHDL;
std::vector<websocketpp::connection_hdl> clientHDLvec;

server_tls * pServer_tls;
server_plain *pServer_plain;

template <typename EndpointType>
void on_message(EndpointType* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	std::cout << "get into on_message" << std::endl;
	std::string mmsg = msg->get_payload();
	Json::Value root;
	std::stringstream ssm;
	try
	{
		ssm.str("");
		ssm << mmsg;
		ssm >> root;
		std::cout << "get a json data!" << std::endl;


	}
	catch (...)
	{
		std::cout << "decode jason data error!" << std::endl;
		return;
	}
	
	std::string requestTypeStr = root.get("requestType", "").asString();
	if (requestTypeStr == "invalid")
	{
		std::cout << "数据格式不正确" << std::endl;
		return;
	}
	std::string tmpstr;
	if (requestTypeStr == std::string("askFace"))
	{
		ssm.str("");
		ssm << hdl.lock().get();

		root["sourceHDL"] = ssm.str();
		std::string buffstr;
		ssm.str("");
		ssm << root;
		buffstr = ssm.str();
		//std::cout << "数据内容：" << buffstr << std::endl;
		try {
			pServer_plain->send(computServerHDL, buffstr/*msg->get_payload()*/, websocketpp::frame::opcode::text/*msg->get_opcode()*/);
		}
		catch (const websocketpp::lib::error_code& e) {
			std::cout << "sent to compute server fail, because: " << e
				<< "(" << e.message() << ")" << std::endl;
		}
		clientHDLvec.push_back(hdl);
		//tmpstr = root.get("recognitionType", "invalid").asString();
		//if (tmpstr == "inTwo")
		//{
		//	std::string pic1str = root.get("pic1Data", "").asString();
		//	std::string pic2str = root.get("pic2Data", "").asString();
		//	cv::Mat mat1 = base64toMat(pic1str);
		//	cv::Mat mat2 = base64toMat(pic2str);
		//}
	}
	else if (requestTypeStr == std::string("replyFace"))
	{
		//std::string sourceHDLstr = root.get("sourceHDL", "").asString();
		//ssm.str("");
		//ssm << root.get("sourceHDL", "");
		std::string sourceHDLstr = root.get("sourceHDL", "").asString();

		for (auto it = clientHDLvec.begin(); it != clientHDLvec.end(); it++)
		{
			ssm.str("");
			ssm << it->lock().get();
			std::string tmp = ssm.str();
			if (sourceHDLstr == tmp)
			{
				try {
					ssm.str("");
					ssm << root;
					std::string buffstr=ssm.str();
					std::cout << "发送给客户端" << std::endl;
					pServer_tls->send(*it, buffstr/*msg->get_payload()*/, websocketpp::frame::opcode::text/*msg->get_opcode()*/);
				}
				catch (const websocketpp::lib::error_code& e) {
					std::cout << "sent to client server fail, because: " << e
						<< "(" << e.message() << ")" << std::endl;
				}
				clientHDLvec.erase(it);
				break;
			}
	
		}

	}
	else if (requestTypeStr == std::string("addServer"))
	{
		computServerHDL = hdl;
		std::cout << "添加了一个计算服务" << std::endl;
		//toServer = (void*)s;
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
template <typename EndpointType>
void on_http(EndpointType* s, websocketpp::connection_hdl hdl) {
	EndpointType::connection_ptr con = s->get_con_from_hdl(hdl);
	if (s->is_secure())
	{
		con->set_body("Hello World!\nThe connection is secure!");
	}
	else
	{
		con->set_body("Hello World!");
	}
	
	con->set_status(websocketpp::http::status_code::ok);
}

std::string get_password() {
	return "214099680310031";//"test"
}

// See https://wiki.mozilla.org/Security/Server_Side_TLS for more details about
// the TLS modes. The code below demonstrates how to implement both the modern
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
			ctx->set_options( asio::ssl::context::default_workarounds /*|
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
template <typename EndpointType>
void onDisconnect(EndpointType* s, websocketpp::connection_hdl hdl)
{
	std::cout << "disConnnect:" << hdl.lock().get() << std::endl;

}
int main()
{
	boost::asio::io_service ios;
	server_tls endPoint_tls;
	// Initialize ASIO
	endPoint_tls.init_asio(&ios);
	//endPoint_tls.is_secure();
	// Register our message handler
	endPoint_tls.set_message_handler(
		bind(&on_message<server_tls>, &endPoint_tls, ::_1, ::_2));
	endPoint_tls.set_http_handler(
		bind(&on_http<server_tls>, &endPoint_tls, ::_1));
	endPoint_tls.set_tls_init_handler(
		bind(&on_tls_init, MOZILLA_INTERMEDIATE, ::_1));
	endPoint_tls.set_close_handler(
		bind(&onDisconnect<server_tls>, &endPoint_tls, ::_1));
	// Listen on port 9002
	endPoint_tls.listen(443);
	endPoint_tls.start_accept();
	pServer_tls = &endPoint_tls;
	//pServer_tls->



	server_plain endPoint_plain;
	endPoint_plain.init_asio(&ios);
	endPoint_plain.set_message_handler(
		bind(&on_message<server_plain>, &endPoint_plain, ::_1, ::_2));
	endPoint_plain.set_http_handler(
		bind(&on_http<server_plain>, &endPoint_plain, ::_1));
	endPoint_plain.listen(9003);
	endPoint_plain.start_accept();
	pServer_plain = &endPoint_plain;
	// Start the ASIO io_service run loop
	ios.run();

    return 0;
}

