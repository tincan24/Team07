// Based Off http://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp11/http/server/request_handler.cpp
#include "echo_handler.hpp"
#include <fstream>
#include <sstream>

namespace http {
namespace server {

void echo_handler::handle_request(const request &req, reply& rep) {
	rep.content += req.content;//.append(req.content, req.content+ req.bytes);

	rep.status = reply::ok;
	header head0;
	head0.name = "Content-Length";
	head0.value = std::to_string(rep.content.size());
	rep.headers.push_back(head0);

	header head1;
	head1.name = "Content-Type";
	head1.value = "text/plain";
	rep.headers.push_back(head1);

}

}
}
