#include "api/Schedule.hpp"
#include "../core/PythonPluginEngine.hpp"
#include <croncpp/croncpp.h>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class ScriptSchedule : public Schedule {
    friend Schedule;

public:
    static size_t addDelayTask(uint64_t delay, std::function<void()>&& func) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return addDelay(*plugin, std::chrono::milliseconds(delay), std::move(func));
        }
        return -1;
    }

    static size_t addRepeatTask(uint64_t delay, std::function<void()>&& func, bool immediately) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return addRepeat(*plugin, std::chrono::milliseconds(delay), std::move(func), immediately);
        }
        return -1;
    }

    static size_t addRepeatTask(uint64_t delay, std::function<void()>&& func, bool immediately, uint64_t times) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return addRepeat(*plugin, std::chrono::milliseconds(delay), std::move(func), immediately, times);
        }
        return -1;
    }

    static inline TaskID addConditionTask(Task&& task, std::function<bool()>&& condition) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return addCondition(*plugin, std::move(task), std::move(condition));
        }
        return -1;
    }

    static inline TaskID addConditionTask(Task&& task, std::function<bool()>&& condition, size_t times) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return addCondition(*plugin, std::move(task), std::move(condition), times);
        }
        return -1;
    }

    static inline TaskID addCronTask(const std::string& cron, Task&& task) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            cron::make_cron(cron);
            return addCron(*plugin, cron, std::move(task));
        }
        return -1;
    }

    static inline TaskID addCronTask(const std::string& cron, Task&& task, size_t times) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            cron::make_cron(cron);
            return addCron(*plugin, cron, std::move(task), times);
        }
        return -1;
    }

    static bool cancelTask(size_t id) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return cancel(*plugin, id);
        }
        return false;
    }
};

void initSchedule(py::module_& m) {
    py::class_<ScriptSchedule>(m, "Schedule")
        .def_static("addDelayTask", &ScriptSchedule::addDelayTask)
        .def_static(
            "addRepeatTask",
            py::overload_cast<uint64_t, std::function<void()>&&, bool>(&ScriptSchedule::addRepeatTask),
            py::arg(),
            py::arg(),
            py::arg() = false
        )
        .def_static(
            "addRepeatTask",
            py::overload_cast<uint64_t, std::function<void()>&&, bool, uint64_t>(&ScriptSchedule::addRepeatTask)
        )
        .def_static(
            "addConditionTask",
            py::overload_cast<std::function<void()>&&, std::function<bool()>&&>(&ScriptSchedule::addConditionTask)
        )
        .def_static(
            "addConditionTask",
            py::overload_cast<std::function<void()>&&, std::function<bool()>&&, uint64_t>(
                &ScriptSchedule::addConditionTask
            )
        )
        .def_static(
            "addCronTask",
            py::overload_cast<const std::string&, std::function<void()>&&>(&ScriptSchedule::addCronTask)
        )
        .def_static(
            "addCronTask",
            py::overload_cast<const std::string&, std::function<void()>&&, uint64_t>(&ScriptSchedule::addCronTask)
        )
        .def_static("cancelTask", &ScriptSchedule::cancelTask);
}
