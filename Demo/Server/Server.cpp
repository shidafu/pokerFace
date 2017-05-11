// Server.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "uWS.h"
#include "cv.h"
#include "iostream"
int main()
{
	uWS::Hub h;
	uWS::Group<uWS::SERVER> *group = h.createGroup<uWS::SERVER>(uWS::PERMESSAGE_DEFLATE);

	group->onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {

		std::cout << "message:" << message << std::endl;
		std::cout << "opCode" << opCode << std::endl;
		ws->send(message, length, opCode);

	});
	cv::Mat m;
	h.listen(3000);
	h.run();
	

    return 0;
}

