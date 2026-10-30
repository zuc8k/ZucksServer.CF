#include "zucks_config.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "zucks_logger.hpp"

namespace Zucks {

std::string readGameVersionFromIni(const std::string& game_path) {
    if (game_path.empty()) return "";
    std::filesystem::path p(game_path);
    std::filesystem::path iniFile = p / "version.ini";

    if (std::filesystem::exists(iniFile)) {
        std::ifstream f(iniFile);
        std::string line;
        while (std::getline(f, line)) {
            if (line.find("Version") != std::string::npos || line.find("version") != std::string::npos) {
                auto pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string ver = line.substr(pos + 1);
                    // تنظيف المسافات
                    ver.erase(0, ver.find_first_not_of(" \t\r\n"));
                    ver.erase(ver.find_last_not_of(" \t\r\n") + 1);
                    return ver;
                }
            }
        }
    }
    return "";
}

bool loadConfig(const std::string& path, Config& out, std::string& err) {
    try {
        std::ifstream f(path);
        if (!f) { err = "Cannot open config: " + path; return false; }

        nlohmann::json root;
        f >> root;
        auto j = root.contains("zucks") ? root["zucks"] : root;

        out.log_level = j.value("log_level", "info");
        out.listen_port = j.value("listen_port", 28888);
        out.game_path = j.value("game_path", "");

        // 1. تحميل السيرفر الافتراضي
        if (j.contains("default_target")) {
            out.default_target.host = j["default_target"].value("target_host", "127.0.0.1");
            out.default_target.port = j["default_target"].value("target_port", 13008);
        }

        // 2. تحميل السيرفرات لكل إصدار
        if (j.contains("versions")) {
            for (auto& [ver, val] : j["versions"].items()) {
                TargetConfig t;
                t.host = val.value("target_host", "127.0.0.1");
                t.port = val.value("target_port", 13008);
                out.versions[ver] = t;
            }
        }

        // 3. تحميل خريطة الهيدرات الديناميكية من ملف خارجي
        std::ifstream hf("config/headers.json");
        if (hf) {
            nlohmann::json hroot;
            hf >> hroot;
            for (auto& [hex, ver] : hroot["headers"].items()) {
                out.headers_map[hex] = ver;
            }
        } else {
            Logger::warn("config/headers.json not found. Using default magic bytes.");
        }

        return true;
    } catch (const std::exception& e) {
        err = std::string("Config parse error: ") + e.what();
        return false;
    }
}

} // namespace Zucks