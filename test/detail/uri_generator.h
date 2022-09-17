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
			bool isPrefixed = false;
			for(auto type : types) {
				if((type == UriComponents::Type::host || type == UriComponents::Type::userinfo) && (!isPrefixed)) {
					auto prefixed = components[type];
					for(auto &element : prefixed) {
						element = "//" + element;
					}
					inputData.push_back(prefixed);
					isPrefixed = true;
				}
				else
					inputData.push_back(components[type]);
			}
			
			std::vector<TestURI> output;
			auto combinations = CartesianProduct<std::string>(inputData);
			for(auto &combination : combinations.get()) {
				TestURI element;
				auto it = combination.begin();
				for(auto type : types) {
					element.raw += *it;
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
	};
}
