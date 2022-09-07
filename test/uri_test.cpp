#include <gtest/gtest.h>
#include <uri.h>

TEST(URITestSuite, Prototyping) {
	auto scheme = URI::Scheme("http");
	auto host = URI::Host("example.com");
	auto port = URI::Port("1234");
	
	auto uri = scheme|host|port;
	EXPECT_EQ(uri->toString(), "http://example.com:1234");
};


//struct Include {
//    static const int Scheme = 1 << 0;
//    static const int Host = 1 << 1;
//    static const int Port = 1 << 2;
//    static const int Path = 1 << 3;
//    static const int QueryString = 1 << 4;
//};
//
//class URITestSuite : public testing::Test, public testing::WithParamInterface<int> {
//public:
//    std::string buildValidURI(int includeFlags)  {
//        std::string rawURI = "";
//		if(includeFlags & Include::Scheme) {
//            rawURI.append(scheme);
//            rawURI.append("://");
//        }
//
//        if(includeFlags & Include::Host) {
//            rawURI.append(host);
//        }
//
//        if(includeFlags & Include::Port) {
//            rawURI.append(":");
//            rawURI.append(port);
//        }
//
//        if(includeFlags & Include::Path) {
//            rawURI.append(path);
//        }
//
//        if(includeFlags & Include::QueryString) {
//            rawURI.append("?");
//            rawURI.append(queryString);
//        }
//
//        return rawURI;
//	}
//
//    void test(const URI &uri, int includeFlags) {
//        if(includeFlags & Include::Scheme)
//            EXPECT_EQ(uri.getScheme(), scheme);
//        else
//            EXPECT_EQ(uri.getScheme(), "");
//
//        if(includeFlags & Include::Host)
//            EXPECT_EQ(uri.getHost(), host);
//        else
//            EXPECT_EQ(uri.getHost(), "");
//
//        if(includeFlags & Include::Port)
//            EXPECT_EQ(uri.getPort(), port);
//        else
//            EXPECT_EQ(uri.getPort(), "");
//
//        if(includeFlags & Include::Path)
//            EXPECT_EQ(uri.getPath(), path);
//        else
//            EXPECT_EQ(uri.getPath(), "");
//
//        if(includeFlags & Include::QueryString)
//            EXPECT_EQ(uri.getQueryString(), queryString);
//        else
//            EXPECT_EQ(uri.getQueryString(), "");
//    }
//
//private:
//    std::string scheme = "http";
//    std::string host = "test.de";
//    std::string port = "1176";
//    std::string path = "/some/path";
//    std::string queryString = "why=not&test=this";
//
//    std::unique_ptr<URI> uri;
//};
//
//TEST_P(URITestSuite, ValidConfigurations) {
//    auto includeFlags = GetParam();
//    auto rawURI = buildValidURI(includeFlags);
//    auto uri = URI::parse(rawURI);
//    test(uri.get(), includeFlags);
//};
//
//INSTANTIATE_TEST_SUITE_P(
//    URITests,
//    URITestSuite,
//    testing::Values(
//        Include::Scheme | Include::Host | Include::Port | Include::Path | Include::QueryString,
//        Include::Scheme | Include::Host | Include::Port | Include::Path,
//        Include::Scheme | Include::Host | Include::Port,
//        Include::Scheme | Include::Host,
//        Include::Scheme,
//        Include::Host | Include::Port | Include::Path | Include::QueryString,
//        Include::Host | Include::Port | Include::Path,
//        Include::Host | Include::Port,
//        Include::Host,
//        Include::Host | Include::Path | Include::QueryString,
//        Include::Host | Include::Path,
//        Include::Host | Include::QueryString
//    )
//);
