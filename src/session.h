#pragma once

#include <boost/asio.hpp>
#include <boost/smart_ptr.hpp>
#include "request_header.h"

using boost::asio::ip::tcp;

namespace pserver
{
	enum REQUEST_PROGRESS
	{
		PG_HEEADER_NULL,
		PG_HEADER_ING,
		PG_HEADER_OVER,
		PG_SEND_OVER
	};

	class Session
	{
	public:
		boost::shared_ptr<tcp::socket> m_socket;
		boost::shared_ptr<RequestHeader> m_req_headers;
		REQUEST_PROGRESS m_req_progress;


		Session();
		~Session();
	};
}




