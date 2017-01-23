#include <iostream>
#include <boost/asio.hpp>
#include "server.hpp"
#include "config_parser.cpp"

int main (int argc, char* argv[]) {
	
	if (argc != 2) {
	std::cerr << "Invalid call. Please call ./webserver <config_file>" <<endl;
	return -1;
	}


	NginxConfigParser config_parser;
    NginxConfig config;
  	int port;

  	/* get the value of port from config parser and store in port */

  	/* run server with port number specified */
}