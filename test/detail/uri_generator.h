// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <config/uri_components.h>
#include <detail/cartesian_product.h>

namespace salzaverde {
    class UriGenerator {
    public:
        struct TestURI {
            std::string raw = "";
            std::map<UriComponents::Type, std::string> components;
        };
        
        UriGenerator() {}
        
        void generate (const std::vector<UriComponents::Type> &types = {}) {
            if(types.empty())
                uris.push_back({""});
            
            std::vector<std::vector<std::string>> inputData;
            for(auto type : types)
                inputData.push_back(components[type]);
            
            std::vector<TestURI> output;
            auto combinations = CartesianProduct<std::string>(inputData);
            for(auto &uriElements : combinations.get()) {
                TestURI testUri;
                auto it = uriElements.begin();
                for(auto type : types) {
                    auto prefix = getPrefix(type, types);
                    testUri.raw += prefix + *it;
                    testUri.components[type] = type == UriComponents::Type::path? prefix + *it++ : *it++;
                }
                output.push_back(testUri);
            }
            
            uris.insert(uris.end(), output.begin(), output.end());
        }
        
        std::vector<TestURI> pop() {
            return std::move(uris);
        }
        
    private:
        UriComponents components;
        std::vector<TestURI> uris;
        
        std::string getPrefix(UriComponents::Type type, const std::vector<UriComponents::Type> &types) {
            if(type == UriComponents::Type::userinfo)
                return "//";
            
            if(type == UriComponents::Type::host && !((type - 1) == UriComponents::Type::userinfo))
                return "//";
            
            if(type == UriComponents::Type::port)
                return ":";
            
            if(type == UriComponents::Type::path && hasAuthority(types))
                return "/";
            
            if(type == UriComponents::Type::query)
                return "?";
            
            if(type == UriComponents::Type::fragment)
                return "#";
            
            return "";
        }
        
        bool hasAuthority(const std::vector<UriComponents::Type> &types) {
            for(auto type : types)
                if(type == UriComponents::Type::host) return true;

            return false;
        };
    };
}
