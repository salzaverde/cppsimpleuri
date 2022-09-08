#include "uri.h"

#include <vector>

static const std::string scheme_suffix = "://";
static const std::string path_prefix = "/";
static const std::string port_prefix = ":";
static const std::string query_prefix = "?";

static std::string buildQueryString(const std::multimap<std::string, std::string> &params) {
	std::string queryString = "";
	auto it = params.begin();
	queryString += it->first + "=" + it->second;
	
	while(++it != params.end()) {
		queryString += "&" + it->first + "=" + it->second;
	}
	return queryString;
};

class QueryParserImpl : public URI::Query {
public:
	QueryParserImpl(const std::string &rawQuery) {
		parse(rawQuery);
	}
	
	QueryParserImpl(const std::map<std::string, std::string> &params) : _params(params) {}
	
	virtual std::string dump() override {
		if(_params.empty())
			return "";
		
		std::string queryString = "?";
		auto it = _params.begin();
		queryString += it->first + "=" + it->second;
		
		while(++it != _params.end()) {
			queryString += "&" + it->first + "=" + it->second;
		}
		return queryString;
	}
	
	virtual std::string get(const std::string &key) override {
		if(_params.contains(key))
			return _params[key];
		
		return "not-found";
	}
	
	virtual void set(const std::string &key, const std::string &value) override {
		_params[key] = value;
	}
	
	virtual void erase(const std::string &key) override {
		_params.erase(key);
	}
	
	virtual bool contains(const std::string &key) override {
		return _params.contains(key);
	}
	
	virtual std::set<std::string> listKeys() override {
		std::set<std::string> keys;
		for(auto &param : _params)
			keys.emplace(param.first);
		return keys;
	}
	
private:
	std::map<std::string, std::string> _params;
	
	void parse(const std::string& queryString) {
		if(queryString.empty())
			return;

		std::string remaining = queryString;

		if(remaining.starts_with("?"))
			remaining.erase(remaining.begin());
		
		std::vector<std::string> keyValuePairs;
		size_t pos = 0;
		while(pos != std::string::npos) {
			pos = remaining.find("&");
			keyValuePairs.push_back(remaining.substr(0, pos));
			remaining = remaining.substr(pos + 1);
		}
		
		for(auto &pair : keyValuePairs) {
			pos = pair.find("=");
			_params.emplace(pair.substr(0, pos), pair.substr(pos + 1));
		}
	}
};

std::unique_ptr<URI::Query> URI::Query::parse(const std::string &rawQuery) {
	return std::make_unique<QueryParserImpl>(rawQuery);
}

std::unique_ptr<URI::Query> URI::Query::build(const std::map<std::string, std::string> &params) {
	return std::make_unique<QueryParserImpl>(params);
}

//URI::Elements::Elements(const std::string& value, const std::string& prefix, const std::string& suffix) : _value(value), _prefix(prefix), _suffix(suffix) {}
//
//URI::Elements& URI::Elements::operator| (const Elements &other) {
//	_value = _value + _suffix + other._prefix + other._value;
//	_suffix = other._suffix;
//	return *this;
//}
//
//std::string URI::Elements::toString() {
//	return _prefix + _value + _suffix;
//}
//
//URI::Elements URI::Elements::Scheme(const std::string& value) {
//	return URI::Elements(value, "", scheme_suffix);
//}
//
//URI::Elements URI::Elements::Host(const std::string& value) {
//	return URI::Elements(value);
//}
//
//URI::Elements URI::Elements::Port(const std::string& value) {
//	return URI::Elements(value, port_prefix);
//}
//
//URI::Elements URI::Elements::Path(const std::string& value) {
//	return URI::Elements(value, path_prefix);
//}
//
//URI::Elements URI::Elements::Query(const std::string& value) {
//	return URI::Elements(value, query_prefix);
//}
//
//URI::Elements URI::Elements::Query(const std::multimap<std::string, std::string> &params) {
//	auto value = buildQueryString(params);
//	return URI::Elements::Query(value);
//}

class URIImpl : public URI {
public:
	URIImpl(const std::string &raw) {
		auto input = raw;
		
		_scheme = extractIncluding(input, scheme_suffix);
		
		_query = Query::parse(extractFrom(input, query_prefix));
		_path = extractFrom(input, "/");
		
		_port = extractFrom(input, ":");
		if(!_port.empty()) {
			_port.erase(_port.begin()); //Remove ":"
		}

		_host = input;
	}
	
	virtual std::string getScheme() override {
		return _scheme;
	}
	
	virtual void setScheme(const std::string &value) override {
		if(! value.ends_with(scheme_suffix))
			_scheme = value + scheme_suffix;
		else
			_scheme = value;
	}
	
	virtual std::string getHost() override {
		return _host;
	}
	
	virtual void setHost(const std::string &value) override {
		_host = value;
	}
	
	virtual std::string getPort() override {
		return _port;
	}
	
	virtual void setPort(const std::string &value) override {
		_port = value;
	}
	
	virtual std::string getPath() override {
		return _path;
	}
	
	virtual void setPath(const std::string &value) override {
		if(! value.starts_with(path_prefix))
			_path = path_prefix + value;
		else
			_path = value;
	}
	
	virtual Query* getQuery() override {
		return _query.get();
	}
	
	virtual void setQuery(std::unique_ptr<Query> query) override {
		_query = std::move(query);
	}
	
	virtual std::string toString() override {
		auto raw = std::string();
		if(! _scheme.empty()) raw += _scheme;
		if(! _host.empty()) raw += _host;
		if(! _port.empty()) raw += port_prefix + _port;
		if(! _path.empty()) raw += _path;
		
		auto queryString = _query->dump();
		if(! queryString.empty()) raw += queryString;
		
		return raw;
	}

private:
	std::string _scheme, _host, _port, _path;
	std::unique_ptr<Query> _query;
	
