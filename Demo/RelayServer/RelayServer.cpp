// Server.cpp : 定义控制台应用程序的入口点。
//


#include "stdafx.h"
#include "uWS.h"
#include "json.h"
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
	Json::Value root;
	root["item1"] = "value1";
	root["item2"] = "value2";
	std::cout << root << std::endl;


	h.listen(3000);
	h.run();
	




    return 0;
}

