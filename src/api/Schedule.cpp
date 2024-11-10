#include "Schedule.hpp"
#include "../core/PythonPluginEngine.hpp"

size_t ScriptSchedule::addDelayTask(int delay, std::function<void()> func) {
    return Schedule::addDelayTask(std::chrono::seconds(delay), func);
}

size_t ScriptSchedule::addRepeatTask(int delay, std::function<void()> func, bool immediately) {
    return Schedule::addRepeatTask(std::chrono::seconds(delay), func, immediately);
}

size_t ScriptSchedule::addRepeatTask(int delay, std::function<void()> func, bool immediately, uint64_t times) {
    return Schedule::addRepeatTask(std::chrono::seconds(delay), func, immediately, times);
}

bool ScriptSchedule::cancelTask(size_t id) { return Schedule::cancelTask(id); }

ScriptSchedule& ScriptSchedule::getInstance() {
    static std::unique_ptr<ScriptSchedule> instance;
    if (!instance) {
        instance = std::make_unique<ScriptSchedule>();
    }
    return *instance;
}

void ScriptSchedule::addPluginTask(std::string const& plugin, size_t taskId) { mPluginTasks[plugin].insert(taskId); }

void ScriptSchedule::removePluginTask(std::string const& plugin, size_t taskId) { mPluginTasks[plugin].erase(taskId); }

void ScriptSchedule::removePluginTasks(std::string const& plugin) {
    for (auto& taskId : mPluginTasks[plugin]) {
        cancelTask(taskId);
    }
    mPluginTasks.erase(plugin);
}

void initSchedule(py::module_& m) {
    py::class_<ScriptSchedule>(m, "Schedule")
        .def_static(
            "addDelayTask",
            [](int delay, std::function<void()> task) -> size_t {
                auto taskId = ScriptSchedule::addDelayTask(delay, task);
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    ScriptSchedule::getInstance().addPluginTask(*plugin, taskId);
                }
                return taskId;
            }
        )
        .def_static(
            "addRepeatTask",
            [](int delay, std::function<void()> task, bool immediately) -> size_t {
                auto taskId = ScriptSchedule::addRepeatTask(delay, task, immediately);
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    ScriptSchedule::getInstance().addPluginTask(*plugin, taskId);
                }
                return taskId;
            },
            py::arg(),
            py::arg(),
            py::arg() = false
        )
        .def_static(
            "addRepeatTask",
            [](int delay, std::function<void()> task, bool immediately, uint64_t times) -> size_t {
                auto taskId = ScriptSchedule::addRepeatTask(delay, task, immediately, times);
                if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                    ScriptSchedule::getInstance().addPluginTask(*plugin, taskId);
                }
                return taskId;
            }
        )
        .def_static("cancelTask", [](size_t taskId) -> bool {
            if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                ScriptSchedule::getInstance().removePluginTask(*plugin, taskId);
            }
            return ScriptSchedule::cancelTask(taskId);
        });
}
