
#include "session.h"
#include <iostream>

using namespace pserver;

Session::Session() : m_req_progress(PG_HEEADER_NULL)
{
}

Session::~Session()
{	
	std::cout << "session destory" << std::endl;
}
