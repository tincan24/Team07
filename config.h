// An nginx config file in memory.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "config_parser.h"

class InvalidConfigException: public std::runtime_error
{
public:
  InvalidConfigException(const std::string msg) : runtime_error(msg) {}
};

class PortRangeException: public std::range_error
{
public:
  PortRangeException(const std::string msg) : range_error(msg) {}
};


// The in-memory representation of the entire config.
class ServerConfig {
 private:
  int portNo;
  bool ParseStatements();
  NginxConfig parsedConfig;

 public:
  ServerConfig(const char* configFilePath);
  int GetPortNo();
  std::string ToString();
};


