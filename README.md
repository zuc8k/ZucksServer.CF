🛡️ ZucksServer.CF

<div align="center">
<img src="https://readme-typing-svg.demolab.com?font=Orbitron&weight=800&size=35&duration=3000&pause=1000&color=00BFFF&center=true&vCenter=true&width=650&lines=ZUCKSSERVER.CF;ADVANCED+SERVER+PROTECTION;BUILT+FOR+CROSSFIRE" alt="ZucksServer.CF Animated Header" />

A High-Performance Reverse Proxy & Server Protection System

Engineered with C++20. Designed for CrossFire.

<br>
<img src="https://img.shields.io/badge/PROJECT-ZucksServer.CF-00BFFF?style=for-the-badge&logo=shield&logoColor=white" />
<img src="https://img.shields.io/badge/VERSION-1.1.0-151515?style=for-the-badge" />
<img src="https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" />
<img src="https://img.shields.io/badge/PLATFORM-CROSS--PLATFORM-222222?style=for-the-badge" />
<img src="https://img.shields.io/badge/LICENSE-MIT-00C853?style=for-the-badge" />
<br>
<br>

Created & Developed by Zucks

Overview • Features • Architecture • Installation • Configuration • Docker

</div>

⸻

📌 Overview

ZucksServer.CF is a high-performance, cross-platform Reverse Proxy and Server Monitoring solution developed specifically for CrossFire v1911 and compatible server environments.

Built with modern C++20 and asynchronous networking through Asio, the project provides an additional security and traffic-management layer between clients and the target server.

Its architecture focuses on performance, scalability, efficient connection handling, and configurable protection mechanisms.

Core Technologies

Technology	Purpose
C++20	Core application development
Asio	Asynchronous networking
spdlog	High-performance logging
nlohmann/json	JSON configuration management
GoogleTest	Unit testing
CMake	Cross-platform build system
Docker	Containerized deployment

⸻

✨ Key Features

🔐 Security & Protection

* Deep Packet Inspection (DPI) — Inspect incoming packets before forwarding them.
* IP Rate Limiting — Token Bucket-based traffic control per IP address.
* Dynamic IP Blacklist — Block suspicious or unwanted IP addresses.
* PPS Protection — Configurable packets-per-second limits.
* BPS Protection — Configurable bandwidth limits per IP.
* Connection Limits — Restrict simultaneous connections per IP.
* Suspicious Connection Handling — Automatically terminate connections that exceed configured security limits.

⚡ Performance & Networking

* Asynchronous, non-blocking I/O powered by Asio.
* Efficient connection and traffic forwarding.
* Modern C++20 architecture.
* Smart memory management using shared_ptr and unique_ptr.
* High-performance logging through spdlog.
* Lightweight and scalable architecture.

🛠️ Development & Deployment

* Modular and extensible codebase.
* Unit testing with GoogleTest.
* Automated CI workflows using GitHub Actions.
* Docker and Docker Compose support.
* JSON-based configuration.
* Cross-platform build support.

⸻

🏗️ Project Structure

ZucksServer.CF/
│
├── .github/
│   └── workflows/
│       └── ci.yml
│
├── config/
│   └── zucks_config.json
│
├── docker/
│   ├── Dockerfile
│   └── docker-compose.yml
│
├── scripts/
│   ├── build.sh
│   ├── run.sh
│   └── clean.sh
│
├── src/
│   ├── main.cpp
│   ├── zucks_core.hpp
│   ├── zucks_core.cpp
│   ├── zucks_proxy.hpp
│   ├── zucks_proxy.cpp
│   ├── zucks_forwarder.hpp
│   ├── zucks_forwarder.cpp
│   ├── zucks_filter.hpp
│   ├── zucks_filter.cpp
│   ├── zucks_config.hpp
│   ├── zucks_config.cpp
│   └── zucks_logger.hpp
│
├── tests/
│   └── test_zucks_filter.cpp
│
├── CMakeLists.txt
├── Makefile
└── README.md

⸻

🚀 Installation

Requirements

Dependency	Minimum Version
C++ Compiler	GCC 10+ / Clang 12+ / MSVC 2019+
CMake	3.20+
Git	2.30+
Python	3.x (Optional)

Dependencies: Asio, spdlog, and nlohmann_json are automatically retrieved through CMake FetchContent.

1. Clone the Repository

git clone https://github.com/zuc8k/ZucksServer.CF.git
cd ZucksServer.CF

2. Prepare Build Scripts

chmod +x scripts/*.sh

3. Build the Project

./scripts/build.sh

Alternatively, build manually:

cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DZUCKS_BUILD_TESTS=ON
cmake --build build -j$(nproc)

⸻

▶️ Running the Server

Start the server using the provided script:

./scripts/run.sh

Or execute the binary directly:

./build/zucks-server config/zucks_config.json

Expected Console Output

[2026-10-29] [Zucks] [info] Starting Zucks Server
[2026-10-29] [Zucks] [info] Zucks config loaded
[2026-10-29] [Zucks] [info] Zucks Server running

⸻

⚙️ Configuration

ZucksServer.CF uses a JSON-based configuration system.

Configuration File: config/zucks_config.json

{
  "zucks": {
    "version": "1.1.0",
    "log_level": "info",
    "listen_port": 28888,
    "target_host": "127.0.0.1",
    "target_port": 18889,
    "security": {
      "max_pps": 1500,
      "max_bps": 5242880,
      "max_connections_per_ip": 30,
      "blacklist_enabled": true
    }
  }
}

Configuration Reference

Parameter	Description	Default
version	Application version	1.1.0
log_level	Logging verbosity	info
listen_port	Proxy listening port	28888
target_host	Target server address	127.0.0.1
target_port	Target server port	18889
max_pps	Maximum packets per second per IP	1500
max_bps	Maximum bytes per second per IP	5242880
max_connections_per_ip	Maximum concurrent connections per IP	30
blacklist_enabled	Enable IP blacklist	true

Available Log Levels

trace • debug • info • warn • error • critical

⸻

🧪 Testing

Run the unit tests using CTest:

ctest --test-dir build --output-on-failure

Or use Make:

make test

⸻

🐳 Docker Support

ZucksServer.CF includes Docker deployment support.

Build and run using Docker Compose:

docker compose -f docker/docker-compose.yml up --build -d

Stop the container:

docker compose -f docker/docker-compose.yml down

⸻

🖥️ Supported Platforms

Platform	Build Support
Linux	✓
Windows	✓
macOS	✓

⸻

🔮 Project Vision

ZucksServer.CF is designed as a foundation for advanced CrossFire server networking, traffic management, and protection.

The project aims to provide a modular architecture that can evolve with future requirements while maintaining performance, reliability, and maintainability.

⸻

👨‍💻 Developer & Ownership

<div align="center">
<img src="https://github.com/zuc8k.png" width="120" height="120" style="border-radius:50%;" alt="Zucks GitHub Avatar" />

ZUCKS

Founder • Creator • Lead Developer

Designed, engineered, and developed by Zucks.

ZucksServer.CF — Built with precision. Powered by C++20.

</div>

⸻

📜 License

This project is licensed under the MIT License.

See the LICENSE file for more information.

⸻

<div align="center">
<img src="https://readme-typing-svg.demolab.com?font=Orbitron&weight=700&size=18&duration=3000&pause=1000&color=00BFFF&center=true&vCenter=true&width=550&lines=THANK+YOU+FOR+VISITING;WELCOME+TO+ZUCKS+PROJECTS" alt="Animated Footer" />

© 2026 Zucks. All Rights Reserved.

Official Repository: ZucksServer.CF

</div>