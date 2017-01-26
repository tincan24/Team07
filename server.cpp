//Based off: http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp11/http/server/server.cpp
// and http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp


#include "server.hpp"
#include <utility>
#include <vector>

namespace http {
namespace server {

// HTTP RESPONSE/REQUEST WRITE/READ RELATED FUNCTIONS
connection::connection(boost::asio::ip::tcp::socket socket): socket_(std::move(socket))
{}

void connection::start() {
	do_read();
}

void connection::stop() {
	socket_.close();
}

void connection::do_read() {
	auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(buffer_),
      [this, self](boost::system::error_code ec, std::size_t bytes)
      {
      	reply_.content.append(buffer_.data(), buffer_.data() + bytes);
      	if (reply_.content.substr(reply_.content.size() - 4, 4) == "\r\n\r\n" )
      		do_write();
      	else
      		do_read();
      });
}

void connection::do_write() {
	auto self(shared_from_this());

	reply_.status = reply::ok;
	header head0;
	head0.name = "Content-Length";
	head0.value = std::to_string(reply_.content.size());
	reply_.headers.push_back(head0);
        
	header head1;
	head1.name = "Content-Type";
	head1.value = "text/plain";
	reply_.headers.push_back(head1);

	boost::asio::async_write(socket_, reply_.to_buffers(),
      [this, self](boost::system::error_code ec, std::size_t)
      {
        if (!ec)
        {
          boost::system::error_code ignored_ec;
          stop();
        }
      });
}

// SERVER CONNECTION RELATED FUNCTIONS

server::server(const std::string& address, const std::string& port)
  : io_service_(),
    acceptor_(io_service_),
    socket_(io_service_) {
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  do_accept();
}

void server::run() {
	io_service_.run();
}

void server::do_accept() {
 	acceptor_.async_accept(socket_,
      [this](boost::system::error_code ec)
      {
        if (!acceptor_.is_open())
        {
          return;
        }

        if (!ec)
        {
          std::make_shared<connection>(std::move(socket_))->start();
        }

        do_accept();
      });
}

}
}
