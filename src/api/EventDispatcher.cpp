#include "api/EventBus.hpp"
#include "api/Logger.hpp"
#include "api/event/LoggerOutputEvent.hpp"
#include "api/event/MessageEvent.hpp"
#include "api/event/MetaEvent.hpp"
#include "api/event/NoticeEvent.hpp"
#include "api/event/PacketEvent.hpp"
#include "api/event/RequestEvent.hpp"
#include "api/utils/StringUtils.hpp"
#include "magic_enum/magic_enum.hpp"
#include "nlohmann/json.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

Logger logger("KobeBryantScriptEngine-Python");

struct ScriptListener {
    uint64_t    mId;
    std::string mType;
    bool        operator<(const ScriptListener& rhs) const { return mId < rhs.mId; }
    bool        operator==(const ScriptListener& rhs) const { return mId == rhs.mId; }
};

class ScriptEventBusImpl {
    std::map<ScriptListener, std::function<void(std::string const&)>> mListeners;
    uint64_t                                                          mNextId = 0;

public:
    ScriptEventBusImpl() {
        EventBus::getInstance().subscribe<PacketEvent>([](PacketEvent const& ev) {
            try {
                auto packet = ev.mPacket;
                if (packet.contains("post_type")) {
                    std::string post_type = packet["post_type"];
                    switch (utils::doHash(post_type)) {
                    case utils::doHash("meta_event"): {
                        std::string meta_event_type = packet["meta_event_type"];
                        return ScriptEventBusImpl::getInstance().publish(
                            fmt::format("{}.{}", post_type, meta_event_type),
                            packet.dump()
                        );
                    }
                    case utils::doHash("message"): {
                        std::string message_type = packet["message_type"];
                        std::string sub_type     = packet["sub_type"];
                        std::string rawMessage   = packet["raw_message"];
                        utils::ReplaceStr(rawMessage, "&#91;", "[");
                        utils::ReplaceStr(rawMessage, "&#93;", "]");
                        utils::ReplaceStr(rawMessage, "&#44;", ",");
                        utils::ReplaceStr(rawMessage, "&amp;", "&");
                        packet["raw_message"] = rawMessage;
                        return ScriptEventBusImpl::getInstance().publish(
                            fmt::format("{}.{}.{}", post_type, message_type, sub_type),
                            packet.dump()
                        );
                    }
                    case utils::doHash("notice"): {
                        std::string notice_type = packet["notice_type"];
                        return ScriptEventBusImpl::getInstance().publish(
                            fmt::format("{}.{}", post_type, notice_type),
                            packet.dump()
                        );
                    }
                    case utils::doHash("request"): {
                        std::string request_type = packet["request_type"];
                        return ScriptEventBusImpl::getInstance().publish(
                            fmt::format("{}.{}", post_type, request_type),
                            packet.dump()
                        );
                    }
                    }
                }
            } catch (const std::exception& e) {
                logger.error("解析事件时发生异常: {}", e.what());
            } catch (...) {
                logger.error("解析事件时发生未知异常");
            }
        });
    }

    static ScriptEventBusImpl& getInstance() {
        static std::unique_ptr<ScriptEventBusImpl> instance;
        if (!instance) {
            instance = std::make_unique<ScriptEventBusImpl>();
        }
        return *instance;
    }

    ScriptListener add(std::string const& event, std::function<void(std::string const&)> func) {
        auto id = mNextId;
        mNextId++;
        auto listener        = ScriptListener(id, event);
        mListeners[listener] = std::move(func);
        return listener;
    }

    bool remove(ScriptListener const& listener) {
        if (mListeners.contains(listener)) {
            mListeners.erase(listener);
            return true;
        }
        return false;
    }

    void publish(std::string const& event, std::string const& data) {
        try {
            for (auto& [listener, func] : mListeners) {
                if (listener.mType == event && func) {
                    func(data);
                }
            }
        } catch (const std::exception& e) {
            logger.error("发布事件时发生异常: {}", e.what());
        } catch (...) {
            logger.error("发布事件时发生未知异常");
        }
    }
};

class ScriptEventBus {
public:
    static ScriptListener add(std::string const& event, std::function<void(std::string const&)> func) {
        return ScriptEventBusImpl::getInstance().add(event, std::move(func));
    }

    static bool remove(ScriptListener const& listener) {
        return ScriptEventBusImpl::getInstance().remove(std::move(listener));
    }
};


PYBIND11_EMBEDDED_MODULE(EventAPI, m) {
    py::class_<ScriptListener>(m, "Listener")
        .def_readonly("mId", &ScriptListener::mId)
        .def_readonly("mType", &ScriptListener::mType);

    py::class_<ScriptEventBus>(m, "EventBus")
        .def_static("add", &ScriptEventBus::add)
        .def_static("remove", &ScriptEventBus::remove);
}
