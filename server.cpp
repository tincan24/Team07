//Based off: http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp11/http/server/server.cpp
// and http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp

#include <utility>
#include <boost/filesystem.hpp>
#include <string.h>
#include <boost/foreach.hpp>

#include "server.hpp"
//TODO: check necessary
#include "file_handler.hpp"
#include "echo_handler.hpp"

namespace http {
namespace server {

// HTTP RESPONSE/REQUEST WRITE/READ RELATED FUNCTIONS
connection::connection(boost::asio::ip::tcp::socket socket): socket_(std::move(socket))
{
	//paths_ = nullptr;
}

//connection::connection(boost::asio::ip::tcp::socket socket, Path* paths): socket_(std::move(socket)), paths_(paths)
//{}

void connection::start() {
  try {
    do_read();
  }
	catch(boost::system::error_code &e) {
    throw e;
  }
}

void connection::stop() {
	socket_.close();
}

void connection::do_read() {
	auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(buffer_),
	//TODO: inline specification
      [this, self](boost::system::error_code ec, std::size_t bytes)
      {
        request_ = Request::Parse(buffer_.data());
        std::string uri = request_->uri();
        boost::filesystem::path path{uri};
	RequestHandler* handler = nullptr;

	auto pathIt = path.begin();
	auto cur_prefix = pathIt->string();
	++pathIt;
	cur_prefix += pathIt->string();
	
	if((*handlers_)[cur_prefix] != nullptr)
        	(*handlers_)[cur_prefix]->HandleRequest(*request_, &response_);

	delete(handler);
        do_write();
      });
}

void connection::do_write() {
	auto self(shared_from_this());

	boost::asio::async_write(socket_, response_.to_buffers(),
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

server::server(const std::string& address, const std::string& sconfig_path)
  : io_service_(),
    acceptor_(io_service_),
  socket_(io_service_){

	config = new ServerConfig(sconfig_path);

	int addressNoRead = std::stoi(address);
	if (addressNoRead < 0) {
		throw boost::system::errc::make_error_code(boost::system::errc::bad_address);
	}

	int port = config->GetPortNo();
	if (port <= 0 || port > 65535) {
		throw boost::system::errc::make_error_code(boost::system::errc::argument_out_of_domain);
	}

	InitHandlers();

	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, std::to_string(port)});
	acceptor_.open(endpoint.protocol());
	acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
	acceptor_.bind(endpoint);
	acceptor_.listen();

	do_accept();
}

void server::InitHandlers() {
	BOOST_FOREACH(auto path_element, config->GetPaths()) 
	{
		Path* path = std::get<1>(path_element);
		auto handler = RequestHandler::CreateByName(path->handler_name);
  		handler->Init(path->token, *(path->child_block_));
		handlers_[path->token] = handler;
	}
}

server::~server() {
  delete(config);
}

void server::run() {
  try {
    io_service_.run();
  }
  catch(boost::system::error_code const &e) {
    throw e;
  }
	
}

void server::do_accept() {
  try {
    acceptor_.async_accept(socket_,
      [this](boost::system::error_code ec)
      {
        if (!acceptor_.is_open())
        {
          return;
        }

        if (!ec)
        {
	  if(config != nullptr)
	  {
          	std::shared_ptr<connection> con = std::make_shared<connection>(std::move(socket_));
		con->handlers_ = &handlers_;
		con->start();
	  }
        } else if (ec) {
          throw ec;
        }

        do_accept();
      });
  }
 	catch (boost::system::error_code const &e) {
    throw e;
  }
}

}
}
