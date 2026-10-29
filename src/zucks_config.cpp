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

        out.version = j.value("version", out.version);
        out.log_level = j.value("log_level", out.log_level);
        out.listen_port = j.value("listen_port", out.listen_port);
        out.target_host = j.value("target_host", out.target_host);
        out.target_port = j.value("target_port", out.target_port);

        if (j.contains("security")) {
            const auto& s = j["security"];
            out.security.max_pps = s.value("max_pps", out.security.max_pps);
            out.security.max_bps = s.value("max_bps", out.security.max_bps);
            out.security.max_connections_per_ip =
                s.value("max_connections_per_ip", out.security.max_connections_per_ip);
            out.security.blacklist_enabled =
                s.value("blacklist_enabled", out.security.blacklist_enabled);
        }

        if (out.listen_port == 0 || out.target_port == 0) {
            err = "Ports must be non-zero";
            return false;
        }
        if (out.target_host.empty()) {
            err = "target_host is empty";
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        err = std::string("Config parse error: ") + e.what();
        return false;
    }
}

} // namespace Zucks