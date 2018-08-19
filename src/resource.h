#pragma once

#include <string>

namespace pserver
{
	class Resource
	{
	public:
		Resource();
		~Resource();
		std::string m_file_content;
		std::string m_file_type;
	};

}

