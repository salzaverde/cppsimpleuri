// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/query.h>

#include <vector>
#include <regex>
#include <sstream>

namespace salzaverde {
    static std::vector<std::string> split(const std::string &input, const std::string &delimiter) {
        std::vector<std::string> output;
        std::stringstream rawStream(input);
        std::string match;
        
        while (std::getline(rawStream, match, delimiter.c_str()[0]))
            output.push_back(match);

        return output;
    }

    static std::pair<std::string, std::string> parseComponent(const std::string &component) {
        auto key = component.substr(0, component.find("="));
        if(key == component)
            return std::make_pair(key, "");

        auto value = component.substr(component.find("=") + 1);
        return std::make_pair(key, value);
    }

    Query::Query(const std::vector<std::pair<Key, Value>> &parameters) {
        for(auto &param : parameters) {
            _order.push_back(param.first);
            _parameters.emplace(param);
        }
    };

    Query::Value& Query::operator[] (const Key &key) {
        return _parameters[key];
    }
    
    void Query::erase(const Key &key) {
        for(auto it = _order.begin(); it != _order.end(); ++it) {
            if(*it == key) it = _order.erase(it);
        }
        _parameters.erase(key);
    }
    
    bool Query::contains(const Key &key) {
        return _parameters.find(key) != _parameters.end();
    }
    
    std::vector<Query::Key> Query::list() {
        return _order;
    }
    
    Query Query::parse(const std::string& raw, const std::string &delimiter) {
        if(raw.empty())
            return Query();

        Query query;
        auto components = split(raw, delimiter);

        for(auto &component : components) {
            //Matches the first occurance of "=" and assumes everything after that is "value"
            auto result = parseComponent(component);
            query._order.push_back(result.first);
            query._parameters.emplace(result);
        }

        return query;
    }

    std::string Query::dump(const std::string &delimiter) {
        if(_parameters.empty())
            return "";
        
        std::string queryString = "";
        auto it = _order.begin();
        while(it != _order.end()) {
            if(it != _order.begin()) queryString += delimiter;
            if(_parameters[*it] == "")
                queryString += *it;
            else
                queryString += *it + "=" + _parameters[*it];
            it++;
        }
        return queryString;
    }
}
