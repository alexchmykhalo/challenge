#include <catch2/catch_test_macros.hpp>

#include <sstream>

#include "HttpProcessorUtils.h"
#include "Processor.h"

TEST_CASE("isHttpRequest", "[http]")
{
    REQUIRE(HttpProcessor::isHttpRequest("PATCH /profile/smadigan HTTP/1.1"));
    REQUIRE(!HttpProcessor::isHttpRequest("HTTP/1.1 202 ACCEPTED"));
}

TEST_CASE("httpPath", "[http]")
{
    REQUIRE(!HttpProcessor::httpPath("GET"));
    REQUIRE(!HttpProcessor::httpPath("GET /profile/ktraia"));
    REQUIRE(*HttpProcessor::httpPath("GET /profile/ktraia HTTP/1.1") == "/profile/ktraia");
}

TEST_CASE("traceId", "[http]")
{
    REQUIRE(!HttpProcessor::traceId("Host: dev-challenges.kerno.io"));
    REQUIRE(!HttpProcessor::traceId("X-Trace-ID: "));
    REQUIRE(*HttpProcessor::traceId("X-Trace-ID: BwQEAwIJ") == "BwQEAwIJ");
}

TEST_CASE("httpStatusCode", "[http]")
{
    REQUIRE(!HttpProcessor::httpPath("HTTP/1.1"));
    REQUIRE(!HttpProcessor::httpPath("HTTP/1.1 undefined"));
    REQUIRE(*HttpProcessor::httpPath("HTTP/1.1 403 FORBIDDEN") == "403");
}

TEST_CASE("Parse HTTP request")
{
    const auto data = "PATCH /profile/smadigan HTTP/1.1\n"
                      "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n"
                      "Host: dev-challenges.kerno.io\n"
                      "Accept-Language: en-us\n"
                      "X-Trace-ID: BwQEAwIJ\n\n";
    std::stringstream stream(data);

    HttpProcessor::Processor processor;
    processor.run(stream);

    REQUIRE(processor.pendingRequests() == 1);
}

TEST_CASE("Parse HTTP request and responce")
{
    const auto data = "PATCH /profile/smadigan HTTP/1.1\n"
                      "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n"
                      "Host: dev-challenges.kerno.io\n"
                      "Accept-Language: en-us\n"
                      "X-Trace-ID: BwQEAwIJ\n\n"

                      "HTTP/1.1 202 ACCEPTED\n"
                      "Date: Fri Mar 15 2024 18:39:30 GMT+0000\n"
                      "Server: nginx\n"
                      "X-Trace-ID: BwQEAwIJ\n"
                      "Connection: Closed\n\n";
    std::stringstream stream(data);

    HttpProcessor::Processor processor;
    processor.run(stream);
    const auto requestList = processor.takeCompletedRequests();

    REQUIRE(processor.pendingRequests() == 0);
    REQUIRE(requestList.size() == 1);
    REQUIRE(requestList.at(0).requestId == "BwQEAwIJ");
    REQUIRE(requestList.at(0).path == "/profile/smadigan");
    REQUIRE(requestList.at(0).code == "202");
}