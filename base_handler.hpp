#ifndef BASE_REQUEST_HANDLER_HPP
#define BASE_REQUEST_HANDLER_HPP

#include "request.hpp"
#include "reply.hpp"

namespace http {
namespace server {

extern const char* FILE_HANDLER_NAME;
extern const char* ECHO_HANDLER_NAME;

class base_handler{
public:
	base_handler() {}
	virtual ~base_handler() {}

	static base_handler* make_handler(std::string& handler_name, std::string& file_root);
	
	// Handle a request and set up corresponding reply.
	virtual void handle_request(const request& req, reply& rep) = 0;
	
};

} // namespace server
} // namespace http

#endif //  BASE_REQUEST_HANDLER_HPP
