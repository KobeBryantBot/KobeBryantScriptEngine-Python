#include "api/Service.hpp"
#include "../core/PythonPluginEngine.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <unordered_map>
#include <unordered_set>

namespace py = pybind11;

class ScriptService : public Service {
    friend Service;

public:
    static bool removeFunc(std::string const& funcName) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return Service::removeFunc(*plugin, funcName);
        }
        return false;
    }

    static bool exportFunc(std::string const& funcName, py::object py_func) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            auto func = [py_func](const std::vector<std::any>& args) -> std::any {
                py::tuple args_tuple(args.size());
                for (size_t i = 0; i < args.size(); ++i) {
                    args_tuple[i] = py::cast(args[i]);
                }
                py::object result = py_func(*args_tuple);
                return py::cast<std::any>(result);
            };
            return exportAnyFunc(*plugin, funcName, func);
        }
        return false;
    }

    static py::cpp_function importFunc(std::string const& pluginName, std::string const& funcName) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            auto func = importAnyFunc(pluginName, funcName);
            return py::cpp_function([func](py::args args) -> py::object {
                // 如果没有提供参数，传递一个空的vector
                std::vector<std::any> cxx_args;
                for (auto& arg : args) {
                    try {
                        cxx_args.push_back(py::cast<std::any>(arg));
                    } catch (const py::cast_error& e) {
                        throw;
                    }
                }
                std::any result = func(cxx_args);
                try {
                    if (result.type() == typeid(void)) {
                        return py::none();
                    }
                    return py::cast(result);
                } catch (const std::bad_any_cast&) {
                    return py::none();
                }
                return py::none();
            });
        }
        return nullptr;
    }
};

void initService(py::module_& m) {
    py::class_<ScriptService>(m, "Service")
        .def_static("hasFunc", &ScriptService::hasFunc)
        .def_static("exportFunc", &ScriptService::exportFunc)
        .def_static("importFunc", &ScriptService::importFunc)
        .def_static("removeFunc", &ScriptService::removeFunc);
}