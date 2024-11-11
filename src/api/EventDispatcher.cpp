#include "EventDispatcher.hpp"

Logger logger("KobeBryantScriptEngine-Python");

bool ScriptListener::operator<(const ScriptListener& rhs) const { return mId < rhs.mId; }
bool ScriptListener::operator==(const ScriptListener& rhs) const { return mId == rhs.mId; }


ScriptEventBusImpl::ScriptEventBusImpl() {
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
                        packet
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
                        packet
                    );
                }
                case utils::doHash("notice"): {
                    std::string notice_type = packet["notice_type"];
                    return ScriptEventBusImpl::getInstance().publish(
                        fmt::format("{}.{}", post_type, notice_type),
                        packet
                    );
                }
                case utils::doHash("request"): {
                    std::string request_type = packet["request_type"];
                    return ScriptEventBusImpl::getInstance().publish(
                        fmt::format("{}.{}", post_type, request_type),
                        packet
                    );
                }
                }
            }
        } catch (const std::exception& e) {
            logger.error("engine.python.event.exception", {e.what()});
        } catch (...) {
            logger.error("engine.python.event.unknownException");
        }
    });
}

ScriptEventBusImpl& ScriptEventBusImpl::getInstance() {
    static std::unique_ptr<ScriptEventBusImpl> instance;
    if (!instance) {
        instance = std::make_unique<ScriptEventBusImpl>();
    }
    return *instance;
}

ScriptListener ScriptEventBusImpl::add(std::string const& event, std::function<void(nlohmann::json const&)> func) {
    auto id = mNextId;
    mNextId++;
    auto listener        = ScriptListener(id, event);
    mListeners[listener] = std::move(func);
    return listener;
}

bool ScriptEventBusImpl::remove(ScriptListener const& listener) {
    if (mListeners.contains(listener)) {
        mListeners.erase(listener);
        return true;
    }
    return false;
}

void ScriptEventBusImpl::publish(std::string const& event, nlohmann::json const& data) {
    try {
        for (auto& [listener, func] : mListeners) {
            if (listener.mType == event && func) {
                func(data);
            }
        }
    } catch (const std::exception& e) {
        logger.error("engine.python.event.exception", {e.what()});
    } catch (...) {
        logger.error("engine.python.event.unknownException");
    }
}

void ScriptEventBusImpl::addPluginListener(std::string const& plugin, ScriptListener const& listener) {
    mPluginListeners[plugin].insert(listener);
}

void ScriptEventBusImpl::removePluginListener(std::string const& plugin, ScriptListener const& listener) {
    mPluginListeners[plugin].erase(listener);
}

void ScriptEventBusImpl::removePluginListeners(std::string const& plugin) {
    for (auto& listener : mPluginListeners[plugin]) {
        remove(listener);
    }
    mPluginListeners.erase(plugin);
}

class ScriptEventBus {
public:
    static ScriptListener add(std::string const& event, std::function<void(nlohmann::json const&)> func) {
        return ScriptEventBusImpl::getInstance().add(event, std::move(func));
    }

    static bool remove(ScriptListener const& listener) {
        return ScriptEventBusImpl::getInstance().remove(std::move(listener));
    }
};


void initEvent(py::module_& m) {
    py::class_<ScriptListener>(m, "Listener")
        .def_readonly("mId", &ScriptListener::mId)
        .def_readonly("mType", &ScriptListener::mType);

    py::class_<ScriptEventBus>(m, "EventBus")
        .def_static(
            "add",
            [](std::string const& event, std::function<void(nlohmann::json const&)> func) -> ScriptListener {
                auto listener = ScriptEventBus::add(event, func);
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    ScriptEventBusImpl::getInstance().addPluginListener(*plugin, listener);
                }
                return listener;
            }
        )
        .def_static("remove", [](ScriptListener const& listener) -> bool {
            if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                ScriptEventBusImpl::getInstance().removePluginListener(*plugin, listener);
            }
            return ScriptEventBus::remove(listener);
        });
}