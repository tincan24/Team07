#include "base_handler.hpp"
#include "echo_handler.hpp"
#include "file_handler.hpp"
#include "config.h"

namespace http {
namespace server {

const char* FILE_HANDLER_NAME = "file_handler";
const char* ECHO_HANDLER_NAME = "echo_handler";

base_handler* base_handler::make_handler(std::string& handler_name, std::string& file_root)
{
	if(strcmp(handler_name.c_str(), ECHO_HANDLER_NAME) == 0)
		return new echo_handler();
	else if(strcmp(handler_name.c_str(), FILE_HANDLER_NAME) == 0 && !file_root.empty())
		return new file_handler(file_root);
	else
		throw;
}

}
}
