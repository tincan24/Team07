#include "gtest/gtest.h"
#include "request.hpp"
#include "reply.hpp"
#include "echo_handler.hpp"
#include <string>
#include <limits.h>
#include <unistd.h>
namespace http {
namespace server {

TEST(EchoTest, SimpleEcho) {
	request req;
	reply rep;
	req.content="test content";
	echo_handler echo_handler_;
	echo_handler_.handle_request(req, rep);
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
	EXPECT_EQ(rep.headers[0].value, std::to_string(req.content.length()));
	EXPECT_EQ(rep.headers[1].name, "Content-Type");
	EXPECT_EQ(rep.headers[1].value, "text/plain");
	EXPECT_EQ(rep.content, req.content);
}

}
}
