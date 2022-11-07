// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/detail/encoder.h>

#include <iostream>
#include <regex>

namespace salzaverde {
	class Regex {
	public:
		struct Match {
			std::string content {};
			std::vector<std::string> groups {};
		};
		
		Regex(const std::regex& regex) : _regex(regex) {}
		
		std::vector<Match> search(const std::string& input) {
			std::vector<Match> matches;
			std::smatch smatch;
			std::string remaining = input;
			while(regex_search(remaining, smatch, _regex)) {
				auto match = smatchToMatch(smatch);
				matches.push_back(match);
				remaining = smatch.suffix();
			}
			return matches;
		}
		
	private:
		std::regex _regex;
		
		Match smatchToMatch(const std::smatch &smatch) {
			Match output;
			output.content = smatch.str();
			for(auto group : smatch) {
				output.groups.push_back(group.str());
			}
			return output;
		}
	};
	
	static std::string hexToString(const std::string &hex) {
		int len = hex.length();
		std::string output;
		for(int i=0; i< len; i+=2) {
			std::string byte = hex.substr(i,2);
			char chr = (char) (int) strtol(byte.c_str(), nullptr, 16);
			output.push_back(chr);
		}
		return output;
	}
	
	std::string URLEncoding::encode(const std::string &raw) {
		return "";
	}
	
	std::string URLEncoding::decode(const std::string &encoded) {
		auto regex = Regex(std::regex(R"((.*?)(%[a-fA-F0-9]{2})|(.+))"));
		auto matches = regex.search(encoded);
		
		std::string result = "";
		
		for(auto &match : matches) {
			assert(match.groups.size() == 4);
			if(! match.groups[1].empty()) result += match.groups[1];
			if(! match.groups[2].empty()) result += hexToString(match.groups[2].substr(1));
			if(! match.groups[3].empty()) result += match.groups[3];
		}
		
		return result;
	}
}
