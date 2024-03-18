#ifndef HTTP_PROCESSOR_H
#define HTTP_PROCESSOR_H

#include <iosfwd>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <utility>

#include "HttpRequest.h"

namespace HttpProcessor
{
    class Processor
    {
    public:
        using PendingRequest = std::pair<std::string, uint32_t>;

        void run(std::istream& stream);
        int pendingRequests() const;
        std::vector<HttpRequest> takeCompletedRequests();
        void cleanupPendingRequests(int timeout);

    private:
        std::mutex mPendingRequestListMutex;
        std::unordered_map<std::string, PendingRequest> mPendingRequestList;

        std::mutex mRequestListMutex;
        std::vector<HttpRequest> mRequestList;
    };
} // namespace HttpProcessor
#endif // HTTP_PROCESSOR_H