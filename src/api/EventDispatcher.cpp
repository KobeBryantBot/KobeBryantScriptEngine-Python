#include "EventDispatcher.hpp"
#include "api/event/CustomEvent.hpp"

Logger logger{};

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
                    auto        eventName       = fmt::format("{}.{}", post_type, meta_event_type);
                    auto        event           = CustomEvent(eventName, packet);
                    return ScriptEventBusImpl::getInstance().publish(eventName, event);
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
                    auto eventName        = fmt::format("{}.{}.{}", post_type, message_type, sub_type);
                    auto event            = CustomEvent(eventName, packet);
                    return ScriptEventBusImpl::getInstance().publish(eventName, event);
                }
                case utils::doHash("notice"): {
                    std::string notice_type = packet["notice_type"];
                    auto        eventName   = fmt::format("{}.{}", post_type, notice_type);
                    auto        event       = CustomEvent(eventName, packet);
                    return ScriptEventBusImpl::getInstance().publish(eventName, event);
                }
                case utils::doHash("request"): {
                    std::string request_type = packet["request_type"];
                    auto        eventName    = fmt::format("{}.{}", post_type, request_type);
                    auto        event        = CustomEvent(eventName, packet);
                    return ScriptEventBusImpl::getInstance().publish(eventName, event);
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

ScriptListener
ScriptEventBusImpl::add(std::string const& event, std::function<void(CustomEvent&)> func, uint32_t priority) {
    auto id = mNextId;
    mNextId++;
    auto listener          = ScriptListener(id, event);
    mCallbacks[listener]   = std::move(func);
    mListenerMap[listener] = priority;
    mListenerPriority[priority].insert(listener);
    return listener;
}

bool ScriptEventBusImpl::remove(ScriptListener const& listener) {
    if (mCallbacks.contains(listener)) {
        mCallbacks.erase(listener);
        auto priority = mListenerMap[listener];
        mListenerPriority[priority].erase(listener);
        mListenerMap.erase(listener);
        return true;
    }
    return false;
}

void ScriptEventBusImpl::publish(std::string const& event, CustomEvent& ev) {
    try {
        for (auto& [priority, listeners] : mListenerPriority) {
            for (auto& listener : listeners) {
                if (auto& callback = mCallbacks[listener]) {
                    if (listener.mType == event) {
                        callback(ev);
                        if (ev.isPassingBlocked()) return;
                    }
                }
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
    static ScriptListener add(std::string const& event, std::function<void(CustomEvent&)> func, uint32_t priority) {
        auto listener = ScriptEventBusImpl::getInstance().add(event, std::move(func), priority);
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            ScriptEventBusImpl::getInstance().addPluginListener(*plugin, listener);
        }
        return listener;
    }

    static bool remove(ScriptListener const& listener) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            ScriptEventBusImpl::getInstance().removePluginListener(*plugin, listener);
        }
        return ScriptEventBusImpl::getInstance().remove(std::move(listener));
    }

    static void emit(std::string const& event, CustomEvent& data) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            ScriptEventBusImpl::getInstance().publish(event, data);
            EventBus::getInstance().publish(data);
        }
    }
};

void initEvent(py::module_& m) {
    py::class_<ScriptListener>(m, "Listener")
        .def_readonly("mId", &ScriptListener::mId)
        .def_readonly("mType", &ScriptListener::mType);

    py::class_<CustomEvent>(m, "Event")
        .def_static(
            "newEvent",
            [](std::string const& event, nlohmann::json const& data) -> CustomEvent {
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    return CustomEvent(event, data, *plugin);
                }
                return CustomEvent("invalid_event", nlohmann::json::object(), "invalid_plugin");
            }
        )
        .def_readwrite("mEventData", &CustomEvent::mEventData)
        .def("block_pass", &CustomEvent::block_pass);

    py::class_<ScriptEventBus>(m, "EventBus")
        .def_static("add", &ScriptEventBus::add, py::arg(), py::arg(), py::arg() = 500)
        .def_static("remove", &ScriptEventBus::remove)
        .def_static("emit", &ScriptEventBus::emit);
}