	std::multimap<std::string, std::string> parseQueryString(const std::string& queryString) {
		if(queryString.empty())
			return {};

		std::multimap<std::string, std::string> params;
		
		std::vector<std::string> keyValuePairs;
		std::string remaining = queryString;
		
		size_t pos = 0;
		while(pos != std::string::npos) {
			pos = remaining.find("&");
			keyValuePairs.push_back(remaining.substr(0, pos));
			remaining = remaining.substr(pos + 1);
		}
		
		for(auto &pair : keyValuePairs) {
			pos = pair.find("=");
			params.emplace(pair.substr(0, pos), pair.substr(pos + 1));
		}
		
		return params;
	}
	
	std::string extractFrom(std::string& input, const std::string& pattern) {
		auto pos = input.find(pattern);
		if(pos != std::string::npos) {
			auto extract = input.substr(pos);
			input.erase(pos);
			return extract;
		}
		return "";
	}

	std::string extractIncluding(std::string& input, const std::string& pattern) {
		auto pos = input.find(pattern);
		if(pos != std::string::npos) {
			auto extract = input.substr(0, pos + pattern.length());
			input.erase(0, pos + pattern.length());
			return extract;
		}
		return "";
	}
};

std::unique_ptr<URI> URI::parse(const std::string &raw) {
	return std::make_unique<URIImpl>(raw);
}




//URI::Query::~Query() {}
//
//URI::Query::Query(const std::multimap<std::string, std::string> &parameters) {}
//URI::Query URI::Query::parse(const std::string &queryString) {}
//
//void URI::Query::add(const std::string &key, const std::string &value) {}
//void URI::Query::remove(const std::string &key) {}
//void URI::Query::modify(const std::string &key, const std::string &value) {}
//std::string URI::Query::getValue(const std::string &key) {}
//std::multimap<std::string, std::string>* URI::Query::get() {}
//std::string URI::Query::toString() {}
//
////private
//URI::Query::Query() {}
//	
//URI::URI () {}
//URI::~URI () {}
//
//URI URI::parse(const std::string& rawUri) {}
//
//constexpr std::string& URI::getScheme() {
//	return _scheme;
//}
//
//void URI::setScheme(const std::string &scheme) {
//	
//}
//
//constexpr std::string& URI::getHost() {
//	return _host;
//}
//
//void URI::setHost(const std::string &host) {
//	
//}
//
//constexpr std::string& URI::getPort() {
//	return _port;
//}
//
//void URI::setPort(const std::string &port) {
//	
//}
//
//constexpr std::string& URI::getPath() {
//	return _path;
//}
//
//void URI::setPath(const std::string &path) {
//	
//}
//
//constexpr URI::Query* URI::getQuery() {
//	return &_query;
//}
//
//void URI::setQuery(const Query &query) {
//	
//}
//
////BaseURL is host + port
//constexpr std::string& URI::getBaseURL() {
//	if(_port.empty())
//		return _host;
//	
//	return _host;//TBD host ü o´port
//}
//constexpr std::string& URI::toString() {
//	return _host;
//}

//class URIImpl : public URI {
//public:
//	URIImpl(std::string raw) {
//		auto input = raw;
//
//		_scheme = extractUntil(raw, "://");
//		if(!_scheme.empty()) raw.erase(0, 3);
//
//		_query = extractFrom(raw, "?");
//		if(!_query.empty()) _query.erase(_query.begin());
//
//		_path = extractFrom(raw, "/");
//
//		_port = extractFrom(raw, ":");
//		if(!_port.empty()) _port.erase(_port.begin());
//
//		_host = raw;
//	}
//
//	virtual std::string getScheme() override {
//		return _scheme;
//	}
//
//	virtual std::string getHost() override {
//		return _host;
//	}
//
//	virtual std::string getPort() override {
//		return _port;
//	}
//
//	virtual std::string getPath() override {
//		return _path;
//	}
//
//	virtual std::string getQueryString() override {
//		return _query;
//	}
//
//	virtual std::string getBaseURL() override {
//		auto baseURL = _host;
//		if(! _port.empty()) baseURL += ":" + _port;
//		return baseURL;
//	}
//
//	virtual void setPort(size_t port) override {
//		_port = std::to_string(port);
//	}
//
//	std::string toString() override {
//		auto raw = std::string();
//		if(! _scheme.empty()) raw += _scheme + "://";
//		if(! _host.empty()) raw += _host;
//		if(! _port.empty()) raw += ":" + _port;
//		if(! _path.empty()) raw += _path;
//		if(! _query.empty()) raw += "?" + _query;
//		return raw;
//	}
//
//private:
//	std::string _scheme, _host, _port, _path, _query;
//
//	std::string extractFrom(std::string& input, const std::string& pattern) {
//		auto pos = input.find(pattern);
//		if(pos != std::string::npos) {
//			auto extract = input.substr(pos);
//			input.erase(pos);
//			return extract;
//		}
//		return "";
//	}
//
//	std::string extractUntil(std::string& input, const std::string& pattern) {
//		auto pos = input.find(pattern);
//		if(pos != std::string::npos) {
//			auto extract = input.substr(0, pos);
//			input.erase(0, pos);
//			return extract;
//		}
//		return "";
//	}
//};
//
//std::unique_ptr<URI> URI::parse(const std::string& rawUri) {
//	return std::make_unique<URIImpl>(rawUri);
//}
