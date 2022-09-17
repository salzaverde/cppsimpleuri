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
		
		void generate (std::vector<UriComponents::Type> types) {
			std::vector<std::vector<std::string>> inputData;
			for(auto type : types)
					inputData.push_back(components[type]);
			
			std::vector<TestURI> output;
			auto combinations = CartesianProduct<std::string>(inputData);
			for(auto &uriElements : combinations.get()) {
				TestURI element;
				auto it = uriElements.begin();
				for(auto type : types) {
					element.raw += getPrefix(type) + *it;
					element.components[type] = *it++;
				}
				output.push_back(element);
			}
			
			uris.insert(uris.end(), output.begin(), output.end());
		}
		
		std::vector<TestURI> pop() {
			return std::move(uris);
		}
		
	private:
		UriComponents components;
		std::vector<TestURI> uris;
		
		std::string getPrefix(UriComponents::Type type) {
			if(type == UriComponents::Type::userinfo)
				return "//";
			
			if(type == UriComponents::Type::host && !((type - 1) == UriComponents::Type::userinfo))
				return "//";
			
			if(type == UriComponents::Type::port)
				return ":";
			
			if(type == UriComponents::Type::query)
				return "?";
			
			if(type == UriComponents::Type::fragment)
				return "#";
			
			return "";
		}
	};
}
