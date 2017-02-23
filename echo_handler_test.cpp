#include "gtest/gtest.h"
#include "request.hpp"
#include "response.hpp"
#include "echo_handler.hpp"
#include "config_parser.h"
#include <string>
#include <limits.h>
#include <unistd.h>
namespace http {
namespace server {

TEST(EchoTest, SimpleEcho) {
	NginxConfig config;
	Request req;
	Response resp;
	req.Parse("GET / HTTP/1.1\nHost: localhost:12345\n");
	echo_handler *echo_handler_ = new echo_handler();
	echo_handler_->Init("", config);
	echo_handler_->HandleRequest(req, &resp);
	EXPECT_EQ(resp.getResponseCode(), 200);
	EXPECT_EQ(resp.getContent(), req.body());
}

}
}
