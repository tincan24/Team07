#include "gtest/gtest.h"
#include "request.hpp"
#include "response.hpp"
#include "echo_handler.hpp"
#include <string>
#include <limits.h>
#include <unistd.h>
namespace http {
namespace server {

TEST(EchoTest, SimpleEcho) {
	Request req;
	Response resp;
	req.content="test content";
	echo_handler echo_handler_;
	echo_handler_.handle_request(req, resp);
	Request req;
	Response resp;
	req.uri="/test.txt";
	file_handler(currdir()).HandleRequest(req, resp);
	std::string test =  "Content-Length: " + std::to_string(test.length())) + "\r\n";
	test += "Content-Type: text/plain\r\n";
	EXPECT_EQ(resp.ToString(), test + req.content());
}

}
}
