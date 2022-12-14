// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/uri.h>
#include <salzaverde/query.h>
#include <iostream>

void printLine(const std::string& text) {
    std::cout << text << std::endl;
}

int main(int argc, char *argv[]) {
    //URI Parsing
    auto myRawUri = std::string("https://salzaverde@github.io:1176/my/music?sort_by=title&before=2020#artist=rhcp");
    std::cout << "Parsing query: "   << myRawUri << std::endl;

    auto uri = salzaverde::URI::parse(myRawUri);
    
    std::cout << "Scheme:   "   << uri.scheme   << std::endl;
    std::cout << "Userinfo: "   << uri.userinfo << std::endl;
    std::cout << "Host:     "   << uri.host     << std::endl;
    std::cout << "Port:     "   << uri.port     << std::endl;
    std::cout << "Path:     "   << uri.path     << std::endl;
    std::cout << "Query:    "   << uri.query    << std::endl;
    std::cout << "Fragment: "   << uri.fragment << std::endl;
    
    //Query parsing
    auto queryString = "sort_by=title&before=2020";
    auto query = salzaverde::Query::parse(queryString);
    std::cout << "Query parameters:" << std::endl;

    //list keys
    for(auto &key : query.list()) {
        std::cout << "Found key: " << key << std::endl;
    }
    
    //edit
    query["sort_by"] = "length";
    std::cout << "Query is now: " << query.dump() << std::endl;
    
    //contains
    if(query.contains("sort_by")) {
        std::cout << "Query contains key \"sort_by\"!" << std::endl;
    }
    
    //erase
    query.erase("sort_by");
    std::cout << "Query is now: " << query.dump() << std::endl;
}
