#ifndef DHT_CONFIG_H
#define DHT_CONFIG_H

#include <string>
#include <cstdint>
#include <optional>

namespace config
{
    #define DEFAULT_DIFFICULTY 1
    #define DEFAULT_REPLICATION_LIMIT 5

    struct Configuration
    {
        std::string p2p_address{"127.0.0.1"};
        uint16_t p2p_port{6002};
        std::string api_address{"127.0.0.1"};
        uint16_t api_port{7002};
        std::string bootstrapNode_address{"127.0.0.1"};
        uint16_t bootstrapNode_port{6002};
        uint64_t node_amount{1};
        std::optional<std::string> startup_script{};
        static uint8_t PoW_Difficulty;
        static uint8_t defaultReplicationLimit;
    };

    Configuration parseConfigFile(const std::string &path);
}

#endif //DHT_CONFIG_H
