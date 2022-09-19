// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <map>
#include <vector>
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
        typedef std::string Key;
        typedef std::string Value;
        
        /**
         * @brief Construct an empty query object
         */
        Query() = default;

        /**
         * @brief Construct a query object from a map of parameters
         */
        explicit Query(const std::vector<std::pair<Key, Value>> &parameters);
        
        /**
         * @brief Access values by key
         */
        Value& operator[] (const Key &key);
        
        /**
         * @brief Erase values by key
         */
        void erase(const Key &key);
        
        /**
         * @brief Returns true if key exists
         */
        bool contains(const Key &key);
        
        /**
         * @brief Returns all keys
         */
        std::vector<Key> list();
        
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
        
    private:
        std::map<Key, Value> _parameters {};
        std::vector<Key> _order {};
    };
}
