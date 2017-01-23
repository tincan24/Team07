#ifndef HTTP_REPLY_HPP
#define HTTP_REPLY_HPP

#include <string>
#include <vector>
#include <boost/asio.hpp>

namespace http {
namespace server {

struct header
{
  std::string name;
  std::string value;
};

struct reply
{
  enum status_type
  {
    ok = 200,
    moved_permanently = 301,
    bad_request = 400,
    unauthorized = 401,
    forbidden = 403,
    not_found = 404,
    internal_server_error = 500
  } status;

  std::vector<header> headers;
  std::string content;
  std::vector<boost::asio::const_buffer> to_buffers();
};





}}

#endif // HTTP_REPLY_HPP
