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
    auto myRawUri = std::string("https://myHomepage.com:1176/my/music?added=last");
    auto uri = salzaverde::URI::parse(myRawUri);
    
    //Access the uri's elements
    auto scheme = uri->getScheme();
    auto host = uri->getHost();
    auto port = uri->getPort();
    auto path = uri->getPath();
    auto queryString = uri->getQuery();
    
    //Add or replace a query parameter
	auto query = salzaverde::Query::parse(queryString);
    query->set("sortby", "name");
    
    //Retrieve a query parameter value by key
    auto sortbyValue = query->get("sortby");
    
    //A value for this key might not exist
    if(! sortbyValue) {
        //Do some error handling
    }
       
    //We can also build a new query string in place
    auto newQuery = salzaverde::Query::build({
        {"list", "songs"},
        {"include", "rhcp"}
    });

    //You can check if a key exists
    auto containsListKey = newQuery->contains("list");
    auto keys = query->listKeys(); //Returns a set of keys
    
    //And replace it in an existing uri
    uri->setQuery(newQuery->dump());
    
    //Print results
    printLine("Parsing " + myRawUri + ":");
    printLine("");
    printLine("Scheme is: " + scheme);
    printLine("Host is: " + host);
    printLine("Port is: " + port);
    printLine("Path is: " + path);
    printLine("Query string is: " + queryString);
    printLine("");
    
    printLine("Parsing Query: " + uri->getQuery());
    printLine("Query contains \"list\" key: " + (containsListKey? std::string("true") : std::string("false")));
    printLine("");
    printLine("Uri with replaced QueryString: " + uri->dump());
    printLine("");
}
