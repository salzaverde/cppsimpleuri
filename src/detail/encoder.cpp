// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <salzaverde/detail/encoder.h>

namespace salzaverde {
	std::string URLEncoding::encode(const std::string &raw) {
		return "";
	}
	
	std::string URLEncoding::decode(const std::string &encoded) {
		int len = encoded.length();
		std::string str;
		for(int i=0; i< len; i+=2) {
			std::string byte = encoded.substr(i,2);
			char chr = (char) (int) strtol(byte.c_str(), nullptr, 16);
			str.push_back(chr);
		}
		return str;
	}
}
