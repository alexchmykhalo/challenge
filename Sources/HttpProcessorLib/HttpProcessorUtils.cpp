#include "HttpProcessorUtils.h"

namespace HttpProcessor
{
    bool isHttpRequest(const std::string& startLine)
    {
        return !startLine.starts_with("HTTP");
    }

    std::optional<std::string> httpPath(const std::string& startLine)
    {
        const auto pathStart = startLine.find(' ');
        const auto pathEnd = startLine.find(' ', pathStart + 1);
        if (pathStart == std::string::npos || pathEnd == std::string::npos)
        {
            return {};
        }
        return startLine.substr(pathStart + 1, pathEnd - pathStart - 1);
    }

    std::optional<std::string> traceId(const std::string& header)
    {
        if (!header.starts_with("X-Trace-ID"))
        {
            return {};
        }

        const auto idStart = header.find(": ");
        if (idStart == std::string::npos || (idStart + 2 >= header.size()))
        {
            return {};
        }

        return header.substr(idStart + 2);
    }

    std::optional<std::string> httpStatusCode(const std::string& statusLine)
    {
        const auto statusCodeStart = statusLine.find(' ');
        const auto statusCodeEnd = statusLine.find(' ', statusCodeStart + 1);
        if (statusCodeStart == std::string::npos || statusCodeEnd == std::string::npos)
        {
            return {};
        }
        return statusLine.substr(statusCodeStart + 1, statusCodeEnd - statusCodeStart - 1);
    }
} // namespace HttpProcessor
