#include "uri.h"



URI::UniqueURI URI::UniqueURI::make(const std::string& value, const std::string& prefix, const std::string& suffix) {
	return URI::UniqueURI(URI::create(value, prefix, suffix).release());
}

URI::UniqueURI URI::UniqueURI::operator| (const UniqueURI &other) {
	return make((*this)->getValue() + (*this)->getSuffix() + other->getPrefix() + other->getValue(), "", "");
}

class URIImpl : public URI {
public:
	URIImpl(const std::string& value, const std::string& prefix, const std::string& suffix) : _value(value), _prefix(prefix), _suffix(suffix) {}
	
	virtual std::string getValue() override {
		return _value;
	}
	
	virtual std::string getPrefix() override {
		return _prefix;
	}
	
	virtual std::string getSuffix() override {
		return _suffix;
	}
	
	virtual std::string toString() override {
		return _prefix + _value + _suffix;
	}

private:
	std::string _prefix = "";
	std::string _suffix = "";
	std::string _value = "";
};

URI::UniqueURI URI::Scheme(const std::string& value) {
	return URI::UniqueURI::make(value, "", "://");
}
URI::UniqueURI URI::Host(const std::string& value) {
	return URI::UniqueURI::make(value, "", "");
}
URI::UniqueURI URI::Port(const std::string& value) {
	return URI::UniqueURI::make(value, ":", "");
}

std::unique_ptr<URI> URI::create(const std::string& value, const std::string& prefix, const std::string& suffix) {
	return std::make_unique<URIImpl>(value, prefix, suffix);
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
