#include <salzaverde/uri.h>
#include <vector>

static const std::string scheme_suffix = "://";
static const std::string path_prefix = "/";
static const std::string port_prefix = ":";
static const std::string query_prefix = "?";

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
