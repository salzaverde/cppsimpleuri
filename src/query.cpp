// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/query.h>
#include <vector>

namespace salzaverde {
    static const std::string query_prefix = "?";

    class QueryImpl : public Query {
    public:
        explicit QueryImpl(const std::string &raw) {
            parse(raw);
        }
        
        explicit QueryImpl(const std::map<std::string, std::string> &params) : _params(params) {}
        
        virtual std::string dump() override {
            if(_params.empty())
                return "";
            
            std::string queryString = query_prefix;
            auto it = _params.begin();
            queryString += it->first + "=" + it->second;
            while(++it != _params.end()) {
                queryString += "&" + it->first + "=" + it->second;
            }
            return queryString;
        }
        
        virtual std::optional<std::string> get(const std::string &key) override {
            if(_params.contains(key))
                return _params[key];
            
            return {};
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

    std::unique_ptr<Query> Query::parse(const std::string &raw) {
        return std::make_unique<QueryImpl>(raw);
    }

    std::unique_ptr<Query> Query::build(const std::map<std::string, std::string> &params) {
        return std::make_unique<QueryImpl>(params);
    }
}
