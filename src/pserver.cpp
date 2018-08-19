
#include "pserver.h"
#include "session.h"
#include "response_header.h"
#include <iostream>
#include <fstream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>

using namespace pserver;

PServer::PServer(boost::asio::io_service&io_svc) :m_io_svc(io_svc),
m_acceptor(io_svc, tcp::endpoint(tcp::v4(), (8888))), m_max_len(1024), m_root("./www/")
{
	status_map[200] = "OK";
	status_map[404] = "NOT FOUND";

	type_map[".ico"] = "image/x-icon";
	type_map[".html"] = "text/html";

	std::fstream fs;
	for (boost::filesystem::recursive_directory_iterator dit(m_root); dit !=
		boost::filesystem::recursive_directory_iterator(); ++dit){
		boost::filesystem::path p = *dit;
		if (is_regular_file(p)){

			std::string str;
			fs.open(p.string(), std::ios::in);
			while (!fs.eof())
			{
				fs.read(m_data, m_max_len);
				str.append(m_data, fs.gcount());
			}
			fs.close();
			
			std::string file_path = p.relative_path().string();
			file_path = file_path.replace(0, m_root.string().length(), "/");
			Resource*prc = new Resource();
			prc->m_file_content = str;
			prc->m_file_type = p.extension().string();
			m_file_map[file_path] = boost::shared_ptr<Resource>(prc);
			
		}
		
	}

	
}

void PServer::accept()
{
	boost::shared_ptr<Session> session(new Session);
	session->m_socket = boost::shared_ptr<tcp::socket>(new tcp::socket(m_io_svc));

	m_acceptor.async_accept(*(session->m_socket),
		boost::bind(&PServer::accept_handler, this, session, _1));
}

void PServer::accept_handler(boost::shared_ptr<Session> session, boost::system::error_code ec)
{
	if (ec != 0)
	{
		std::cout << "accept error = " << ec << std::endl;
		return;
	}

	accept();

	std::cout << "client connect " << std::endl;

	session->m_socket->async_read_some(boost::asio::buffer(m_data, m_max_len),
		boost::bind(&PServer::read_handler, this, session, _1, _2));

}


void PServer::read_handler(boost::shared_ptr<Session> session, boost::system::error_code ec, const size_t len)
{
	if (ec != 0)
	{
		std::cout << "read error = " << ec << std::endl;
		session->m_socket->close();
		return;
	}

	bool read_header_over = false;
	for (int i = 0; i < len && !read_header_over; ++i)
	{
		switch (session->m_req_progress)
		{
		case PG_HEEADER_NULL:
			session->m_req_headers = boost::shared_ptr<RequestHeader>(new RequestHeader());
			session->m_req_progress = PG_HEADER_ING;
			session->m_req_headers->read_over(m_data[i]);
			break;
		case PG_HEADER_ING:
			if (session->m_req_headers->read_over(m_data[i]))
			{
				session->m_req_progress = PG_HEADER_OVER;
				read_header_over = true;
			}
			break;

		}
	}

	if (PG_HEADER_OVER == session->m_req_progress)
	{
		if (!session->m_req_headers->parse_request_line())
		{
			session->m_socket->close();
			return;
		}

		boost::shared_ptr<Resource> prc = m_file_map[session->m_req_headers->get_path()];
		

		if (prc == NULL)
		{
			ResponseHeader resp_header(404, 0,"text/plain");
			std::string resp_raw_headers = resp_header.to_m_raw_headers();

			session->m_socket->async_write_some(boost::asio::buffer(resp_raw_headers.c_str(),
				resp_raw_headers.length()),
				boost::bind(&PServer::write_handler, this, session, _1));
			session->m_req_progress = PG_SEND_OVER;
		}
		else
		{
			std::string file_content = prc->m_file_content;
			ResponseHeader resp_header(200, file_content.length(),type_map[prc->m_file_type]);
			std::string resp_raw_headers = resp_header.to_m_raw_headers();

			session->m_socket->async_write_some(boost::asio::buffer(resp_raw_headers.c_str(),
				resp_raw_headers.length()),
				boost::bind(&PServer::write_handler, this, session, _1));

			session->m_socket->async_write_some(boost::asio::buffer(file_content.c_str(), file_content.length()),
				boost::bind(&PServer::write_handler, this, session, _1));
			session->m_req_progress = PG_SEND_OVER;
		}



	}
	else
	{
		session->m_socket->async_read_some(boost::asio::buffer(m_data, m_max_len),
			boost::bind(&PServer::read_handler, this, session, _1, _2));
		std::cout << "async read some start" << std::endl;
	}


}

void PServer::write_handler(boost::shared_ptr<Session> session, boost::system::error_code ec)
{
	if (ec != 0)
	{
		std::cout << "write error" << std::endl;
		return;
	}
	if (PG_SEND_OVER == session->m_req_progress)
	{
		session->m_socket->close();
	}

}


PServer::~PServer(){}
