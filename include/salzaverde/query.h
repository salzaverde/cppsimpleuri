// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <memory>
#include <map>
#include <optional>
#include <set>
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
         */
        static Query parse(const std::string &raw);
        
        /**
         * @brief Returns a query string
         */
        virtual std::string dump();
    };
}
