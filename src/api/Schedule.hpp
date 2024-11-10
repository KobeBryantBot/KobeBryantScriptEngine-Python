#pragma once
#include "api/Schedule.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class ScriptSchedule {
    std::unordered_map<std::string, std::unordered_set<size_t>> mPluginTasks;

public:
    static size_t addDelayTask(int delay, std::function<void()> func);

    static size_t addRepeatTask(int delay, std::function<void()> func, bool immediately);

    static size_t addRepeatTask(int delay, std::function<void()> func, bool immediately, uint64_t times);

    static bool cancelTask(size_t id);

    static ScriptSchedule& getInstance();

    void addPluginTask(std::string const& plugin, size_t taskId);

    void removePluginTask(std::string const& plugin, size_t taskId);

    void removePluginTasks(std::string const& plugin);
};
