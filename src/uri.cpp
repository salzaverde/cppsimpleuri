// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/uri.h>
#include <vector>
#include <regex>

#include <iostream>

namespace salzaverde {
    static const std::string scheme_suffix = "://";
	static const std::string userinfo_suffix = "@";
    static const std::string path_prefix = "/";
    static const std::string port_prefix = ":";
    static const std::string query_prefix = "?";
    static const std::string fragment_prefix = "#";

    class URIImpl : public URI {
    public:
        explicit URIImpl(std::string raw) {
			//The original proposed regex from RFC 3986 splits the uri into scheme | authority | path | query | fragment
			auto RFC3986Results= matchRegex(raw, std::regex(R"(^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)"));
			
			//Split the authority into userinfo, host and port
			auto authority = RFC3986Results[4].str();
			auto authorityResults = matchRegex(authority, std::regex(R"(^(([^@]*)@)?((\[(.*)\]|[^:]+):)?(.*)?)"));
			
			_scheme = RFC3986Results[2].str();
			_userinfo = authorityResults[2].str();
			_host = authorityResults[4].str();
			_port = authorityResults[6].str();
            _path = RFC3986Results[5].str();
            _query = RFC3986Results[7].str();
            _fragment = RFC3986Results[9].str();
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
        
		virtual std::string getUserInfo() override {
			return _userinfo;
		}
		
		virtual void setUserInfo(const std::string &value) override {
			_userinfo = value;
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
			if(! _userinfo.empty()) raw += _userinfo + userinfo_suffix;
            if(! _host.empty()) raw += _host;
            if(! _port.empty()) raw += port_prefix + _port;
            if(! _path.empty()) raw += _path;
            if(! _query.empty()) raw += query_prefix + _query;
            if(! _fragment.empty()) raw += fragment_prefix + _fragment;
            return raw;
        }

    private:
		std::smatch matchRegex(const std::string &input, const std::regex &regex) {
			std::smatch results;
			std::regex_match(input, results, regex);
			return results;
		}
		
        std::string _scheme, _userinfo, _host, _port, _path, _query, _fragment;
    };
    
    std::unique_ptr<URI> URI::parse(const std::string &raw) {
        return std::make_unique<URIImpl>(raw);
    }
}
