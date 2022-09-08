#pragma once

#include <string>
#include <memory>
#include <map>
#include <set>

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
	
	//TODO: Rename to Builder
//	class Elements {
//	public:
//		Elements(const std::string &value, const std::string &prefix = "", const std::string &suffix = "");
//		Elements& operator| (const Elements &other);
//		std::string operator() ();
//		
//		static Elements Scheme(const std::string &value);
//		static Elements Host(const std::string &value);
//		static Elements Port(const std::string &value);
//		static Elements Path(const std::string &value);
//		static Elements Query(const std::string &value);
//		static Elements Query(const std::multimap<std::string, std::string> &params);
//		
//		std::string toString();
//		
//		std::string _value;
//		std::string _prefix;
//		std::string _suffix;
//	};
};
