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
	 * "?key1=value1&key2=value2" or "key1=value1&key2=value2"
	 *
	 * Key-value pairs are called parameters and a "Query" object can also be generated
	 * from a map representing a number of parameters.
	 */
	class Query {
	public:
		virtual ~Query() {}
		
		/**
		 * @brief Factories
		 */
		static std::unique_ptr<Query> parse(const std::string &raw);
		static std::unique_ptr<Query> build(const std::map<std::string, std::string> &parameters);
		
		/**
		 * @brief Returns a string representation of the query object including the preceding question mark.
		 */
		virtual std::string dump() = 0;
		
		/**
		 * @brief Operations
		 */
		virtual std::optional<std::string> get(const std::string &key) = 0;
		virtual void set(const std::string &key, const std::string &value) = 0;
		virtual void erase(const std::string &key) = 0;
		virtual bool contains(const std::string &key) = 0;
		virtual std::set<std::string> listKeys() = 0;
	};
}
