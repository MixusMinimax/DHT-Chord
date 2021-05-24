#include "constants.h"
#include "message_data.h"
#define ASIO_STANDALONE

#include <iostream>
#include <chrono>
#include <csignal>
#include "api.h"
#include "request.h"

bool sigIntReceived = false;

void sigHandler(sig_t)
{
    sigIntReceived = true;
    std::cout << "Shutting down..." << std::endl;
}

int main()
{
    using namespace std::chrono_literals;
    using namespace API;

    struct sigaction sigIntHandler
    {
    };
    sigIntHandler.sa_handler = reinterpret_cast<__sighandler_t>(sigHandler);
    sigaction(SIGINT, &sigIntHandler, nullptr);

    std::cout << "This is the main method for testing api!" << std::endl;
    try {
        auto api = std::make_unique<Api>();

        // This is a placeholder request handler, it just echoes the request back, as long as it has the correct format.
        api->on<util::constants::DHT_GET>([](const Message_KEY &message_data) {
            std::cout << "[apiMain] DHT_GET" << std::endl;
            return message_data.m_bytes;
        });

        api->on<util::constants::DHT_PUT>([](const Message_KEY_VALUE &message_data) {
            std::cout << "[apiMain] DHT_PUT" << std::endl;
            return message_data.m_bytes;
        });

        while (!sigIntReceived)
            std::this_thread::sleep_for(1s);
        std::cout << "sigint received" << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error" << std::endl;
    }
    return 0;
}