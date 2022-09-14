// Copyright (c) 2022 Daniel Schütz. All rights reserved.
// MIT License

#include <gtest/gtest.h>
#include <salzaverde/query.h>

using namespace salzaverde;

TEST(QueryTest, Parse) {
    auto raw = std::string("?key1=val1&key2=val2");
    auto query = Query::parse(raw);

    EXPECT_EQ(query->dump(), raw);
}

TEST(QueryTest, ParseWithoutQuestionMark) {
    auto raw = std::string("key1=val1");
    auto query = Query::parse(raw);

    EXPECT_EQ(query->dump(), "?key1=val1");
}

TEST(QueryTest, Build) {
    std::map<std::string, std::string> params;
    params.emplace("key1", "val1");
    params.emplace("key2", "val2");
    auto query = Query::build(params);
    
    EXPECT_EQ(query->dump(), "?key1=val1&key2=val2");
}

TEST(QueryTest, Contains) {
    auto query = Query::parse("?key1=val1&key2=val2");

    EXPECT_TRUE(query->contains("key1"));
    EXPECT_TRUE(query->contains("key2"));
}

TEST(QueryTest, Get) {
    auto raw = std::string("?key1=val1&key2=val2");
    auto query = Query::parse(raw);
    
    EXPECT_EQ(query->get("key1"), "val1");
    EXPECT_EQ(query->get("key2"), "val2");
}

TEST(QueryTest, GetEmpty) {
    auto raw = std::string("?key1=val1");
    auto query = Query::parse(raw);
    EXPECT_FALSE(query->get("key2"));
}

TEST(QueryTest, ListKeys) {
    auto raw = std::string("?key1=val1&key2=val2");
    auto query = Query::parse(raw);
    auto keys = query->listKeys();
    
    EXPECT_TRUE(keys.contains("key1"));
    EXPECT_TRUE(keys.contains("key2"));
}

TEST(QueryTest, Set) {
    auto raw = std::string("?key1=val1&key2=val2");
    auto query = Query::parse(raw);
    query->set("key3", "val3");
    
    EXPECT_TRUE(query->contains("key3"));
    EXPECT_EQ(query->get("key3"), "val3");
}

TEST(QueryTest, SetExisting) {
    auto raw = std::string("?key1=val1&key2=val2");
    auto query = Query::parse(raw);
    query->set("key1", "val3");
    
    EXPECT_EQ(query->get("key1"), "val3");
}

TEST(QueryTest, Erase) {
    auto query = Query::parse("?key1=val1&key2=val2");
    query->erase("key1");
    
    EXPECT_FALSE(query->contains("key1"));
}

TEST(QueryTest, Empty) {
    auto query = Query::parse("");
    EXPECT_EQ(query->dump(), "");
}
