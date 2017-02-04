#ifndef BASE_REQUEST_HANDLER_HPP
#define BASE_REQUEST_HANDLER_HPP

namespace http {
namespace server {

//TODO: idk if you wanna include these,example code did like this
struct request;
struct reply;

class base_handler{
public:
	base_handler(const base_handler&) = delete;
	base_handler& operator=(const base_handler&) = delete;
	
	// Handle a request and set up corresponding reply.
	void handle_request(const request& req, reply& rep) = 0;
	
private:
	reply* reply;
}

} // namespace server
} // namespace http

#endif //  BASE_REQUEST_HANDLER_HPP
