// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/query.h>
#include <vector>

namespace salzaverde {
    Query::Query(const std::map<std::string, std::string> &parameters) : parameters(parameters) {};

    Query Query::parse(const std::string& queryString) {
        if(queryString.empty())
            return Query();

        Query query;
        std::string remaining = queryString;
        
        std::vector<std::string> keyValuePairs;
        size_t pos = 0;
        while(pos != std::string::npos) {
            pos = remaining.find("&");
            keyValuePairs.push_back(remaining.substr(0, pos));
            remaining = remaining.substr(pos + 1);
        }
        
        for(auto &pair : keyValuePairs) {
            pos = pair.find("=");
            query.parameters.emplace(pair.substr(0, pos), pair.substr(pos + 1));
        }

        return query;
    }

    std::string Query::dump() {
        if(parameters.empty())
            return "";
        
        std::string queryString = "";
        auto it = parameters.begin();
        queryString += it->first + "=" + it->second;
        while(++it != parameters.end()) {
            queryString += "&" + it->first + "=" + it->second;
        }
        return queryString;
    }
}
