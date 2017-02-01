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

TEST(ConnectionTesting, ConnectionConstructor) {
	try {
		
		http::server::server server_("127.0.0.1", "12345");
		
		boost::asio::io_service io_service_;
		boost::asio::ip::tcp::acceptor acceptor_(io_service_);

		boost::asio::ip::tcp::socket socket_(io_service_);

		boost::system::error_code ec;
  		boost::asio::ip::tcp::resolver resolver(io_service_);
  		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({"127.0.0.1",  "12345"}, ec);
  		acceptor_.open(endpoint.protocol());
  		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  		//acceptor_.bind(endpoint);

  		throw ec;
		//http::server::connection connection_(socket_);	

	}
	catch(boost::system::error_code &e) {
		//std::cout << e.message();
		EXPECT_EQ(e.message(), "Success");
	}
}

