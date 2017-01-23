#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "config.h"

#define SERVER_TOKEN "server"
#define PORT_TOKEN "listen"

ServerConfig::ServerConfig(const char* configFilePath) {
  NginxConfigParser config_parser;
  config_parser.Parse(configFilePath, &parsedConfig);

  ParseStatements();
}

int ServerConfig::GetPortNo() {
	return portNo;
}

bool ServerConfig::ParseStatements() {
	for (const auto& statement : parsedConfig.statements_) 
	{
		if(statement->tokens_[0].compare(SERVER_TOKEN) == 0 &&
		   statement->child_block_ != nullptr)
		{
			for (const auto& serverStatement : statement->child_block_->statements_) 
			{
				if(serverStatement->tokens_[0].compare(PORT_TOKEN) == 0)
				{
					int portNoRead = std::stoi(serverStatement->tokens_[1]);
					if(portNoRead > 0 && portNoRead <= 65535)
					{
						portNo = portNoRead;
						return true;
					}
					else
					{
						//Setting standard port if given port out of range.
						portNo = 80;
						throw PortRangeException("Port number out of range.");
						return false;
					}
				}
			}
		}
	}
	
	//Setting standard port if not specified.
	portNo = 80;
	throw InvalidConfigException("Port number missing in config file.");
	return false;
}

std::string ServerConfig::ToString() {
  std::string config_output;
  config_output.append("Server is running on Port: " + GetPortNo());

  return config_output;
}
