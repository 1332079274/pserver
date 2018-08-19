// pserver.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include "pserver.h"

using namespace pserver;
boost::shared_ptr<PServer> ptr_server;

int main()
{
	boost::asio::io_service io_svc;
	ptr_server = boost::shared_ptr<PServer>(new PServer(io_svc));
	ptr_server->accept();
	io_svc.run();

	std::cout << "server terminate" << std::endl;

	getchar();
	return 0;
}

