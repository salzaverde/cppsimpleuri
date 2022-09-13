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
	static const std::string fragment_prefix = "#";

    class URIImpl : public URI {
    public:
        URIImpl(std::string raw) {
			//Regex parsing contributed by Andreas Schütz.
			//Taken from RFC3986 and modified to extract host and port separately from 'authority', while still supporting IPv6 addresses.
			
			std::smatch results;
			const std::regex regex(R"(^(([^:/?#]+):)?(//((\[.*\]|[^/?#:]+)*(:(\d+))?))?([^?#]*)(\?([^#]*))?(#(.*))?)");
			std::regex_match(raw, results, regex);
			
			_scheme = results[2].str();
			_host = results[5].str();
			_port = results[7].str();
			_path = results[8].str();
			_query = results[10].str();
			_fragment = results[12].str();
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
        
        virtual std::string getQuery() override {
            return _query;
        }
        
        virtual void setQuery(const std::string &value) override {
			if(value.starts_with(query_prefix))
				_query = value.substr(query_prefix.length());
			else
				_query = value;
        }
		
		virtual std::string getFragment() override {
			return _fragment;
		}
		
		virtual void setFragment(const std::string &value) override {
			if(value.starts_with(fragment_prefix))
				_fragment = value.substr(fragment_prefix.length());
			else
				_fragment = value;
		}
        
        virtual std::string dump() override {
            auto raw = std::string();
            if(! _scheme.empty()) raw += _scheme + scheme_suffix;
            if(! _host.empty()) raw += _host;
            if(! _port.empty()) raw += port_prefix + _port;
            if(! _path.empty()) raw += _path;
			if(! _query.empty()) raw += query_prefix + _query;
			if(! _fragment.empty()) raw += fragment_prefix + _fragment;
            return raw;
        }

    private:
        std::string _scheme, _host, _port, _path, _query, _fragment;
	};
	
    std::unique_ptr<URI> URI::parse(const std::string &raw) {
        return std::make_unique<URIImpl>(raw);
    }
}
