// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/uri.h>
#include <regex>

#include <iostream>

namespace salzaverde {
    static std::smatch matchRegex(const std::string &input, const std::regex &regex) {
        std::smatch results;
        std::regex_match(input, results, regex);
        return results;
    }

    URI URI::parse(const std::string &raw) {
        //The original proposed regex from RFC 3986 splits the uri into scheme | authority | path | query | fragment
        auto RFC3986Results= matchRegex(raw, std::regex(R"(^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)"));
        
        //Split the authority into userinfo, host and port
        auto authority = RFC3986Results[4].str();
        auto authorityResults = matchRegex(authority, std::regex(R"(^(([^@]*)@)?((\[(.*)\]|[^:]+)|(.*)):?(.*))"));
        
        URI uri;
        uri.scheme = RFC3986Results[2].str();
        uri.userinfo = authorityResults[2].str();
        uri.host = authorityResults[4].str();
        uri.port = authorityResults[7].str();
        uri.path = RFC3986Results[5].str();
        uri.query = RFC3986Results[7].str();
        uri.fragment = RFC3986Results[9].str();

        return uri;
    }
    
    std::string URI::dump() {
        auto raw = std::string();
        bool hasAuthority = ! host.empty();
        
        if(! scheme.empty()) raw += scheme + (hasAuthority? "://" : ":");
        if(! userinfo.empty()) raw += userinfo + "@";
        if(! host.empty()) raw += host;
        if(! port.empty()) raw += ":" + port;
        if(! path.empty()) raw += path;
        if(! query.empty()) raw += "?" + query;
        if(! fragment.empty()) raw += "#" + fragment;
        return raw;
    }
}
