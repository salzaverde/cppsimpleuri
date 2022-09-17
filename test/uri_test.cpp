// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <gtest/gtest.h>
#include <salzaverde/uri.h>

#include <config/uri_components.h>
#include <detail/uri_generator.h>


using namespace salzaverde;

class URITest2 : public testing::TestWithParam<UriGenerator::TestURI> {
public:
	void SetUp() override {
		element = GetParam();
		uri = URI::parse(element.raw);
	}

	protected:
	UriGenerator::TestURI element;
	std::unique_ptr<URI> uri;
};

TEST_P(URITest2, Sample) {
	if(element.components.find(UriComponents::Type::scheme) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::scheme], uri->getScheme() + ":") << "Raw URI was:" + element.raw;
	
	bool containsUserInfo = element.components.find(UriComponents::Type::userinfo) != element.components.end();
	if(containsUserInfo)
		EXPECT_EQ(element.components[UriComponents::Type::userinfo], "//" + uri->getUserInfo() + "@") << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::host) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::host], (containsUserInfo?"":"//") + uri->getHost()) << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::port) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::port], ":" + uri->getPort()) << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::path) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::path], uri->getPath()) << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::query) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::query], "?" + uri->getQuery()) << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::fragment) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::fragment], "#" + uri->getFragment()) << "Raw URI was:" + element.raw;
	
}

static std::vector<UriGenerator::TestURI> createTestValues() {
	UriGenerator uriGenerator;
	
	uriGenerator.generate({UriComponents::Type::scheme});
	uriGenerator.generate({UriComponents::Type::scheme, UriComponents::Type::userinfo, UriComponents::Type::host});
	uriGenerator.generate({UriComponents::Type::scheme, UriComponents::Type::userinfo, UriComponents::Type::host, UriComponents::Type::port});
	uriGenerator.generate({UriComponents::Type::scheme, UriComponents::Type::userinfo, UriComponents::Type::host, UriComponents::Type::port, UriComponents::Type::path, UriComponents::Type::query, UriComponents::Type::fragment});
	uriGenerator.generate({UriComponents::Type::scheme, UriComponents::Type::path, UriComponents::Type::query, UriComponents::Type::fragment});
	
	return uriGenerator.pop();
}

INSTANTIATE_TEST_SUITE_P(
	ExampleTests,
	URITest2,
	::testing::ValuesIn(createTestValues())
);


class URITest : public testing::Test {
public:
	void SetUp() override {
		uri = URI::parse(raw);
	}

protected:
	std::string raw = "http://salzaverde@example.com:1234/to/location?key1=value1&key2=value2#keyA=valueA";
	std::unique_ptr<URI> uri;
};

TEST_F(URITest, Parse) {
    EXPECT_EQ(uri->getScheme(), "http");
	EXPECT_EQ(uri->getUserInfo(), "salzaverde");
    EXPECT_EQ(uri->getHost(), "example.com");
    EXPECT_EQ(uri->getPort(), "1234");
    EXPECT_EQ(uri->getPath(), "/to/location");
    EXPECT_EQ(uri->getQuery(), "key1=value1&key2=value2");
    EXPECT_EQ(uri->getFragment(), "keyA=valueA");
};

TEST_F(URITest, SetUserInfo) {
	uri->setUserInfo("salzaverde");
	EXPECT_EQ(uri->getUserInfo(), "salzaverde");
};

TEST_F(URITest, SetScheme) {
	uri->setScheme("https");
	EXPECT_EQ(uri->getScheme(), "https");
};

TEST_F(URITest, SetSchemeWithSuffix) {
	uri->setScheme("https://");
	EXPECT_EQ(uri->getScheme(), "https");
};

TEST_F(URITest, SetHost) {
	uri->setHost("example2.com");
	EXPECT_EQ(uri->getHost(), "example2.com");
};

TEST_F(URITest, SetPort) {
	uri->setPort("5678");
	EXPECT_EQ(uri->getPort(), "5678");
};

TEST_F(URITest, SetPath) {
	uri->setPath("to/other");
	EXPECT_EQ(uri->getPath(), "/to/other");
};

TEST_F(URITest, SetPathWithPrefix) {
	uri->setPath("/to/other");
	EXPECT_EQ(uri->getPath(), "/to/other");
};

TEST_F(URITest, SetQuery) {
	uri->setQuery("key3=value3");
	EXPECT_EQ(uri->getQuery(), "key3=value3");
};

TEST_F(URITest, SetQueryWithPrefix) {
	uri->setQuery("?key3=value3");
	EXPECT_EQ(uri->getQuery(), "key3=value3");
};

TEST_F(URITest, SetFragment) {
    uri->setFragment("keyB=valueB");
    EXPECT_EQ(uri->getFragment(), "keyB=valueB");
};

TEST_F(URITest, SetFragmentWithPrefix) {
	uri->setFragment("#keyB=valueB");
	EXPECT_EQ(uri->getFragment(), "keyB=valueB");
};

TEST_F(URITest, ToString) {
    EXPECT_EQ(uri->dump(), raw);
};

TEST(CustomURITest, Scheme) {
    auto uri = URI::parse("http://");
    EXPECT_EQ(uri->getScheme(), "http");
};

TEST(CustomURITest, Path) {
    auto uri = URI::parse("/some/path");
    EXPECT_EQ(uri->getPath(), "/some/path");
};

TEST(URITeCustomURITestst, QueryString) {
    auto uri = URI::parse("?key1=value1");
    EXPECT_EQ(uri->getQuery(), "key1=value1");
};

TEST(CustomURITest, Fragment) {
    auto uri = URI::parse("#keyC=valueC");
    EXPECT_EQ(uri->getFragment(), "keyC=valueC");
};

TEST(CustomURITest, IPv6) {
	auto uri = URI::parse("https://[2001:db8:2a:3256:adfe:5c0:3:6]:1176/to/location?key1=value1&key2=value2#keyA=valueA");
	EXPECT_EQ(uri->getHost(), "[2001:db8:2a:3256:adfe:5c0:3:6]");
	EXPECT_EQ(uri->getPort(), "1176");
	EXPECT_EQ(uri->getQuery(), "key1=value1&key2=value2");
};

TEST(CustomURITest, Empty) {
    auto uri = URI::parse("");
    EXPECT_EQ(uri->dump(), "");
};


