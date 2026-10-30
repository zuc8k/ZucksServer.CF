// Zucks Server - Config Implementation
#include "zucks_config.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace Zucks {

bool loadConfig(const std::string& path, Config& out, std::string& err) {
    try {
        std::ifstream f(path);
        if (!f) { err = "Cannot open config: " + path; return false; }

        nlohmann::json root;
        f >> root;
        auto j = root.contains("zucks") ? root["zucks"] : root;

        out.log_level = j.value("log_level", "info");
        out.listen_port = j.value("listen_port", 28888);

        if (j.contains("versions")) {
            for (auto& [ver, val] : j["versions"].items()) {
                TargetConfig t;
                t.host = val.value("target_host", "127.0.0.1");
                t.port = val.value("target_port", 13008);
                out.versions[ver] = t;
            }
        }

        if (j.contains("security")) {
            const auto& s = j["security"];
            out.security.max_connections_per_ip = s.value("max_connections_per_ip", 30);
            out.security.blacklist_enabled = s.value("blacklist_enabled", true);
        }

        if (out.versions.empty()) {
            err = "No versions configured in JSON";
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        err = std::string("Config parse error: ") + e.what();
        return false;
    }
}

} // namespace Zucks