#include "api/Logger.hpp"
#include "../core/PythonPluginEngine.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

class ScriptLogger : public Logger {
public:
    ScriptLogger() : Logger() {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            setTitle(*plugin);
        }
    }
    ScriptLogger(std::string const& title) : Logger(title) {}

    void log(LogLevel level, std::string const& content) { return printView(level, content); }
    void fatal(std::string const& content) { return printView(LogLevel::Fatal, content); }
    void error(std::string const& content) { return printView(LogLevel::Error, content); }
    void warn(std::string const& content) { return printView(LogLevel::Warn, content); }
    void info(std::string const& content) { return printView(LogLevel::Info, content); }
    void debug(std::string const& content) { return printView(LogLevel::Debug, content); }
};

void initLogger(py::module_& m) {
    py::class_<ScriptLogger> LoggerClass(m, "Logger");
    LoggerClass.def(py::init<>())
        .def(py::init<std::string const&>())
        .def("setTitle", &ScriptLogger::setTitle)
        .def("setLevel", &ScriptLogger::setLevel)
        .def("setFile", &ScriptLogger::setFile)
        .def("log", &ScriptLogger::log)
        .def("fatal", &ScriptLogger::fatal)
        .def("error", &ScriptLogger::error)
        .def("warn", &ScriptLogger::warn)
        .def("info", &ScriptLogger::info)
        .def("debug", &ScriptLogger::debug);

    py::enum_<Logger::LogLevel>(LoggerClass, "LogLevel")
        .value("Trace", Logger::LogLevel::Trace)
        .value("Fatal", Logger::LogLevel::Fatal)
        .value("Error", Logger::LogLevel::Error)
        .value("Warn", Logger::LogLevel::Warn)
        .value("Info", Logger::LogLevel::Info)
        .value("Debug", Logger::LogLevel::Debug)
        .export_values();
}
