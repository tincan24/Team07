#include "server_stats.hpp"
#include <algorithm>

namespace http {
namespace server {

ServerStats& ServerStats::getInstance() {
    static ServerStats instance;
    return instance;
}

void ServerStats::insertHandler(const std::string& prefix, const std::string& handler_name) {
    std::pair<std::string, std::string> handler(prefix, handler_name);
    handlers_.push_back(handler);
}

void ServerStats::insertRequest(const std::string& url, Response::ResponseCode response) {
    std::pair<std::string, Response::ResponseCode> req(url, response);
    url_requests_.push_back(req);
}

ServerStats::Handlers ServerStats::getHandlers() const {
    return handlers_;
}

ServerStats::Requests ServerStats::getRequests() const {
    return url_requests_;
}

int ServerStats::getNumRequests() {
    return url_requests_.size();
}

void ServerStats::clearAllEntries() {
	handlers_.clear();
	url_requests_.clear();
}

}
}