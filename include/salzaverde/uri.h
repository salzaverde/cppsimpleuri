#pragma once

#include <string>
#include <memory>
#include <map>

//class URI {
//public:
//	class Query {
//	public:
//		~Query();
//
//		Query();
//		Query(const std::multimap<std::string, std::string> &parameters);
//		Query parse(const std::string &queryString);
//
//		void add(const std::string &key, const std::string &value);
//		void remove(const std::string &key);
//		void modify(const std::string &key, const std::string &value);
//		std::string getValue(const std::string &key);
//		std::multimap<std::string, std::string>* get();
//		std::string toString();
//	private:
//		std::multimap<std::string, std::string> _parameters;
//	};
//
//	URI();
//	~URI ();
//
//	URI parse(const std::string& rawUri);
//
//	constexpr std::string& getScheme();
//	void setScheme(const std::string &scheme);
//
//	constexpr std::string& getHost();
//	void setHost(const std::string &host);
//
//	constexpr std::string& getPort();
//	void setPort(const std::string &port);
//
//	constexpr std::string& getPath();
//	void setPath(const std::string &path);
//
//	constexpr Query* getQuery();
//	void setQuery(const Query &query);
//
//	//BaseURL is host + port
//	constexpr std::string& getBaseURL();
//	constexpr std::string& toString();
//private:
//	std::string _scheme, _host, _port, _path;
//	Query _query;
//};

//Elements: Scheme, Host, Port, Path (Could be used multiple times, starts with/), QueryString

//www.example.com:1234/some/path?key=value&key2=value2
class URI {
public:
	class UniqueURI : public std::unique_ptr<URI> {
	public:
		static UniqueURI make(const std::string& value, const std::string& prefix, const std::string& suffix);
		UniqueURI operator| (const UniqueURI &other);
	protected:
		using std::unique_ptr<URI>::unique_ptr;
	};
	
	static UniqueURI Scheme(const std::string& value);
	static UniqueURI Host(const std::string& value);
	static UniqueURI Port(const std::string& value);
	
	virtual std::string getValue() = 0;
	virtual std::string getPrefix() = 0;
	virtual std::string getSuffix() = 0;
	
	virtual std::string toString() = 0;
	
protected:
	static std::unique_ptr<URI> create(const std::string& value, const std::string& prefix, const std::string& suffix);
};
