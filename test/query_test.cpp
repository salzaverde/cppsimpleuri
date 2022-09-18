// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <gtest/gtest.h>
#include <salzaverde/query.h>

#include <vector>
#include <map>

using namespace salzaverde;

std::string toRaw(const std::map<std::string, std::string> &componentsToUse, const std::string &delimiter) {
    if(componentsToUse.empty()) 
        return "";

    std::string output;
    auto it = componentsToUse.begin();

    bool hasValue = ! it->second.empty();

    output += it->first;
    if(! it->second.empty()) output += "=" + (it->second);
    while(++it != componentsToUse.end()) {
        output += delimiter + it->first;
        if(! it->second.empty()) output += "=" + (it->second);
    }
    return output;
}

class TestData {
public:
    std::string raw;
    std::string delimiter;
    std::map<std::string, std::string> components;

    static std::vector<TestData> generate() {
        std::vector<TestData> output;
        std::vector<std::string> validDelimiters = {"!", "$", "&", "'", "*", "+", ",", ";"};
        std::vector<std::map<std::string, std::string>> testComponentsVector = {
            {},
            {{"key1", ""}, {"key2", ""}},
            {{"key1", "val1"}},
            {{"key1", "val1"}, {"key2", "val2"}},
            {{"key1", "val1=val2"}, {"key2", "val2=val3"}}
        };

        for(auto &testComponents : testComponentsVector) {
            for(auto &delimiter : validDelimiters) {
                auto raw = toRaw(testComponents, delimiter);
                output.push_back({raw, delimiter, testComponents});
            }
        }

        return output;
    }
};

class QueryParsing : public testing::TestWithParam<TestData> {};

TEST_P(QueryParsing, Parametrized) {
    auto testData = GetParam();

    auto query = Query::parse(testData.raw, testData.delimiter);
    EXPECT_EQ(query.dump(testData.delimiter), testData.raw);

    for(auto &component : testData.components) {
        EXPECT_TRUE(query.parameters.find(component.first) != query.parameters.end()) << "Tried to parse:" + testData.raw + ", Key: " + component.first;
        EXPECT_EQ(query.parameters[component.first], component.second);
    }
}

INSTANTIATE_TEST_SUITE_P(
	QueryTests,
	QueryParsing,
	::testing::ValuesIn(TestData::generate())
);

TEST(QueryTest, Build) {
    std::map<std::string, std::string> params;
    params.emplace("key1", "val1");
    params.emplace("key2", "val2");
    auto query = Query(params);
    
    EXPECT_EQ(query.dump(), "key1=val1&key2=val2");
}
