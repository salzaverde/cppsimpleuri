// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/detail/encoder.h>

#include <iostream>
#include <regex>

static const std::string hexEntries = "0123456789abcdef";

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
	
	class Conversions {
	public:
		static std::string hexToString(const std::string &hex) {
			int len = hex.length();
			std::string output;
			for(int i=0; i< len; i+=2) {
				std::string byte = hex.substr(i,2);
				char character = (char) (int) strtol(byte.c_str(), nullptr, 16);
				output.push_back(character);
			}
			return output;
		}

		static std::string intToHex(int integer) {
			auto hexIndex0 = integer / 16;
			auto hexIndex1 = integer % 16;
			
			std::string result = "";
			result += hexEntries[hexIndex0];
			result += hexEntries[hexIndex1];

			return result;
		}

		static std::string charToHex(const char& raw) {
			auto integer = static_cast<uint8_t>(raw);
			return intToHex(integer);
		}
	};

	static bool isUnreserved(const char* character, const std::string &excludedCharacters) {
		auto regex = Regex(std::regex("([a-zA-Z0-9]|[" + excludedCharacters + "])"));
		auto matches = regex.search(std::string(character));
		if(matches.size() > 0 && matches[0].content[0] == *character) 
			return true;

		return false;
	}

	std::string URLEncoding::encode(const std::string &raw, const std::string &excludedCharacters) {
		std::string result = "";
		for(auto character : raw) {
			if(isUnreserved(&character, excludedCharacters))
				result += character;
			else
				result += "%" + Conversions::charToHex(character);
		}
		return result;
	}
	
	std::string URLEncoding::decode(const std::string &encoded) {
		auto regex = Regex(std::regex(R"((.*?)(%[a-fA-F0-9]{2})|(.+))"));
		auto matches = regex.search(encoded);
		
		std::string result = "";
		
		for(auto &match : matches) {
			if(! match.groups[1].empty()) result += match.groups[1];
			if(! match.groups[2].empty()) result += Conversions::hexToString(match.groups[2].substr(1));
			if(! match.groups[3].empty()) result += match.groups[3];
		}
		
		return result;
	}
}
