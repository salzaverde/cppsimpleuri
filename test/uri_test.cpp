#include <gtest/gtest.h>
#include <uri.h>

//TEST(URITestSuite, Builder) {
//	auto scheme = URI::Elements::Scheme("http");
//	auto host = URI::Elements::Host("example.com");
//	auto port = URI::Elements::Port("1234");
//	auto path = URI::Elements::Path("to/location");
//	auto query = URI::Elements::Query("key1=value1&key2=value2");
//
//	auto combinedElements = scheme|host|port|path|query;
//	EXPECT_EQ(combinedElements.toString(), "http://example.com:1234/to/location?key1=value1&key2=value2");
//};

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
//
//TEST(URITestSuite, RemoveFromQuery) {
//	auto raw = "?key1=value1&key2=value2";
//	auto uri = URI::parse(raw);
//	EXPECT_EQ(uri->toString(), "?key1=value1&key2=value2");
//
//	uri->removeFromQuery("key1");
//	EXPECT_EQ(uri->toString(), "?key2=value2");
//};
//
//TEST(URITestSuite, AddQueryParam) {
//	auto raw = "?key1=value1&key2=value2";
//	auto uri = URI::parse(raw);
//	EXPECT_EQ(uri->toString(), "?key1=value1&key2=value2");
//
//	uri->addToQuery("key1", "value3");
//	EXPECT_EQ(uri->toString(), "?key1=value1&key2=value2&key1=value3");
//};

//TEST(URITestSuite, UpdateQuery) {
//	auto uri = URI::Query("key1=value1&key2=value2");
//	EXPECT_EQ(uri->toString(), "?key1=value1&key2=value2");
//
//	uri->addToQuery("key1", "changedValue");
//	EXPECT_EQ(uri->toString(), "?key1=changedValue&key2=value2");
//};
//
//TEST(URITestSuite, QueryFromMap) {
//	std::multimap<std::string, std::string> queryParams;
//	queryParams.emplace("key1", "value1");
//	queryParams.emplace("key2", "value2");
//	queryParams.emplace("key2", "value3");
//
//	auto uri = URI::Query(queryParams);
//	EXPECT_EQ(uri->toString(), "?key1=value1&key2=value2&key2=value3");
//};

//class QueryParser {
//public:
//	virtual ~QueryParser() {}
//	static std::unique_ptr<QueryParser> parse(const std::string &rawQuery);
//	static std::unique_ptr<QueryParser> build(const std::multimap<std::string, std::string> &params);
//
//	virtual std::string dump() = 0;
//	virtual std::string get(const std::string &key) = 0;
//	virtual std::string set(const std::string &key, const std::string &value) = 0;
//	virtual void erase(const std::string &key) = 0;
//	virtual bool contains(const std::string &key) = 0;
//};


TEST(QueryTest, Dump) {
	auto raw = std::string("?key1=val1&key2=val2");
	auto query = URI::Query::parse(raw);

	EXPECT_EQ(query->dump(), raw);
}

TEST(QueryTest, Build) {
	std::map<std::string, std::string> params;
	params.emplace("key1", "val1");
	params.emplace("key2", "val2");
	auto query = URI::Query::build(params);
	
	EXPECT_EQ(query->dump(), "?key1=val1&key2=val2");
}

TEST(QueryTest, Contains) {
	auto query = URI::Query::parse("?key1=val1&key2=val2");

	EXPECT_TRUE(query->contains("key1"));
	EXPECT_TRUE(query->contains("key2"));
}

TEST(QueryTest, Get) {
	auto raw = std::string("?key1=val1&key2=val2");
	auto query = URI::Query::parse(raw);

	EXPECT_EQ(query->get("key1"), "val1");
	EXPECT_EQ(query->get("key2"), "val2");
}

TEST(QueryTest, ListKeys) {
	auto raw = std::string("?key1=val1&key2=val2");
	auto query = URI::Query::parse(raw);
	auto keys = query->listKeys();
	
	EXPECT_TRUE(keys.contains("key1"));
	EXPECT_TRUE(keys.contains("key2"));
}

TEST(QueryTest, Set) {
	auto raw = std::string("?key1=val1&key2=val2");
	auto query = URI::Query::parse(raw);
	query->set("key3", "val3");
	
	EXPECT_TRUE(query->contains("key3"));
	EXPECT_EQ(query->get("key3"), "val3");
}

TEST(QueryTest, SetExisting) {
	auto raw = std::string("?key1=val1&key2=val2");
	auto query = URI::Query::parse(raw);
	query->set("key1", "val3");
	
	EXPECT_EQ(query->get("key1"), "val3");
}

TEST(QueryTest, Erase) {
	auto query = URI::Query::parse("?key1=val1&key2=val2");
	query->erase("key1");
	
	EXPECT_FALSE(query->contains("key1"));
}

TEST(QueryTest, Empty) {
	auto query = URI::Query::parse("");
	EXPECT_EQ(query->dump(), "");
}
