// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#pragma once

#include <string>

namespace salzaverde {
	struct URLEncoding {
		/**
		 * @brief Applies percent-encoding to a raw string for all
		 * non alpha-numeric characters despite the provided excluded
		 * characters.
		*/
		static std::string encode(const std::string &raw, const std::string &excludedCharacters = "-._~");

		/**
		 * @brief Decodes any percent-encoding found in "encoded" and
		 * returns the decoded string.
		*/
		static std::string decode(const std::string &encoded);
	};
}
