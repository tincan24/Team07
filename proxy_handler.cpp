/* Source:
    www.boost.org/doc/libs/1_62_0/doc/html/boost_aAsio/example/cpp03/http/client/sync_client.cpp
*/

#include <iostream>
#include <cstring>
#include <istream>
#include <ostream>
#include <sstream>
#include <vector>
#include "proxy_handler.hpp"
#include "config_parser.h"

using boost::asio::ip::tcp;

namespace http {
namespace server {

ProxyHandler::ProxyHandler() {}

ProxyHandler::~ProxyHandler() {}

RequestHandler::Status ProxyHandler::Init(const std::string& uri_prefix, const NginxConfig& config) {
    uri_prefix_ = uri_prefix;

    if (config.statements_.size() == 0)
        return RequestHandler::not_found;

    location_ = config.statements_[0]->tokens_[1];
    return RequestHandler::OK;
}

RequestHandler::Status ProxyHandler::HandleRequest(const Request &request, Response* response) {
    std::string location = location_;
    
    for(;;) {
        Response::ResponseCode response_code = RedirectRequest(location, request.uri() == uri_prefix_ ? "/" : request.uri(), response);
        //TODO: change return not found
        if (response_code == Response::ResponseCode::internal_server_error)
            return RequestHandler::not_found;
        else if (response_code == Response::ResponseCode::ok)
            return RequestHandler::OK;
    }
    return RequestHandler::OK;
}

Response::ResponseCode ProxyHandler::RedirectRequest(std::string& location, const std::string& uri, Response* response) {
    boost::asio::io_service io_service;
    tcp::resolver resolver(io_service);
    std::cout << location << std::endl;
    tcp::resolver::query query(location, "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    tcp::socket socket(io_service);
    boost::asio::connect(socket, endpoint_iterator);
    
    std::cout << uri << "- uri\n";
    std::string req = "GET " + uri + " HTTP/1.1\r\nHost: " + location + "\r\nAccept: */*\r\nConnection: close\r\n\r\n";
    boost::asio::write(socket, boost::asio::buffer(req, req.size()));

    boost::asio::streambuf resp;
    boost::asio::read_until(socket, resp, "\r\n");
    
    std::istream response_stream(&resp);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
        std::cout << "Invalid response\n";
        return Response::ResponseCode::internal_server_error;
    }

    // Read the response headers, which are terminated by a blank line.
    boost::asio::read_until(socket, resp, "\r\n\r\n");

    // Process the response headers.
    std::string header;
    while (std::getline(response_stream, header) && header != "\r") {
        size_t colon_pos = header.find(":");
        std::string key = header.substr(0, colon_pos);
        if (key == "Content-Type") {
            std::string value = header.substr(colon_pos + 2);

            if (status_code == 302 && key == "Location") {
                location = value;
                return Response::ResponseCode::moved_temporarily;
            }

            response->AddHeader(key, value);
        }
    }

    if (status_code != 200) {
        std::cout << "Response returned with status code " << status_code << "\n";
        return Response::ResponseCode::internal_server_error;
    }

    //std::string response_body = "";
    
    // Write whatever content we already have to output.
    /*if (resp.size() > 0) {
        std::ostringstream ss;
        ss << &resp;
        response_body += ss.str();
    }*/

    // Read until EOF, writing data to output as we go.
    boost::system::error_code error;
    // while (boost::asio::read_until(socket, resp, "\n") > 0) {
    while (boost::asio::read(socket, resp, error)) {
        std::ostringstream ss;
        ss << &resp;
        // std::string s = ss.str();
        //std::string s((std::istreambuf_iterator<char>(&resp)), std::istreambuf_iterator<char>());
        response->SetBody(ss.str());
        // std::cout << s << std::endl;
        if (error == boost::asio::error::eof) {
            std::cout << "reached EOF" << std::endl;
            break;
        }
    }
    //std::cout << response_body << std::endl;
    // if (error != boost::asio::error::eof)
    //     std::cout << "Error: " << error << std::endl;
    //response->AddHeader("Content-Location", "/proxy/index.html");
    response->SetStatus(Response::ok);
    //response->SetBody(response_body);
    std::cout << response->ToString() << "\n";
    return Response::ResponseCode::ok;
}

}
}
