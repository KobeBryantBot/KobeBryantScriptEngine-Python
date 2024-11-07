#include "api/Schedule.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class ScriptSchedule {
public:
    static size_t addDelayTask(std::chrono::seconds delay, std::function<void()> func) {
        return Schedule::addDelayTask(delay, std::move(func));
    }

    static size_t addRepeatTask(std::chrono::seconds delay, std::function<void()> func, bool immediately) {
        return Schedule::addRepeatTask(delay, std::move(func), immediately);
    }

    static size_t
    addRepeatTask(std::chrono::seconds delay, std::function<void()> func, bool immediately, uint64_t times) {
        return Schedule::addRepeatTask(delay, std::move(func), immediately, times);
    }

    static bool cancelTask(size_t id) { return Schedule::cancelTask(id); }
};


PYBIND11_EMBEDDED_MODULE(ScheduleAPI, m) {
    py::class_<ScriptSchedule>(m, "Schedule")
        .def_static("addDelayTask", &ScriptSchedule::addDelayTask)
        .def_static(
            "addRepeatTask",
            py::overload_cast<std::chrono::seconds, std::function<void()>, bool>(&ScriptSchedule::addRepeatTask),
            py::arg(),
            py::arg(),
            py::arg() = false
        )
        .def_static(
            "addRepeatTask",
            py::overload_cast<std::chrono::seconds, std::function<void()>, bool, uint64_t>(
                &ScriptSchedule::addRepeatTask
            )
        )
        .def_static("cancelTask", &ScriptSchedule::cancelTask);
}
