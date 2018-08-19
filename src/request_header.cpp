
#include "request_header.h"
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace pserver;

RequestHeader::RequestHeader()
{

}

bool RequestHeader::read_over(char c)
{
	m_raw_headers += c;

	if (m_raw_headers.length() == MAX_LENGTH)
	{
		return true;
	}
	else if ( boost::ends_with(m_raw_headers, "\r\n\r\n") )
	{
		return true;
	}

	return false;

}

bool RequestHeader::parse_request_line()
{
	if (m_raw_headers.length() == MAX_LENGTH)
	{
		return false;
	}

	int index = m_raw_headers.find("\r\n");
	if (std::string::npos == index)
	{
		return false;
	}
	m_request_line = m_raw_headers.substr(0, index);

	std::vector<std::string> strs;
	boost::split(strs, m_request_line, boost::is_space());

	if (strs.size() != 3)
	{
		return false;
	}
	m_path = strs[1];

	return true;
}

std::string&RequestHeader::get_path()
{
	return m_path;
}

RequestHeader::~RequestHeader()
{
}
