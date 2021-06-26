#ifndef DHT_DHT_H
#define DHT_DHT_H

#include <api.h>
#include <future>
#include <utility>
#include <vector>
#include <cstddef>
#include <memory>
#include <message_data.h>
#include <shared_mutex>
#include <peer.capnp.h>
#include <exception>
#include <stdexcept>
#include "Peer.h"
#include "NodeInformation.h"

namespace dht
{
    class Dht
    {
    public:
        explicit Dht(std::shared_ptr<NodeInformation> nodeInformation) :
            m_nodeInformation(std::move(nodeInformation)),
            m_mainLoop(std::async(std::launch::async, [this]() { runServer(); }))
        {}
        ~Dht()
        {
            m_dhtRunning = false;
            m_api = nullptr;
            m_mainLoop.wait(); // This happens after the destructor anyway, but this way it is clearer
        };
        Dht(const Dht &) = delete;
        Dht(Dht &&) = delete;

        /**
         * The api is used to receive requests.
         * @param api - unique, transfers ownership
         */
        void setApi(std::unique_ptr<api::Api> api);

    private:
        void runServer();

        /**
         * This is where the actual work happens.
         * mainLoop is called asynchronously from the constructor of Dht.
         * It needs to worry about stopping itself.
         */
        void mainLoop();

        std::shared_ptr<NodeInformation> m_nodeInformation;

        [[nodiscard]] std::optional<NodeInformation::Node> getSuccessor(NodeInformation::id_type key);
        std::vector<uint8_t> onDhtPut(const api::Message_KEY_VALUE &m, std::atomic_bool &cancelled);
        std::vector<uint8_t> onDhtGet(const api::Message_KEY &m, std::atomic_bool &cancelled);

        std::future<void> m_mainLoop;
        std::unique_ptr<api::Api> m_api;
        std::atomic_bool m_dhtRunning{true};
        std::optional<std::reference_wrapper<PeerImpl>> m_peerImpl;
        std::optional<std::reference_wrapper<const kj::Executor>> m_executor;

        // Getters

        /**
         * @throw std::bad_optional_access
         * @return PeerImpl instance
         */
        auto getPeerImpl()
        {
            return m_peerImpl.value().get();
        }
    };
}

#endif //DHT_DHT_H