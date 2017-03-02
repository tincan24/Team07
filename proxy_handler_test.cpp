#include "gtest/gtest.h"
#include "request.hpp"
#include "response.hpp"
#include "proxy_handler.hpp"
#include "request_handler.hpp"
#include "config_parser.h"
#include <string>
#include <limits.h>
#include <unistd.h>
namespace http {
namespace server {

TEST(ProxyHandlerTest, InitTest) {
    ProxyHandler proxy_handler;
    NginxConfig* config = new NginxConfig();
    NginxConfigStatement* st = new NginxConfigStatement();

    st->tokens_.push_back("root");
    st->tokens_.push_back("ucla.edu");

    EXPECT_EQ(404, proxy_handler.Init("/proxy", *config));
}

TEST(ProxyHandlerTest, BadRequestTest) {
    Response resp;
    ProxyHandler proxy_handler;
    NginxConfig* config = new NginxConfig();
    NginxConfigStatement* st = new NginxConfigStatement();

    st->tokens_.push_back("root");
    st->tokens_.push_back("ucla.edu");

    std::string body = "GET /proxy HTTP/1.1\r\n\r\n";
    std::unique_ptr<Request> req = Request::Parse(body);

    proxy_handler.Init("/proxy", *config);
    //proxy_handler.HandleRequest(*req, &resp);

    EXPECT_EQ("HTTP/1.1 500 Internal Server Error\r\n", resp.ToString());
}

}
}
