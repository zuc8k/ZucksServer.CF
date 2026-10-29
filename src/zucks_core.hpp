#pragma once
#include "zucks_config.hpp"
#include <memory>
#include <string>

namespace Zucks {

class Proxy;

class Core {
public:
    Core();
    ~Core();

    bool loadConfig(const std::string& path);
    void run();

private:
    Config cfg_;
    std::shared_ptr<Proxy> proxy_;
};

} // namespace Zucks