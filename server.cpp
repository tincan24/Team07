//Based off: http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp11/http/server/server.cpp
// and http://www.boost.org/doc/libs/1_62_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp

#include <utility>
#include <boost/filesystem.hpp>
#include <string.h>

#include "server.hpp"
#include "file_handler.hpp"
#include "echo_handler.hpp"
#include "base_handler.hpp"

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
      [this, self](boost::system::error_code ec, std::size_t bytes)
      {
      	request_parser::result_type res;
        std::tie(res, std::ignore) = request_parser_.parse(request_, buffer_.data(), buffer_.data() + bytes);
        std::string uri = request_.uri;
        boost::filesystem::path path_{uri};
	base_handler* handler = nullptr;

	auto cur_path = (*paths_)[path_.parent_path().string()];
	if(cur_path == nullptr|| cur_path->options[std::string(FILE_HANDLER_ROOT_TOKEN)] == nullptr)
		cur_path = (*paths_)[path_.string()];
	if(cur_path != nullptr)
	{
		auto cur_option = cur_path->options[std::string(FILE_HANDLER_ROOT_TOKEN)];
		std::string doc_root;
		if(cur_option != nullptr)
		{
			request_.uri = "/" + path_.filename().string();
			doc_root += cur_option->value;
		}
		handler = base_handler::make_handler(cur_path->handler_name, doc_root);
	}

	/*Path* next_path = paths_;
	while(next_path != nullptr)
	{
		if((strcmp(next_path->token.c_str(), path_.parent_path().c_str()) == 0 &&
		    next_path->options != nullptr) ||
		   (strcmp(next_path->token.c_str(), path_.c_str()) == 0 && 
		    next_path->options == nullptr))
		{
			PathOption* next_option = next_path->options;
			std::string doc_root;
			while(next_option != nullptr)
			{
				if(strcmp(next_option->token.c_str(), FILE_HANDLER_ROOT_TOKEN) == 0)
				{
					request_.uri = "/" + path_.filename().string();
					doc_root += next_option->value;
					break;
				}
				next_option = next_option->next_option;
			}
			handler = base_handler::make_handler(next_path->handler_name, doc_root);
			break;
		}
		next_path = next_path->next_path;
	}*/

	if(handler != nullptr)
        	handler->handle_request(request_, reply_);
        do_write();
	delete(handler);

        //boost::filesystem::path path1_{config.getPaths()[0].token};
        //boost::filesystem::path path2_{config.getPaths()[1].token};
        //std::cout << "rootPath is " << path_.parent_path()<< std::endl;
        //std::string doc_root;
        //std::string filename = "/" + path_.filename().string();
        //std::cout << "filename is " << filename << std::endl;


        /*int echo = 0;
        if (uri == "/echo") {
          echo = 1;
        } else if (path_.parent_path() == path1_) {
          doc_root = "files";
        } else if (path_.parent_path() == path2_) {
          doc_root = "files2";
        }*/

        /*if (echo) {
          echo_handler echo_handler_;
	  echo_handler_.handle_request(request_, reply_);
        }
        else {
          request_.uri = filename;

          file_handler file_handler_(doc_root);
          file_handler_.handle_request(request_, reply_);
        }*/


        //reply_.content.append(buffer_.data(), buffer_.data() + bytes);
      	//if (reply_.content.substr(reply_.content.size() - 4, 4) == "\r\n\r\n" )
      	//	do_write();
      	//else
      	//	do_read();
      });
}

void connection::do_write() {
	auto self(shared_from_this());

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

server::server(const std::string& address, const std::string& port, ServerConfig* sconfig)
  : io_service_(),
    acceptor_(io_service_),
  socket_(io_service_),
  config(sconfig) {

  int addressNoRead = std::stoi(address);
  if (addressNoRead < 0) {
    throw boost::system::errc::make_error_code(boost::system::errc::bad_address);
  }
    
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  do_accept();

  int portNoRead = std::stoi(port);
  if (portNoRead <= 0 || portNoRead > 65535) {
    throw boost::system::errc::make_error_code(boost::system::errc::argument_out_of_domain);
  }
  
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
		con->paths_ = &config->GetPaths();
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
