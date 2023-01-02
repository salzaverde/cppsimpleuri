// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <string>

namespace salzaverde {
	struct URLEncoding {
		static std::string encode(const std::string &raw, const std::string &excludedCharacters = "-._~");
		static std::string decode(const std::string &encoded);
	};
}
