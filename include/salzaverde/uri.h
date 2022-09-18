// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <string>

namespace salzaverde {
    /**
     * @brief A URI parser according to RFC3986
     * Example URI: "http://user@some.host:12345/path/to/resource?some=query#some=fragment"
     *
     * Recognized elements are "Scheme", "Userinfo", "Host", "Port", "Path", "Query" and "Fragment"
     * A URI can be parsed from any combination of those in the expected sequence according to RFC3986
     */
    struct URI {
    public:
        /**
         * @brief URI Components
         */
        std::string scheme {};
        std::string userinfo {};
        std::string host {};
        std::string port {}; 
        std::string path {}; 
        std::string query {}; 
        std::string fragment {};

        /**
         * @brief Parses a raw URI
         */
        static URI parse(const std::string &raw);
        
        /**
         * @brief Returns a raw URI
         */
        std::string dump();
    };
}
