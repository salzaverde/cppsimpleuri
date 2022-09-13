// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <gtest/gtest.h>
#include <salzaverde/uri.h>

using namespace salzaverde;

TEST(URITest, Parse) {
    auto raw = "http://example.com:1234/to/location?key1=value1&key2=value2#keyA=valueA";
    auto uri = URI::parse(raw);

    EXPECT_EQ(uri->getScheme(), "http");
    EXPECT_EQ(uri->getHost(), "example.com");
    EXPECT_EQ(uri->getPort(), "1234");
    EXPECT_EQ(uri->getPath(), "/to/location");
    EXPECT_EQ(uri->getQuery(), "key1=value1&key2=value2");
	EXPECT_EQ(uri->getFragment(), "keyA=valueA");
};

TEST(URITest, Change) {
    auto raw = "http://example.com:1234/to/location?key1=value1&key2=value2";
    auto uri = URI::parse(raw);

    uri->setScheme("https");
    uri->setHost("example2.com");
    uri->setPort("5678");
    uri->setPath("to/other");
    uri->setQuery("key3=value3");
	uri->setFragment("keyB=valueB");

    EXPECT_EQ(uri->getScheme(), "https");
    EXPECT_EQ(uri->getHost(), "example2.com");
    EXPECT_EQ(uri->getPort(), "5678");
    EXPECT_EQ(uri->getPath(), "/to/other");
	EXPECT_EQ(uri->getQuery(), "key3=value3");
	EXPECT_EQ(uri->getFragment(), "keyB=valueB");
};

TEST(URITest, ToString) {
    auto raw = "http://example.com:1234/to/location?key1=value1&key2=value2#keyA=valueA";
    auto uri = URI::parse(raw);
    EXPECT_EQ(uri->dump(), raw);
};

TEST(URITest, Scheme) {
    auto raw = "http://";
    auto uri = URI::parse(raw);
    EXPECT_EQ(uri->getScheme(), "http");
};

TEST(URITest, Path) {
    auto raw = "/some/path";
    auto uri = URI::parse(raw);
    EXPECT_EQ(uri->getPath(), raw);
};

TEST(URITest, QueryString) {
    auto raw = "?key1=value1";
    auto uri = URI::parse(raw);
    EXPECT_EQ(uri->getQuery(), "key1=value1");
};

TEST(URITest, Fragment) {
	auto raw = "#keyC=valueC";
	auto uri = URI::parse(raw);
	EXPECT_EQ(uri->getFragment(), "keyC=valueC");
};
