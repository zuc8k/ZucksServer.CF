// Zucks Server - Core Module
// (c) 2026 Zucks Project. All rights reserved.

#pragma once
#include <memory>
#include <string>

namespace Zucks {

class Core {
public:
    Core();
    ~Core();
    bool loadConfig(const std::string& path);
    void run();

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace Zucks