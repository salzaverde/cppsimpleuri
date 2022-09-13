// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <memory>
#include <string>

namespace salzaverde {
    /**
     * @brief A simplified URI parser which expects an URI in the style of
     * "http://some.host:12345/path/to/resource?key1=value1&key2=value2"
     *
     * Recognized elements are "Scheme", "Host", "Port", "Path", "QueryString"
     * A URI can be parsed from any combination of those in the expected sequence,
     * none is required.
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
