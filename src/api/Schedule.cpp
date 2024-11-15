#include "api/Schedule.hpp"
#include "../core/PythonPluginEngine.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class ScriptSchedule : public Schedule {
    friend Schedule;

public:
    static size_t addDelayTask(int delay, std::function<void()> func) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return addDelay(*plugin, std::chrono::milliseconds(delay), func);
        }
        return -1;
    }

    static size_t addRepeatTask(int delay, std::function<void()> func, bool immediately) {
        if (immediately) {
            func();
        }
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return addRepeat(*plugin, std::chrono::seconds(delay), func);
        }
        return -1;
    }

    static size_t addRepeatTask(int delay, std::function<void()> func, bool immediately, uint64_t times) {
        if (immediately) {
            func();
            times--;
        }
        if (times >= 1) {
            if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
                return addRepeat(*plugin, std::chrono::seconds(delay), func, times);
            }
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
            py::overload_cast<int, std::function<void()>, bool>(&ScriptSchedule::addRepeatTask),
            py::arg(),
            py::arg(),
            py::arg() = false
        )
        .def_static(
            "addRepeatTask",
            py::overload_cast<int, std::function<void()>, bool, uint64_t>(&ScriptSchedule::addRepeatTask)
        )
        .def_static("cancelTask", &ScriptSchedule::cancelTask);
}
