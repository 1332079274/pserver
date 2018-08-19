
#include <cstdio>
#include "response_header.h"
#include "pserver.h"

using namespace pserver;

extern boost::shared_ptr<PServer> ptr_server;

ResponseHeader::ResponseHeader(int status_code, int content_length, std::string content_type)
{
	char buf[64];
	sprintf(buf, "%d %s", status_code, ptr_server->status_map[status_code].c_str());
	
	m_response_line.append("HTTP/1.1 ");
	m_response_line.append(buf);
	m_response_line.append("\r\n");
	
	m_headers["Server"] = "pserver 1.0b";
	sprintf(buf,"%d",content_length);
	m_headers["Content-Length"] = buf;
	m_headers["Content-Type"] = content_type;
}

std::string&ResponseHeader::to_m_raw_headers()
{
	m_raw_headers += m_response_line;
	std::map<std::string, std::string>::iterator it = m_headers.begin();
	while (it != m_headers.end())
	{
		m_raw_headers += it->first + ": " + it->second + "\r\n";
		++it;
	}
	m_raw_headers += "\r\n";

	return m_raw_headers;
}


ResponseHeader::~ResponseHeader()
{
}
