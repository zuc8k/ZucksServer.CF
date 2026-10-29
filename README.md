# 🛡️ ZucksServer.CF

<div align="center">

![Version](https://img.shields.io/badge/version-1.1.0-blue.svg)
![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=c%2B%2B)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![CI](https://github.com/zuc8k/ZucksServer.CF/actions/workflows/ci.yml/badge.svg)

**Reverse Proxy & Server Watcher عالي الأداء لحماية سيرفرات CrossFire**

[الميزات](#-الميزات) • [البناء](#-البناء-والتشغيل) • [التكوين](#-التكوين) • [Docker](#-التشغيل-عبر-docker) • [المساهمة](#-المساهمة)

</div>

---

## 📖 نظرة عامة

**ZucksServer.CF** هو بروكسي عكسي (Reverse Proxy) مكتوب بلغة **C++20**، مصمم لحماية وإدارة سيرفرات **CrossFire v1911** وما شابهها. يعتمد على مكتبة `Asio` غير المتزامنة لضمان أداء عالٍ، ويوفر طبقات أمان متعددة تشمل:

- فحص عميق للحزم (DPI)
- تحديد معدل الاتصالات (Rate Limiting)
- قوائم حظر IP (Blacklist)
- تسجيل احترافي للأحداث (`spdlog`)

المشروع **متعدد المنصات**، **قابل للتوسع**، و**جاهز للنشر** عبر Docker.

---

## ✨ الميزات

### 🔒 الأمان
- ✅ **فحص عميق للحزم** قبل تمريرها للسيرفر الهدف
- ✅ **Rate Limiting** لكل IP باستخدام Token Bucket
- ✅ **قوائم حظر IP** ديناميكية
- ✅ **حدود PPS/BPS** قابلة للتخصيص
- ✅ **إغلاق فوري** للاتصالات المشبوهة

### ⚡ الأداء
- 🚀 **I/O غير متزامن** عبر `Asio` (بدون Blocking)
- 🚀 **بنية PIMPL** لتقليل الاعتماديات
- 🚀 **إدارة ذاكرة آمنة** عبر `shared_ptr` و`unique_ptr`
- 🚀 **تسجيل عالي الأداء** عبر `spdlog`

### 🛠️ التطوير
- 🧩 **بنية Modular** قابلة للتوسع
- 🧪 **اختبارات وحدة** عبر GoogleTest
- 🔄 **CI/CD** عبر GitHub Actions
- 🐳 **دعم Docker** و`docker-compose`

---

## 🏗️ هيكل المشروع

├── .github/
│   └── workflows/
│       └── ci.yml                  # GitHub Actions
├── config/
│   └── zucks_config.json           # التكوين
├── docker/
│   ├── Dockerfile                  # صورة Docker
│   └── docker-compose.yml          # Compose
├── scripts/
│   ├── build.sh                    # سكربت البناء
│   ├── run.sh                      # سكربت التشغيل
│   └── clean.sh                    # سكربت التنظيف
├── src/
│   ├── main.cpp                    # نقطة الدخول
│   ├── zucks_core.{hpp,cpp}        # النواة
│   ├── zucks_proxy.{hpp,cpp}       # البروكسي
│   ├── zucks_forwarder.{hpp,cpp}   # مسار التحويل
│   ├── zucks_filter.{hpp,cpp}      # الفلتر الأمني
│   ├── zucks_config.{hpp,cpp}      # التكوين
│   └── zucks_logger.hpp            # التسجيل
├── tests/
│   └── test_zucks_filter.cpp       # اختبارات الوحدة
├── CMakeLists.txt                  # ملف البناء
├── Makefile                        # أوامر مختصرة
└── README.md


---

## 🚀 البناء والتشغيل

### 📋 المتطلبات

| المتطلب | الإصدار الأدنى |
|---------|----------------|
| C++ Compiler | GCC 10+ / Clang 12+ / MSVC 2019+ |
| CMake | 3.20+ |
| Git | 2.30+ |
| Python | 3.x (اختياري، لـ CMake) |

> المكتبات `Asio` و`spdlog` و`nlohmann_json` يتم تحميلها تلقائيًا عبر `FetchContent`، لا حاجة لتثبيتها يدويًا.

### 🔨 خطوات البناء

```bash
# 1) استنساخ المشروع
git clone https://github.com/zuc8k/ZucksServer.CF.git
cd ZucksServer.CF

# 2) تجهيز السكربتات
chmod +x scripts/*.sh

# 3) البناء
./scripts/build.sh

# أو يدويًا
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DZUCKS_BUILD_TESTS=ON
cmake --build build -j$(nproc)

▶️ التشغيل

# عبر السكربت
./scripts/run.sh

# أو مباشرة
./build/zucks-server config/zucks_config.json



عند التشغيل، يجب أن ترى:

[2026-10-29 ...] [Zucks] [info] Starting Zucks Server
[2026-10-29 ...] [Zucks] [info] Zucks config loaded: listen 28888 -> 127.0.0.1:18889
[2026-10-29 ...] [Zucks] [info] Zucks Server running 

🧪 الاختبارات
ctest --test-dir build --output-on-failure
# أو
make test
⚙️ التكوين
 
يتم التكوين عبر ملف JSON:

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

📝 شرح الحقول
الحقل الوصف القيمة الافتراضية
version إصدار Zucks 1.1.0
log_level مستوى التسجيل (trace/debug/info/warn/error/critical) info
listen_port منفذ استماع Zucks 28888
target_host عنوان السيرفر الهدف 127.0.0.1
target_port منفذ السيرفر الهدف 18889
security.max_pps أقصى حزم/ثانية لكل IP 1500
security.max_bps أقصى بايت/ثانية لكل IP 5242880
security.max_connections_per_ip أقصى اتصالات متزامنة/IP 30
security.blacklist_enabled تفعيل قائمة الحظر true







