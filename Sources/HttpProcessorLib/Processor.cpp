#include "Processor.h"

#include <istream>
#include <chrono>

#include "HttpProcessorUtils.h"

namespace
{
    enum class ProcessorState
    {
        ReadyForParse,
        GotHttpRequest,
        GotHttpResponce
    };

    uint32_t generateTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        long int timestamp_seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
        return timestamp_seconds;
    }
} // namespace

namespace HttpProcessor
{
    void Processor::run(std::istream& stream)
    {
        std::string line;
        std::string requestPath;
        std::string responceCode;
        auto state = ProcessorState::ReadyForParse;
        while (std::getline(stream, line, '\n'))
        {
            if (line.empty())
            {
                // Got empty line -> start parsing new HTTP request/response
                state = ProcessorState::ReadyForParse;
                continue;
            }

            switch (state)
            {
            case ProcessorState::ReadyForParse:
            {
                if (HttpProcessor::isHttpRequest(line))
                {
                    if (const auto path = HttpProcessor::httpPath(line))
                    {
                        requestPath = *path;
                        state = ProcessorState::GotHttpRequest;
                    }
                }
                else
                {
                    if (const auto code = HttpProcessor::httpStatusCode(line))
                    {
                        responceCode = *code;
                        state = ProcessorState::GotHttpResponce;
                    }
                }
                break;
            }
            case ProcessorState::GotHttpRequest:
            {
                if (const auto id = HttpProcessor::traceId(line))
                {
                    const auto pendingRequest = std::make_pair(requestPath, generateTimestamp());

                    std::scoped_lock pendingRequestListMutex(mPendingRequestListMutex);
                    mPendingRequestList.emplace(std::make_pair(*id, pendingRequest));

                    state = ProcessorState::ReadyForParse;
                }

                break;
            }
            case ProcessorState::GotHttpResponce:
            {
                if (const auto id = HttpProcessor::traceId(line))
                {
                    std::scoped_lock pendingRequestListMutex(mPendingRequestListMutex);
                    const auto it = mPendingRequestList.find(*id);

                    if (it != mPendingRequestList.end())
                    {
                        std::scoped_lock requestListMutex(mRequestListMutex);
                        mRequestList.push_back(HttpRequest{*id, responceCode, (*it).second.first});

                        mPendingRequestList.erase(it);
                    }

                    state = ProcessorState::ReadyForParse;
                }
                break;
            }
            default:
                break;
            }
        }
    }

    int Processor::pendingRequests() const
    {
        return mPendingRequestList.size();
    }

    void Processor::cleanupPendingRequests(int timeout)
    {
        const auto timeoutTimestamp = generateTimestamp() - timeout;

        std::scoped_lock lock(mPendingRequestListMutex);
        auto it = mPendingRequestList.begin();
        while (it != mPendingRequestList.end())
        {
            const auto timestamp = (*it).second.second;
            if (timestamp <= timeoutTimestamp)
            {
                // According to https://en.cppreference.com/w/cpp/container/unordered_map/erase
                // only iterators to the erased elements are invalidated
                it = mPendingRequestList.erase(it);
                continue;
            }

            it++;
        }
    }

    std::vector<HttpRequest> Processor::takeCompletedRequests()
    {
        std::scoped_lock lock(mRequestListMutex);
        return std::move(mRequestList);
    }
} // namespace HttpProcessor