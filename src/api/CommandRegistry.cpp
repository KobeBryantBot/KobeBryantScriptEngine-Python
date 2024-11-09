#include "CommandRegistry.hpp"
#include "../core/PythonPluginEngine.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

ScriptCommandManager& ScriptCommandManager::getInstance() {
    static std::unique_ptr<ScriptCommandManager> instance;
    if (!instance) {
        instance = std::make_unique<ScriptCommandManager>();
    }
    return *instance;
}

void ScriptCommandManager::addPluginCommand(std::string const& plugin, std::string const& cmd) {
    mPluginCommands[plugin].insert(cmd);
}

void ScriptCommandManager::removePluginCommand(std::string const& plugin, std::string const& cmd) {
    mPluginCommands[plugin].erase(cmd);
}

void ScriptCommandManager::removePluginCommands(std::string const& plugin) {
    for (auto& cmd : mPluginCommands[plugin]) {
        CommandRegistry::getInstance().unregisterCommand(cmd);
    }
    mPluginCommands.erase(plugin);
}

PYBIND11_EMBEDDED_MODULE(CommandRegistryAPI, m) {
    py::class_<CommandRegistry>(m, "CommandRegistry")
        .def_static(
            "registerCommand",
            [](std::string const& cmd, std::function<void(std::vector<std::string> const&)> callback) {
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    ScriptCommandManager::getInstance().addPluginCommand(*plugin, cmd);
                }
                return CommandRegistry::getInstance().registerCommand(cmd, std::move(callback));
            }
        )
        .def_static(
            "unregisterCommand",
            [](std::string const cmd) -> bool {
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    ScriptCommandManager::getInstance().removePluginCommand(*plugin, cmd);
                }
                return CommandRegistry::getInstance().unregisterCommand(cmd);
            }
        )
        .def_static("executeCommand", [](std::string const cmd) {
            CommandRegistry::getInstance().executeCommand(cmd);
        });
}
