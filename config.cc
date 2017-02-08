#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>
#include <string.h>

#include "config.h"
#include "base_handler.hpp"

const int DEFAULT_PORT = 80;

const char* PORT_TOKEN = "port";
const char* PATH_TOKEN = "path";
const char* FILE_HANDLER_ROOT_TOKEN = "root";

ServerConfig::ServerConfig(const char* configFilePath) {
	paths = nullptr;
	NginxConfigParser config_parser;
	parsedConfig = new NginxConfig();
	config_parser.Parse(configFilePath, parsedConfig);

	ParseStatements();
}

int ServerConfig::GetPortNo() {
	return portNo;
}

Path* ServerConfig::GetPaths() {
	return paths;
}

bool ServerConfig::ParseStatements() {
	portNo = 0;
	for(const auto& statement : parsedConfig->statements_)
	{
		if(statement && statement->tokens_.size() >= 1)
			ParseStatement(statement, getLastPath());
	}

	if(portNo == 0)
	{
		//Setting standard port if not specified.
		portNo = 80;
		throw InvalidConfigException("Port number missing in config file.");
	}
	if(paths == nullptr)
		throw InvalidConfigException("No handler paths specified. Server not useable.");

	return true;
}



Path* ServerConfig::getLastPath(){
	if(paths == nullptr)
		return paths;

	Path* last_path = paths;
	while(last_path != nullptr && last_path->next_path != nullptr)
		last_path = last_path->next_path;
	return last_path;
}

PathOption* ServerConfig::getLastPathOption(Path* path){
	if(path == nullptr)
		return nullptr;

	PathOption* last_path_option = path->options;
	while(last_path_option != nullptr && last_path_option->next_option != nullptr)
		last_path_option = last_path_option->next_option;
	return last_path_option;
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

		if(lastPath == nullptr)
		{
			lastPath = new Path();
			paths = lastPath;
		}
		else
		{
			lastPath->next_path = new Path();
			lastPath = lastPath->next_path;
		}

		lastPath->next_path = nullptr;
		lastPath->options = nullptr;
		lastPath->token = statement->tokens_[1];
		lastPath->handler_name = statement->tokens_[2];

		if(statement->child_block_ != nullptr)
			for (const auto& fileHandlerStatement : statement->child_block_->statements_) 
				ParseStatement(fileHandlerStatement, lastPath);
		if(strcmp(statement->tokens_[2].c_str(), http::server::FILE_HANDLER_NAME) == 0 && 
		   (lastPath->options == nullptr || 
			strcmp(lastPath->options->token.c_str(), FILE_HANDLER_ROOT_TOKEN) != 0))
			throw InvalidConfigException("No doc_root path specified. File handler not useable.");
		return true;
	}
	else if (strcmp(statement->tokens_[0].c_str(), FILE_HANDLER_ROOT_TOKEN) == 0)
	{
		PathOption* new_option = getLastPathOption(lastPath);
		if(new_option == nullptr)
		{
			new_option = new PathOption();
			lastPath->options = new_option;
		}
		else
		{
			new_option->next_option = new PathOption();
			new_option = new_option->next_option;
		}

		new_option->next_option = nullptr;
		new_option->token = statement->tokens_[0];
		new_option->value = statement->tokens_[1];
	}
	return false;
}

ServerConfig::~ServerConfig(){
	while(paths != nullptr && paths->next_path!= nullptr)
	{
		Path* next_path = paths->next_path;
		while(paths->options != nullptr && paths->options->next_option != nullptr)
		{

			PathOption* next_option = paths->options->next_option;
			delete(paths->options);
			paths->options = next_option;
		}
		delete(paths->options);
		delete(paths);
		paths = next_path;
	}
	delete(paths);
}

std::string ServerConfig::ToString() {
  std::string config_output;
  config_output.append("Server is running on Port: ");
  config_output.append(std::to_string(GetPortNo()));

  return config_output;
}
