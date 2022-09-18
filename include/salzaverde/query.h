// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <map>
#include <string>

namespace salzaverde {
    /**
     * @brief A query string parser which expects a query string in the form of
     * "key1=value1&key2=value2"
     *
     * Key-value pairs are called parameters and a "Query" object can also be generated
     * from a map representing a number of parameters.
     */
    struct Query {
    public:
        /**
         * @brief The query parameters
         */
        std::map<std::string, std::string> parameters {};

        /**
         * @brief Construct an empty query object
         */
        Query() = default;

        /**
         * @brief Construct a query object from a map of parameters
         */
        explicit Query(const std::map<std::string, std::string> &parameters);
        
        /**
         * @brief Parses a raw query string
         * @param delimiter valid delimiters are: "!" / "$" / "&" / "'" / "*" / "+" / "," / ";"
         */
        static Query parse(const std::string &raw, const std::string &delimiter = "&");
        
        /**
         * @brief Returns a query string
         * @param delimiter valid delimiters are: "!" / "$" / "&" / "'" / "*" / "+" / "," / ";"
         */
        virtual std::string dump(const std::string &delimiter = "&");
    };
}
