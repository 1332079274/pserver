#pragma once

#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/filesystem.hpp>
#include "session.h"
#include "resource.h"

using boost::asio::ip::tcp;

namespace pserver
{
	class PServer;

	class PServer
	{

	public:
		PServer(boost::asio::io_service&io_svc);
		~PServer();
		void accept();
		std::map<int, std::string> status_map;
		std::map<std::string, std::string> type_map;

	private:
		
		void accept_handler(boost::shared_ptr<Session> session, boost::system::error_code ec);
		void write_handler(boost::shared_ptr<Session> session, boost::system::error_code ec);
		void read_handler(boost::shared_ptr<Session> session, boost::system::error_code ec, size_t len);

	private:

		boost::asio::io_service&m_io_svc;
		tcp::acceptor m_acceptor;
		char m_data[1024];
		int m_max_len;
		boost::filesystem::path m_root;
		std::map<std::string, boost::shared_ptr<Resource> > m_file_map;

	};
}



