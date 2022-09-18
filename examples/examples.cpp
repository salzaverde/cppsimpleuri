// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/uri.h>
#include <salzaverde/query.h>
#include <iostream>

void printLine(const std::string& text) {
    std::cout << text << std::endl;
}

int main(int argc, char *argv[]) {
    //Parsing
    auto myRawUri = std::string("https://myHomepage.com:1176/my/music?added=last#artist=rhcp");
    auto uri = salzaverde::URI::parse(myRawUri);
    
    //Access the uri's elements
    auto scheme = uri.scheme;
    auto host = uri.host;
    auto port = uri.port;
    auto path = uri.path;
    auto queryString = uri.query;
    auto fragment = uri.fragment;
    
    //Query parsing
    auto query = salzaverde::Query::parse(queryString);
    query.parameters["sortby"] = "name";
    
    //Retrieve a query parameter value by key
    if(query.parameters.find("sortby") != query.parameters.end())
        auto sortbyValue = query.parameters["sortby"];
    
       
    //Build a new query string in place
    auto newQuery = salzaverde::Query({
        {"list", "songs"},
        {"include", "rhcp"}
    });

	//Replace query in an existing uri
    uri.query = newQuery.dump();
    
    //Print results
    printLine("Parsing " + myRawUri + ":");
    printLine("");
    printLine("Scheme is: " + scheme);
    printLine("Host is: " + host);
    printLine("Port is: " + port);
    printLine("Path is: " + path);
    printLine("Query string is: " + queryString);
    printLine("Fragment is: " + fragment);
    printLine("");
}
