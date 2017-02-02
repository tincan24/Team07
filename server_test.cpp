#include "gtest/gtest.h"
#include "server.hpp"

TEST(ServerTesting, ConstructorCheck) {
	try {
		http::server::server server_("127.0.0.1", "123450");
		
	}
	catch(boost::system::error_code &e) {
		EXPECT_EQ(e.message(), "Numerical argument out of domain");
	}

}

TEST(ServerTesting, AddressCheck) {
	try {
		http::server::server server_("-5", "12345");	
	}
	catch(boost::system::error_code &e) {
		EXPECT_EQ(e.message(), "Bad address");
	}

}

TEST(ConnectionTesting, InitializationAndConnectionTest) {
	try {
		http::server::server server_("127.0.0.1", "12345");	
	}
	catch(boost::system::error_code &e) {
		std::cout << e.message() << std::endl;
	}

}

void handler(
	const boost::system::error_code& ec, std::size_t bytes
	){};


TEST(ConnectionTesting, ConnectionConstructor) {
	try {
	boost::asio::io_service io;
	boost::asio::ip::tcp::socket sock(io);
	std::string buffer = "GET /index.html HTTP/1.1\r\n\r\n";
	
	sock.async_write_some(boost::asio::buffer(buffer), 
		std::bind([this, self](boost::system::error_code ec, std::size_t bytes))
		{});
	http::server::connection(std::move(sock)).start();
	}
	catch(boost::system::error_code &e) {
		//std::cout << e.message();
		EXPECT_EQ(e.message(), "Success");
	}
}

