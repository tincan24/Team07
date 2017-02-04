#include "gtest/gtest.h"
#include "request.hpp"
#include "reply.hpp"
#include "file_handler.cpp"
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
	request req;
	reply rep;
	req.uri="/test.txt";
	file_handler(currdir()).handle_request(req, rep);
	std::string test ="Just a test file to check webserver response in the Integration Test.\n";
	EXPECT_EQ(rep.headers[0].name, "Content-Length");
	EXPECT_EQ(rep.headers[0].value, std::to_string(test.length()));
	EXPECT_EQ(rep.headers[1].name, "Content-Type");
	EXPECT_EQ(rep.headers[1].value, "text/plain");
	EXPECT_EQ(rep.content, test);
}

TEST(BadRequestTest, Docroot) {
	request req;
	reply rep;
	req.uri="abcd.txt";
	file_handler(currdir()).handle_request(req, rep);
	EXPECT_EQ(rep.status, 400);
}

TEST(FileNotFound, Docroot) {
	request req;
	reply rep;
	req.uri="/abcd.txt";
	file_handler(currdir()).handle_request(req, rep);
	EXPECT_EQ(rep.status, 404);
}}}
