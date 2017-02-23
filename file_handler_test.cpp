#include "gtest/gtest.h"
#include "request.hpp"
#include "response.hpp"
#include "file_handler.hpp"
#include "config_parser.h"
#include <string>
#include <limits.h>
#include <unistd.h>
namespace http {
namespace server {

TEST(SimpleRequestTest, Docroot) {
	NginxConfig config;
	Request req;
	Response resp;
	//std::string sample_request="GET /test.txt HTTP/1.1 \nHost: localhost:12345";
	std::string sample_request="GET /test.txt HTTP/1.1\nHost:localhost";
	req.Parse(sample_request);
	file_handler *f= new file_handler();
	f->Init("", config);
	f->HandleRequest(req, &resp);
	std::string test = "Content-Length: " + std::to_string(test.length()) + "\r\n";
	test += "Content-Type: text/plain\r\n";
	test += "Just a test file to check webserver response in the Integration Test.\n";
	EXPECT_EQ(req.method(), "GET");
	EXPECT_EQ(req.uri(), "/test.txt");
	EXPECT_EQ(resp.ToString(), test);
}

TEST(BadRequestTest, Docroot) {
	NginxConfig config;
	Request req;
	Response resp;
	req.Parse("GET abcd.txt HTTP/1.1 \nHost: localhost:12345");
	file_handler *f = new file_handler();
	f->Init("", config);
	f->HandleRequest(req, &resp);
	EXPECT_EQ(resp.getResponseCode(), 400);
}

TEST(FileNotFound, Docroot) {
	NginxConfig config;
	Request req;
	Response rep;
	req.Parse("GET /abcd.txt HTTP/1.1 \nHost: localhost:12345");
	file_handler *f= new file_handler();
	f->Init("", config);
	f->HandleRequest(req, &rep);
	EXPECT_EQ(rep.getResponseCode(), 404);
}

}}
