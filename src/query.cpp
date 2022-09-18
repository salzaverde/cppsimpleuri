// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/query.h>

#include <vector>
#include <regex>

namespace salzaverde {
    static bool isValidDelimiter(const std::string &delimiter) {
        for(auto &valid : {"!", "$", "&", "'", "*", "+", ",", ";"})
            if(delimiter == valid) return true;

        return false;
    }

    static std::smatch matchRegex(const std::string &input, const std::regex &regex) {
        std::smatch results;
        std::regex_match(input, results, regex);
        return results;
    }

    Query::Query(const std::map<std::string, std::string> &parameters) : parameters(parameters) {};

    Query Query::parse(const std::string& queryString, const std::string &delimiter) {
        if(queryString.empty() || ! isValidDelimiter(delimiter))
            return Query();

        Query query;
        auto components = matchRegex(queryString, std::regex("[^" + delimiter + "]+"));
        for(auto &component : components) {
            //Matches the first occurance of "=" and assumes everything after that is "value"
            auto keyValuePair = matchRegex(queryString, std::regex("(^[^=]+)=?(.*)"));
            
            auto key = keyValuePair[1].str();
            auto value = keyValuePair.length() > 2? keyValuePair[2].str() : "";

            query.parameters.emplace(key, value);
        }

        return query;
    }

    std::string Query::dump(const std::string &delimiter) {
        if(parameters.empty())
            return "";
        
        std::string queryString = "";
        auto it = parameters.begin();
        queryString += it->first + "=" + it->second;
        while(++it != parameters.end()) {
            queryString += delimiter + it->first + "=" + it->second;
        }
        return queryString;
    }
}
