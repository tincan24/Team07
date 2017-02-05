//Used from http://www.boost.org/doc/libs/1_60_0/doc/html/boost_asio/example/cpp03/http/server/request.hpp

#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP
// Based Off http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp11/http/server/request_handler.cpp
#include "base_handler.hpp"

namespace http {
namespace server {
class file_handler: public base_handler {
public:
	explicit file_handler(const std::string& doc){
		this->docroot = doc;
	}
	bool url_decode(const std::string& in, std::string& out);
	void handle_request(const request &req, reply& rep);
private:
	std::string docroot;
};

}
}

#endif // FILE_HANDLER_HPP
