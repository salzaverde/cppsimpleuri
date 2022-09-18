// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <gtest/gtest.h>
#include <salzaverde/query.h>

using namespace salzaverde;

TEST(QueryTest, Parse) {
    auto query = Query::parse("key1=val1");
    EXPECT_EQ(query.dump(), "key1=val1");
}

TEST(QueryTest, Build) {
    std::map<std::string, std::string> params;
    params.emplace("key1", "val1");
    params.emplace("key2", "val2");
    auto query = Query(params);
    
    EXPECT_EQ(query.dump(), "key1=val1&key2=val2");
}

TEST(QueryTest, Empty) {
    auto query = Query::parse("");
    EXPECT_EQ(query.dump(), "");
}
