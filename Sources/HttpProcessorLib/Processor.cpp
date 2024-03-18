#include "Processor.h"

#include <istream>

#include "HttpProcessorUtils.h"

namespace
{
    enum class ProcessorState
    {
        ReadyForParse,
        GotHttpRequest,
        GotHttpResponce
    };
}

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
                    mPendingRequests.emplace(std::make_pair(*id, requestPath));
                    state = ProcessorState::ReadyForParse;
                }

                break;
            }
            case ProcessorState::GotHttpResponce:
            {
                if (const auto id = HttpProcessor::traceId(line))
                {
                    const auto it = mPendingRequests.find(*id);
                    mRequests.push_back(HttpRequest{*id, responceCode, (*it).second});
                    mPendingRequests.erase(it);
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
        return mPendingRequests.size();
    }

    std::vector<HttpRequest> Processor::takeCompletedRequests()
    {
        return std::move(mRequests);
    }
} // namespace HttpProcessor