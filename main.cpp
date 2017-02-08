#include <iostream>
#include <boost/asio.hpp>
#include <iostream>
#include "server.hpp"
#include "config.h"

int main (int argc, char* argv[]) {
	
	if (argc != 2) {
	std::cerr << "Invalid call. Please call ./webserver <config_file>" <<std::endl;
	return -1;
	}

  	int port;
	
	ServerConfig config(argv[1]);
	
	port = config.GetPortNo();
	std::cout << port;
	
	http::server::server server_("127.0.0.1", std::to_string(port), &config);
	//server_.config = &config;
	server_.run();

	return 0;
}
