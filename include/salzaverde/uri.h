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
         * @brief Operations
         */
        virtual std::string getScheme() = 0;
        virtual void setScheme(const std::string &value) = 0;
		virtual std::string getUserInfo() = 0;
		virtual void setUserInfo(const std::string &value) = 0;
        virtual std::string getHost() = 0;
        virtual void setHost(const std::string &value) = 0;
        virtual std::string getPort() = 0;
        virtual void setPort(const std::string &value) = 0;
        virtual std::string getPath() = 0;
        virtual void setPath(const std::string &value) = 0;
        virtual std::string getQuery() = 0;
        virtual void setQuery(const std::string &value) = 0;
        virtual std::string getFragment() = 0;
        virtual void setFragment(const std::string &value) = 0;
        
        /**
         * @brief Returns a string representation of the uri object
         */
        virtual std::string dump() = 0;
    };
}
