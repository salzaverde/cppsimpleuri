// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <gtest/gtest.h>
#include <salzaverde/uri.h>

#include <config/uri_components.h>
#include <detail/uri_generator.h>


using namespace salzaverde;

class URIParsing : public testing::TestWithParam<UriGenerator::TestURI> {
public:
	void SetUp() override {
		element = GetParam();
		uri = URI::parse(element.raw);
	}

	protected:
	UriGenerator::TestURI element;
	std::unique_ptr<URI> uri;
};

TEST_P(URIParsing, Sample) {
	if(element.components.find(UriComponents::Type::scheme) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::scheme], uri->scheme + ":") << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::userinfo) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::userinfo], uri->userinfo + "@") << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::host) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::host], uri->host) << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::port) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::port], uri->port) << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::path) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::path], uri->path) << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::query) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::query], uri->query) << "Raw URI was:" + element.raw;
	
	if(element.components.find(UriComponents::Type::fragment) != element.components.end())
		EXPECT_EQ(element.components[UriComponents::Type::fragment], uri->fragment) << "Raw URI was:" + element.raw;
	
	EXPECT_EQ(uri->dump(), element.raw);
}

static std::vector<UriGenerator::TestURI> createTestValues() {
	UriGenerator uriGenerator;
	
	uriGenerator.generate();
	uriGenerator.generate({UriComponents::Type::scheme});
	uriGenerator.generate({UriComponents::Type::scheme, UriComponents::Type::userinfo, UriComponents::Type::host});
	uriGenerator.generate({UriComponents::Type::scheme, UriComponents::Type::userinfo, UriComponents::Type::host, UriComponents::Type::port});
	uriGenerator.generate({UriComponents::Type::scheme, UriComponents::Type::userinfo, UriComponents::Type::host, UriComponents::Type::port, UriComponents::Type::path, UriComponents::Type::query, UriComponents::Type::fragment});
	uriGenerator.generate({UriComponents::Type::scheme, UriComponents::Type::path, UriComponents::Type::query, UriComponents::Type::fragment});
	
	return uriGenerator.pop();
}

INSTANTIATE_TEST_SUITE_P(
	ExampleTests,
	URIParsing,
	::testing::ValuesIn(createTestValues())
);


class URISetters : public testing::Test {
public:
	void SetUp() override {
		uri = URI::parse(raw);
	}

protected:
	std::string raw = "http://salzaverde@example.com:1234/to/location?key1=value1&key2=value2#keyA=valueA";
	std::unique_ptr<URI> uri;
};

TEST_F(URISetters, Dump) {
    EXPECT_EQ(uri->dump(), raw);
};


