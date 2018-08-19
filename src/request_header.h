#pragma once

#include <map>
#include <string>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>

namespace pserver
{
	class RequestHeader
	{
	public:
		RequestHeader();
		~RequestHeader();
		bool read_over(char c);
		bool parse_request_line();
		std::string&get_path();
		

	private:
		std::string m_request_line;
		std::string m_path;
		std::map<std::string, std::string> m_headers;
		std::string m_raw_headers;
		static const int MAX_LENGTH = 2048;

	};

}


