// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <memory>
#include <string>

namespace salzaverde {
    /**
     * @brief A URI parser according to RFC3986
     * Example URI: "http://some.host:12345/path/to/resource?some=query#some=fragment"
     *
     * Recognized elements are "Scheme", "Host", "Port", "Path", "Query" and "Fragment"
     * A URI can be parsed from any combination of those in the expected sequence according to RFC3986
     */
    class URI {
    public:
        virtual ~URI () {}
        
        /**
         * @brief Factory
         */
        static std::unique_ptr<URI> parse(const std::string &raw);
        
        /**
         * @brief URI Components
         */
        std::string scheme;
        std::string userinfo;
        std::string host;
        std::string port; 
        std::string path; 
        std::string query; 
        std::string fragment;
        
        /**
         * @brief Returns a string representation of the uri object
         */
        virtual std::string dump() = 0;
    };
}
