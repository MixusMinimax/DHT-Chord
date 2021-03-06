#include "config.h"
#include <inipp.h>
#include <fstream>
#include <regex>

// Define static variables
uint8_t config::Configuration::PoW_Difficulty{DEFAULT_DIFFICULTY};
uint8_t config::Configuration::defaultReplicationLimit{DEFAULT_REPLICATION_LIMIT};

bool splitIP(const std::string &value, std::string &address, uint16_t &port)
{
    if (value.empty()) return false;
    std::regex ipv4{R"((\d{1,3}(?:\.\d{1,3}){3}):(\d+))"};
    std::regex ipv6{R"(\[([0-9a-fA-F]*(?::[0-9a-fA-F]*)*)\]:(\d+))"};
    std::smatch match{};

    if (std::regex_match(value, match, ipv4) || std::regex_match(value, match, ipv6)) {
        try {
            uint16_t p = static_cast<uint16_t>(std::stoul(match[2]));
            address = match[1];
            port = p;
        } catch (...) { return false; }
        return true;
    }

    return false;
}

config::Configuration config::parseConfigFile(const std::string &path)
{
    Configuration config{};
    bool boolean;
    std::string str{};
    uint64_t uint64;

    inipp::Ini<char> ini{};
    std::ifstream is(path);
    ini.parse(is);
    ini.strip_trailing_comments();
    ini.default_section(ini.sections["DEFAULT"]);
    ini.interpolate();

    if (inipp::get_value(ini.sections["DEFAULT"], "hostkey", str))
        config.private_key_path = str;
    if (inipp::get_value(ini.sections["dht"], "certificate", str))
        config.certificate_path = str;
    if (inipp::get_value(ini.sections["dht"], "use_tls", boolean))
        config.use_tls = boolean;
    if (inipp::get_value(ini.sections["dht"], "p2p_address", str))
        splitIP(str, config.p2p_address, config.p2p_port);
    if (inipp::get_value(ini.sections["dht"], "api_address", str))
        splitIP(str, config.api_address, config.api_port);
    if (inipp::get_value(ini.sections["dht"], "bootstrapNode_address", str))
        splitIP(str, config.bootstrapNode_address, config.bootstrapNode_port);
    if (inipp::get_value(ini.sections["dht"], "node_amount", uint64))
        config.node_amount = uint64;
    if (inipp::get_value(ini.sections["dht"], "startup_script", str))
        config.startup_script = str;
    return config;
}

