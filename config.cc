#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include <string.h>
#include <boost/foreach.hpp>

#include "config.h"
#include "base_handler.hpp"

const int DEFAULT_PORT = 80;

const char* PORT_TOKEN = "port";
const char* PATH_TOKEN = "path";
const char* FILE_HANDLER_ROOT_TOKEN = "root";

ServerConfig::ServerConfig(const char* configFilePath) {
	NginxConfigParser config_parser;
	parsedConfig = new NginxConfig();
	config_parser.Parse(configFilePath, parsedConfig);

	ParseStatements();
}

int ServerConfig::GetPortNo() {
	return portNo;
}

boost::unordered_map<std::string, Path*>& ServerConfig::GetPaths() {
	return paths;
}

bool ServerConfig::ParseStatements() {
	portNo = 0;
	for(const auto& statement : parsedConfig->statements_)
	{
		if(statement && statement->tokens_.size() >= 1)
			ParseStatement(statement);
	}

	if(portNo == 0)
	{
		//Setting standard port if not specified.
		portNo = 80;
		throw InvalidConfigException("Port number missing in config file.");
	}
	if(paths.empty())
		throw InvalidConfigException("No handler paths specified. Server not useable.");

	return true;
}

bool ServerConfig::ParseStatement(std::shared_ptr<NginxConfigStatement> statement, Path* lastPath) {
	if(strcmp(statement->tokens_[0].c_str(), PORT_TOKEN) == 0)
	{
		int portNoRead = std::stoi(statement->tokens_[1]);
		if(portNoRead > 0 && portNoRead <= 65535)
			portNo = portNoRead;
		else
		{
			//Setting standard port if given port out of range.
			portNo = 80;
			throw PortRangeException("Port number out of range.");
		}
		return true;
	} 
	else if(statement->tokens_[0].compare(PATH_TOKEN) == 0)
	{
		Path* new_path = new Path(statement->tokens_[1], statement->tokens_[2]);
		paths[statement->tokens_[1]] = new_path;

		if(statement->child_block_ != nullptr)
			for (const auto& fileHandlerStatement : statement->child_block_->statements_) 
				ParseStatement(fileHandlerStatement, new_path);
		if(statement->tokens_[2].compare(http::server::FILE_HANDLER_NAME) == 0 && 
		   (new_path->options.empty() ||  
			new_path->options[FILE_HANDLER_ROOT_TOKEN] == nullptr))
			throw InvalidConfigException("No doc_root path specified. File handler not useable.");
		return true;
	}
	else if (strcmp(statement->tokens_[0].c_str(), FILE_HANDLER_ROOT_TOKEN) == 0)
	{

		PathOption* new_option = new PathOption(statement->tokens_[0], statement->tokens_[1]);
		lastPath->options[statement->tokens_[0]] = new_option;
	}
	return false;
}

ServerConfig::~ServerConfig(){
	typedef std::pair<std::string, Path*> map_val_type;
	typedef std::pair<std::string, PathOption*> option_map_val_type;

	BOOST_FOREACH(map_val_type path, paths) 
	{
		BOOST_FOREACH(option_map_val_type option, std::get<1>(path)->options) 
		{
			delete(std::get<1>(option));
		}
		delete(std::get<1>(path));
	}
}

std::string ServerConfig::ToString() {
  std::string config_output;
  config_output.append("Server is running on Port: ");
  config_output.append(std::to_string(GetPortNo()));

  return config_output;
}
