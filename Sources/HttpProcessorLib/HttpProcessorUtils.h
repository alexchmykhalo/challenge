#ifndef HTTP_PROCESSOR_UTILS_H
#define HTTP_PROCESSOR_UTILS_H

#include <string>
#include <optional>

namespace HttpProcessor
{
    bool isHttpRequest(const std::string& startLine);
    std::optional<std::string> httpPath(const std::string& startLine);
    std::optional<std::string> traceId(const std::string& header);
    std::optional<std::string> httpStatusCode(const std::string& statusLine);
} // namespace HttpProcessor

#endif // HTTP_PROCESSOR_UTILS_H