// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <gtest/gtest.h>
#include <salzaverde/detail/encoder.h>

using namespace salzaverde;

TEST(URLDecoding, Space) {
	auto hexString = std::string("%20");
	auto result = URLEncoding::decode(hexString);

	EXPECT_EQ(result, " ");
}

TEST(URLEncoding, Space) {
	auto rawString = std::string(" ");
	auto result = URLEncoding::encode(rawString);

	EXPECT_EQ(result, "%20");
}

TEST(URLDecoding, TwoSpaces) {
	auto hexString = std::string("%20%20");
	auto result = URLEncoding::decode(hexString);

	EXPECT_EQ(result, "  ");
}

TEST(URLEncoding, TwoSpaces) {
	auto hexString = std::string("  ");
	auto result = URLEncoding::encode(hexString);

	EXPECT_EQ(result, "%20%20");
}

TEST(URLDecoding, CopyrightCharacter) {
	auto hexString = std::string("%c2%a9");
	auto result = URLEncoding::decode(hexString);

	EXPECT_EQ(result, "©");
}

TEST(URLEncoding, CopyrightCharacter) {
	auto hexString = std::string("©");
	auto result = URLEncoding::encode(hexString);

	EXPECT_EQ(result, "%c2%a9");
}

TEST(URLDecoding, CopyrightAddress) {
	auto hexString = std::string("%c2%a9salzaverde%40github.com");
	auto result = URLEncoding::decode(hexString);

	EXPECT_EQ(result, "©salzaverde@github.com");
}

TEST(URLEncoding, CopyrightAddress) {
	auto hexString = std::string("©salzaverde@github.com");
	auto result = URLEncoding::encode(hexString);

	EXPECT_EQ(result, "%c2%a9salzaverde%40github.com");
}
