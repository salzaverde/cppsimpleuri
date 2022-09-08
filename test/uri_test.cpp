#include <gtest/gtest.h>
#include <salzaverde/uri.h>

using namespace salzaverde;

TEST(URITest, Parse) {
	auto raw = "http://example.com:1234/to/location?key1=value1&key2=value2";
	auto uri = URI::parse(raw);

	EXPECT_EQ(uri->getScheme(), "http://");
	EXPECT_EQ(uri->getHost(), "example.com");
	EXPECT_EQ(uri->getPort(), "1234");
	EXPECT_EQ(uri->getPath(), "/to/location");
	EXPECT_EQ(uri->getQuery()->dump(), "?key1=value1&key2=value2");
};

TEST(URITest, Change) {
	auto raw = "http://example.com:1234/to/location?key1=value1&key2=value2";
	auto uri = URI::parse(raw);

	uri->setScheme("https");
	uri->setHost("example2.com");
	uri->setPort("5678");
	uri->setPath("to/other");
	uri->setQuery(URI::Query::parse("key3=value3"));

	EXPECT_EQ(uri->getScheme(), "https://");
	EXPECT_EQ(uri->getHost(), "example2.com");
	EXPECT_EQ(uri->getPort(), "5678");
	EXPECT_EQ(uri->getPath(), "/to/other");
	EXPECT_EQ(uri->getQuery()->dump(), "?key3=value3");
};

TEST(URITest, ToString) {
	auto raw = "http://example.com:1234/to/location?key1=value1&key2=value2";
	auto uri = URI::parse(raw);
	EXPECT_EQ(uri->toString(), raw);
};

TEST(URITest, PlainString) {
	auto raw = "http"; //Can't be parsed as scheme without ://, so interpreted as host
	auto uri = URI::parse(raw);
	EXPECT_EQ(uri->getHost(), raw);
};

TEST(URITest, Scheme) {
	auto raw = "http://"; //Can't be parsed as scheme without ://, so interpreted as host
	auto uri = URI::parse(raw);
	EXPECT_EQ(uri->getScheme(), raw);
};

TEST(URITest, Path) {
	auto raw = "/some/path"; //Can't be parsed as scheme without ://, so interpreted as host
	auto uri = URI::parse(raw);
	EXPECT_EQ(uri->getPath(), raw);
};

TEST(URITest, QueryString) {
	auto raw = "?key1=value1"; //Can't be parsed as scheme without ://, so interpreted as host
	auto uri = URI::parse(raw);
	EXPECT_EQ(uri->getQuery()->dump(), raw);
};
