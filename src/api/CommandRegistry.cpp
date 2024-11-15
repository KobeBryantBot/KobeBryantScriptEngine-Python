#include "../core/PythonPluginEngine.hpp"
#include <api/CommandRegistry.hpp>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

CommandRegistry::CommandRegistry() = default;

class ScriptCommandRegistry : public CommandRegistry {
    friend CommandRegistry;

public:
    static ScriptCommandRegistry& getInstance() {
        static std::unique_ptr<ScriptCommandRegistry> instance;
        if (!instance) {
            instance = std::make_unique<ScriptCommandRegistry>();
        }
        return *instance;
    }

    bool _registerSimpleCommand(
        const std::string&                                   plugin,
        const std::string&                                   cmd,
        std::function<void(const std::vector<std::string>&)> callback
    ) {
        return registerSimpleCommand(plugin, cmd, callback);
    }

    bool _unregisterCommand(const std::string& plugin, const std::string& cmd) {
        return unregisterCommand(plugin, cmd);
    }
};

void initCommandRegistry(py::module_& m) {
    py::class_<ScriptCommandRegistry>(m, "CommandRegistry")
        .def_static(
            "registerSimpleCommand",
            [](std::string const& cmd, std::function<void(std::vector<std::string> const&)> callback) {
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    return ScriptCommandRegistry::getInstance()
                        ._registerSimpleCommand(*plugin, cmd, std::move(callback));
                }
                return false;
            }
        )
        .def_static(
            "unregisterCommand",
            [](std::string const cmd) -> bool {
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    return ScriptCommandRegistry::getInstance()._unregisterCommand(*plugin, cmd);
                }
                return false;
            }
        )
        .def_static("executeCommand", [](std::string const cmd) {
            ScriptCommandRegistry::getInstance().executeCommand(cmd);
        });
}
