#include "gtest/gtest.h"
#include "request.hpp"
#include "response.hpp"
#include "file_handler.hpp"
#include <string>
#include <limits.h>
#include <unistd.h>
namespace http {
namespace server {
std::string currdir()
{
	char result[PATH_MAX];
	ssize_t count = readlink ("/proc/self/exe", result, PATH_MAX);
	std::string abc = std::string (result, (count > 0) ? count : 0);
	std::string last = abc.substr(0,abc.find_last_of('/'));
	return last;
}


TEST(SimpleRequestTest, Docroot) {
	Request req;
	Response resp;
	req.uri="/test.txt";
	file_handler(currdir()).HandleRequest(req, resp);
	std::string test = "Content-Length: " + std::to_string(test.length())) + "\r\n";
	test += "Content-Type: text/plain\r\n";
	test += "Just a test file to check webserver response in the Integration Test.\n";
	EXPECT_EQ(rep.ToString(), test);
}

TEST(BadRequestTest, Docroot) {
	Request req;
	Response resp;
	req.uri="abcd.txt";
	file_handler(currdir()).HandleRequest(req, resp);
	EXPECT_EQ(resp.getResponseCode(), 400);
}

TEST(FileNotFound, Docroot) {
	Request req;
	Response rep;
	req.uri="/abcd.txt";
	file_handler(currdir()).HandleRequest(req, rep);
	EXPECT_EQ(resp.getResponseCode(), 404);
}

}}
