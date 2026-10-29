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
