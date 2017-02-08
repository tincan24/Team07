//Used from http://www.boost.org/doc/libs/1_60_0/doc/html/boost_asio/example/cpp03/http/server/request.hpp

#ifndef ECHO_HANDLER_HPP
#define ECHO_HANDLER_HPP
// Based Off http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp11/http/server/request_handler.cpp
#include "base_handler.hpp"

namespace http {
namespace server {
class echo_handler: public base_handler {
public:
	void handle_request(const request &req, reply& rep);
	~echo_handler(){}
};

}
}

#endif // ECHO_HANDLER_HPP
