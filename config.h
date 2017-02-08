// An nginx config file in memory.

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "config_parser.h"

extern const int DEFAULT_PORT;

extern const char* PORT_TOKEN;
extern const char* PATH_TOKEN;
extern const char* FILE_HANDLER_ROOT_TOKEN;

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

struct PathOption{
	PathOption(){}
	PathOption(const std::string& token_, const std::string& value_) : token(token_), value(value_){}

	std::string token;
	std::string value;
	PathOption* next_option;
};

struct Path{
	Path(){}
	Path(const std::string& token_, const std::string& handler_name_) : token(token_), handler_name(handler_name_){}

	std::string token;
	std::string handler_name;
	PathOption* options;
	Path* next_path;
};


// The in-memory representation of the entire config.
class ServerConfig {
 private:
	int portNo;
	Path* paths;
	bool ParseStatements();
	bool ParseStatement(std::shared_ptr<NginxConfigStatement> statement, Path* lastPath = nullptr);
	Path* getLastPath();
	PathOption* getLastPathOption(Path* path);
	NginxConfig* parsedConfig;

 public:
	ServerConfig(const char* configFilePath);
	int GetPortNo();
	std::string ToString();
	~ServerConfig();
	Path* GetPaths();
};

#endif //  CONFIG_H
