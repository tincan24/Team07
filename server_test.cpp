#include "gtest/gtest.h"
#include "server.hpp"

class ServerTest : public ::testing::Test {
protected:
	bool checkAccept(boost::system::error_code ec) {
		http::server::server server_("127.0.0.1", "8080");
		return server_.checkServer(ec);
	}

};

TEST_F(ServerTest, ServerStatus) {
	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::success);
    EXPECT_TRUE(checkAccept(ec));
}

TEST(ConnectionTest, ConnectionStatus) {
	http::server::server server_("127.0.0.1", "12345");
	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::connection_aborted);
    EXPECT_FALSE(server_.checkServer(ec));
}
TEST(ConnectionTest, ConnectionStatusMore) {
	http::server::server server_("127.0.0.1", "12346");
	boost::system::error_code ec = boost::system::errc::make_error_code(boost::system::errc::connection_refused);
    EXPECT_FALSE(server_.checkServer(ec));
}


