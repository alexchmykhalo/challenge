#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <string>

namespace HttpProcessor
{
    struct HttpRequest
    {
        std::string requestId;
        std::string code;
        std::string path;
    };
} // namespace HttpProcessor

#endif // HTTP_REQUEST_H