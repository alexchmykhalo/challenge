#include "Processor.h"

#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <fstream>
#include <thread>

using namespace std::chrono_literals;
constexpr auto cWriteTimeout{60s};

void writeFileWorker(HttpProcessor::Processor* processor,
                     const std::filesystem::path& path,
                     const std::chrono::seconds& timeout)
{
    std::ofstream fileStream(path);
    if (!fileStream.is_open())
    {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    while (true)
    {
        std::this_thread::sleep_for(timeout);

        const auto result = processor->takeCompletedRequests();
        std::cout << "Writing " << result.size() << " requests, " << processor->pendingRequests() << " pending"
                  << std::endl;

        for (const auto& request : result)
        {
            fileStream << request.path << ',' << request.code << '\n';
        }
        fileStream.flush();

        // Clean up requests without responce that are waiting
        processor->cleanupPendingRequests(timeout.count());
    }
}

int main(int argc, const char** argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    if (arguments.size() < 2 || arguments.at(0) != "-o")
    {
        std::cout << "Utility to write statistics about Http requests/responses read from the Unix pipe." << std::endl;
        std::cout << "usage: processor [options]" << std::endl;
        std::cout << "options:" << std::endl;
        std::cout << "\t-o FILE\tspecify file to write results" << std::endl;
        return -1;
    }

    const auto path = arguments.at(1);

    HttpProcessor::Processor processor;
    std::thread thread(writeFileWorker, &processor, path, cWriteTimeout);
    processor.run(std::cin);
    thread.join();
}