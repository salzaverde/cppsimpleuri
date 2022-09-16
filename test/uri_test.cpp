// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <gtest/gtest.h>
#include <salzaverde/uri.h>

using namespace salzaverde;

template<typename T> class CartesianProduct {
public:
	CartesianProduct(std::vector<std::vector<T>> sequences) {
		std::vector<std::vector<T>> accum;
		std::vector<T> stack;
		if (sequences.size() > 0)
			recurse(accum, stack, sequences);
		
		_result = accum;
	}
	
	std::vector<std::vector<T>> get() {
		return _result;
	}
private:
	std::vector<std::vector<T>> _result;
	void recurse(std::vector<std::vector<T>> &accum, std::vector<T> stack, std::vector<std::vector<T>> sequences, int index = 0) {
		std::vector<T> sequence = sequences[index];
		for (T& i : sequence) {
			stack.push_back(i);
			if (index == sequences.size() - 1)
				accum.push_back(stack);
			else
				recurse(accum, stack, sequences, index + 1);
			stack.pop_back();
		}
	}
};

class TestData {
public:
	enum Components {
		scheme = 0,
		userinfo,
		host,
		port,
		path,
		query,
		fragment,
		count
	};
	
	struct TestElement {
		std::string rawURI = "";
		std::map<Components, std::string> expected;
	};
	
	TestData() : data(Components::count) {
		//Valid schemes contain characters, digits, ., + or -
		data[Components::scheme] = {
			"mailto:",
			"my.ftp+ssh-1:"
		};
	
		//Valid user infos can contain "-" / "." / "_" / "~"
		data[Components::userinfo] = {
			"sa.lza-ve_r~de@"
		};
	
		//Valid hosts can contain "-" / "."
		data[Components::host] = {
			"comp.infosystems.www.servers.unix",
			"salza-verde.org",
			"127.0.0.1",
			"localhost"
		};
	
		//Valid ports contain digits
		data[Components::port] = {
			":1176"
		};
	
		//Valid paths can contain "-" / "." / "_" / "~" / "@" / "/"
		data[Components::path] = {
			"/oasis:names:specification:docbook:dtd:xml:4.1.2",
			"/comp.infosystems.www.servers.unix",
			"/fred@example.com",
			"/some/common/path",
			"/+1-816-555-1212"
		};
	
		//Valid queries can contain reserved characters "?", "/"
		data[Components::query] = {
			"?key=value",
			"?key1=value1&key2=value2",
			"?data?",
			"?otherdata/"
		};
	
		//Valid fragments can contain reserved characters "?", "/"
		data[Components::fragment] = {
			"#frag=value",
			"#frag1=value1&frag2=value2",
			"#frag_data?",
			"#frag_otherdata/"
		};
	}
	
	std::vector<TestElement> generateTestElements (std::vector<Components> components) {
		std::vector<std::vector<std::string>> inputData;
		bool isPrefixed = false;
		for(auto component : components) {
			if((component == Components::host || component == Components::userinfo) && (!isPrefixed)) {
				auto prefixed = data[component];
				for(auto &element : prefixed) {
					element = "//" + element;
				}
				inputData.push_back(prefixed);
				isPrefixed = true;
			}
			else
				inputData.push_back(data[component]);
		}
		
		std::vector<TestElement> output;
		auto combinations = CartesianProduct<std::string>(inputData);
		for(auto &combination : combinations.get()) {
			TestElement element;
			auto it = combination.begin();
			for(auto component : components) {
				element.rawURI += *it;
				element.expected[component] = *it++;
			}
			output.push_back(element);
		}
		return output;
	}
	
private:
	std::vector<std::vector<std::string>> data;
};

class URITest2 : public testing::TestWithParam<TestData::TestElement> {
public:
	void SetUp() override {
		element = GetParam();
		uri = URI::parse(element.rawURI);
	}

	protected:
	TestData::TestElement element;
	std::unique_ptr<URI> uri;
};

TEST_P(URITest2, Sample) {
	if(element.expected.find(TestData::Components::scheme) != element.expected.end())
		EXPECT_EQ(element.expected[TestData::Components::scheme], uri->getScheme() + ":") << "Raw URI was:" + element.rawURI;
	
	bool containsUserInfo = element.expected.find(TestData::Components::userinfo) != element.expected.end();
	if(containsUserInfo)
		EXPECT_EQ(element.expected[TestData::Components::userinfo], "//" + uri->getUserInfo() + "@") << "Raw URI was:" + element.rawURI;
	
	if(element.expected.find(TestData::Components::host) != element.expected.end())
		EXPECT_EQ(element.expected[TestData::Components::host], (containsUserInfo?"":"//") + uri->getHost()) << "Raw URI was:" + element.rawURI;
	
	if(element.expected.find(TestData::Components::port) != element.expected.end())
		EXPECT_EQ(element.expected[TestData::Components::port], ":" + uri->getPort()) << "Raw URI was:" + element.rawURI;
	
	if(element.expected.find(TestData::Components::path) != element.expected.end())
		EXPECT_EQ(element.expected[TestData::Components::path], uri->getPath()) << "Raw URI was:" + element.rawURI;
	
	if(element.expected.find(TestData::Components::query) != element.expected.end())
		EXPECT_EQ(element.expected[TestData::Components::query], "?" + uri->getQuery()) << "Raw URI was:" + element.rawURI;
	
	if(element.expected.find(TestData::Components::fragment) != element.expected.end())
		EXPECT_EQ(element.expected[TestData::Components::fragment], "#" + uri->getFragment()) << "Raw URI was:" + element.rawURI;
	
}

static std::vector<TestData::TestElement> createTestValues() {
	TestData testData;

	std::vector<std::vector<TestData::TestElement>> testElements;
	testElements.push_back(testData.generateTestElements({TestData::Components::scheme}));
	testElements.push_back(testData.generateTestElements({TestData::Components::scheme, TestData::Components::userinfo, TestData::Components::host}));
	testElements.push_back(testData.generateTestElements({TestData::Components::scheme, TestData::Components::userinfo, TestData::Components::host, TestData::Components::port}));
	testElements.push_back(testData.generateTestElements({TestData::Components::scheme, TestData::Components::userinfo, TestData::Components::host, TestData::Components::port, TestData::Components::path, TestData::Components::query, TestData::Components::fragment}));
	testElements.push_back(testData.generateTestElements({TestData::Components::scheme, TestData::Components::path, TestData::Components::query, TestData::Components::fragment}));
	
	std::vector<TestData::TestElement> output;
	for(auto &testElementVector : testElements) {
		output.insert(output.end(), testElementVector.begin(), testElementVector.end());
	}
	return output;
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


