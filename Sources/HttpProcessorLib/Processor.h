#ifndef HTTP_PROCESSOR_H
#define HTTP_PROCESSOR_H

#include <iosfwd>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "HttpRequest.h"

namespace HttpProcessor
{
    class Processor
    {
    public:
        void run(std::istream& stream);
        int pendingRequests() const;
        std::vector<HttpRequest> takeCompletedRequests();

    private:
        std::unordered_map<std::string, std::string> mPendingRequests;

        std::mutex mRequestListMutex;
        std::vector<HttpRequest> mRequestList;
    };
} // namespace HttpProcessor
#endif // HTTP_PROCESSOR_H