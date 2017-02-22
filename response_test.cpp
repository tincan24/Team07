#include "gtest/gtest.h"
#include "reply.hpp"

TEST(ResponseTest, StatusCheck) {
    http::server::Response resp_;
    resp_.SetStatus(http::server::Response::ok);
  	EXPECT_EQ(resp_.getResponseCode(), 200);
}

TEST(ResponseTest, HeaderStruct) {
	http::server::Response resp_;
	resp_.SetBody("Test content");
	http::server::header head_;
	head_.name = "Content-Length";
	head_.value = std::to_string(resp_.content.size());
	resp_.AddHeader(head_.name, head_.value);
	EXPECT_EQ(resp_.getHeaders()[0].name, "Content-Length");
	EXPECT_EQ(resp_.getHeaders()[0].value, "12");
}

TEST(ResponseTest, HeaderVector) {
	http::server::Response resp_;
	resp_.SetBody("Test content");
	http::server::header head_;
	head_.name = "Content-Length";
	head_.value = std::to_string(resp_.content.size());
	resp_.AddHeader(head_.name, head_.value);
	EXPECT_EQ(resp_.getHeaders().size(), 1);

	head_.name = "Content-Type";
	head_.value = "text/plain";
	resp_.AddHeader(head_.name, head_.value);
	EXPECT_EQ(resp_.getHeaders().size(), 2);
}

class ResponseHeaderTest : public ::testing::Test {
protected:
	bool AnalyzeHeader(std::string rcontent){
		resp_.SetStatus(http::server::Response::not_found);
		resp_.SetBody(rcontent);
		buffers = resp_.to_buffers();
		if (resp_.getHeaders().size() == 0) {
			return true;
		}
		return false;	

	}
	void loadHeader() {
		resp_.SetStatus(http::server::Response::ok);
		res_.SetBody("Test content");
		head1_.name = "Content-Length";
		head1_.value = std::to_string(reply_.content.size());
		reply_.AddHeader(head1_.name, head1_.value);

		head2_.name = "Content-Type";
		head2_.value = "text/plain";
		reply_.AddHeader(head2_.name, head2_.value);

		buffers = reply_.to_buffers();
	}
	http::server::Response resp_;
	http::server::header head1_, head2_;
	std::vector<boost::asio::const_buffer> buffers;
};

TEST_F(ResponseHeaderTest, HeaderParsing) {
	loadHeader();
  	EXPECT_EQ(resp_.getBody(), "Test content");
  	EXPECT_EQ(resp_.getHeaders()[0].name, "Content-Length");
  	EXPECT_EQ(resp_.getHeaders()[0].value, "12");
  	EXPECT_EQ(resp_.getHeaders()[1].name, "Content-Type");
  	EXPECT_EQ(resp_.getHeaders()[1].value,"text/plain");
}

TEST_F(ResponseHeaderTest, ContentPassedInBuffer) {
  	EXPECT_TRUE(AnalyzeHeader("Test Content"));
}
