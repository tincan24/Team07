#ifndef SERVER_STATS_H
#define SERVER_STATS_H

#include "request_handler.hpp"

namespace http {
namespace server {

class ServerStats {
public:
    static ServerStats& getInstance();

    void insertHandler(const std::string& prefix, const std::string& handler_name);
    void insertRequest(const std::string& url, Response::ResponseCode response);
    int getNumRequests();

    using Handlers = std::vector<std::pair<std::string, std::string>>;
    Handlers getHandlers() const;

    using Requests = std::vector<std::pair<std::string, Response::ResponseCode>>;
    Requests getRequests() const;


    ServerStats(ServerStats const&) = delete;
    void operator=(ServerStats const&) = delete;

private:
    ServerStats() {} 

    std::vector<std::pair<std::string, std::string>> handlers_;        
    std::vector<std::pair<std::string, Response::ResponseCode>> url_requests_;
        
};

}
}

#endif // SERVER_STATS_H