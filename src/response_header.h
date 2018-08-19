#pragma once
#include <string>
#include <map>
#include <boost/format.hpp>

namespace pserver
{

	class ResponseHeader
	{
	public:
		ResponseHeader(int status_code, int content_length, std::string content_type);
		~ResponseHeader();
		std::string&to_m_raw_headers();

	private:
		std::string m_response_line;
		std::string m_raw_headers;
		std::map<std::string, std::string> m_headers;


	};

}

