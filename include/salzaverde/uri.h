#pragma once

#include <string>
#include <memory>
#include <map>
#include <set>

namespace salzaverde {
	class URI {
	public:
		class Query {
		public:
			virtual ~Query() {}
			static std::unique_ptr<Query> parse(const std::string &rawQuery);
			static std::unique_ptr<Query> build(const std::map<std::string, std::string> &params);
			
			virtual std::string dump() = 0;
			virtual std::string get(const std::string &key) = 0;
			virtual void set(const std::string &key, const std::string &value) = 0;
			virtual void erase(const std::string &key) = 0;
			virtual bool contains(const std::string &key) = 0;
			virtual std::set<std::string> listKeys() = 0;
		};
		
		static std::unique_ptr<URI> parse(const std::string &raw);
		
		virtual std::string getScheme() = 0;
		virtual void setScheme(const std::string &value) = 0;
		virtual std::string getHost() = 0;
		virtual void setHost(const std::string &value) = 0;
		virtual std::string getPort() = 0;
		virtual void setPort(const std::string &value) = 0;
		virtual std::string getPath() = 0;
		virtual void setPath(const std::string &value) = 0;
		virtual Query* getQuery() = 0;
		virtual void setQuery(std::unique_ptr<Query> query) = 0;
		
		virtual std::string toString() = 0;
	};
}
