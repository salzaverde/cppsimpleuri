// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/uri.h>
#include <vector>

namespace salzaverde {
    static const std::string scheme_suffix = "://";
    static const std::string path_prefix = "/";
    static const std::string port_prefix = ":";
    static const std::string query_prefix = "?";

    class URIImpl : public URI {
    public:
        URIImpl(std::string raw) {
            _scheme = extractIncluding(raw, scheme_suffix);
            _query = Query::parse(extractFrom(raw, query_prefix));
            _path = extractFrom(raw, path_prefix);
            _port = extractFrom(raw, port_prefix);
            if(!_port.empty())
                _port.erase(0, port_prefix.length()); //Remove port prefix
            _host = raw;
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
        
        virtual std::string dump() override {
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
}
