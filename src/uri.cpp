// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/uri.h>
#include <vector>
#include <regex>

#include <iostream>

namespace salzaverde {
    static const std::string scheme_suffix = "://";
    static const std::string path_prefix = "/";
    static const std::string port_prefix = ":";
    static const std::string query_prefix = "?";

    class URIImpl : public URI {
    public:
        URIImpl(std::string raw) {
			std::smatch results;
			const std::regex regex(R"(^(([^:/?#]+):)?(//((\[.*\]|[^/?#:]+)*(:(\d+))?))?([^?#]*)(\?([^#]*))?(#(.*))?)");
			std::regex_match(raw, results, regex);
			
			_scheme = results[2].str();
			_host = results[5].str();
			_port = results[7].str();
			_path = results[8].str();
			_query = Query::parse(results[10].str());
        }
        
        virtual std::string getScheme() override {
            return _scheme;
        }
        
        virtual void setScheme(const std::string &value) override {
            if(value.ends_with(scheme_suffix))
                _scheme = value.substr(0, value.length() - scheme_suffix.length());
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
        
        virtual std::string dump() override {
            auto raw = std::string();
            if(! _scheme.empty()) raw += _scheme + scheme_suffix;
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
	};
	
    std::unique_ptr<URI> URI::parse(const std::string &raw) {
        return std::make_unique<URIImpl>(raw);
    }
}
