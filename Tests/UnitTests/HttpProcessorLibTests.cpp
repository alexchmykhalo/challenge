#include <catch2/catch_test_macros.hpp>

#include "HttpProcessorUtils.h"

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
