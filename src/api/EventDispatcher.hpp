#pragma once
#include "../core/PythonPluginEngine.hpp"
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

struct ScriptListener {
    uint64_t    mId;
    std::string mType;
    bool        operator<(const ScriptListener& rhs) const;
    bool        operator==(const ScriptListener& rhs) const;
};

class ScriptEventBusImpl {
    std::map<ScriptListener, std::function<void(std::string const&)>> mListeners;
    uint64_t                                                          mNextId = 0;
    std::unordered_map<std::string, std::set<ScriptListener>>         mPluginListeners;

public:
    ScriptEventBusImpl();

    static ScriptEventBusImpl& getInstance();

    ScriptListener add(std::string const& event, std::function<void(std::string const&)> func);

    bool remove(ScriptListener const& listener);

    void publish(std::string const& event, std::string const& data);

    void addPluginListener(std::string const& plugin, ScriptListener const& listener);

    void removePluginListener(std::string const& plugin, ScriptListener const& listener);

    void removePluginListeners(std::string const& plugin);
};