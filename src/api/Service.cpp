#include "api/Service.hpp"
#include "../core/PythonPluginEngine.hpp"
#include <any>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11_json/pybind11_json.hpp>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace py = pybind11;

class ScriptService : public Service {
    friend Service;

    using ScriptType = std::variant<std::monostate, int64_t, double, std::string, bool, nlohmann::json>;

public:
    static py::object any_to_py(std::any const& any_value) {
        auto& type = any_value.type();
        if (type == typeid(void) || type == typeid(std::nullptr_t)) {
            return py::none();
        } else if (type == typeid(int64_t)) {
            return py::cast(std::any_cast<int64_t>(any_value));
        } else if (type == typeid(double)) {
            return py::cast(std::any_cast<double>(any_value));
        } else if (type == typeid(std::string)) {
            return py::cast(std::any_cast<std::string>(any_value));
        } else if (type == typeid(bool)) {
            return py::cast(std::any_cast<bool>(any_value));
        } else if (type == typeid(nlohmann::json)) {
            return py::cast(std::any_cast<nlohmann::json>(any_value));
        } else {
            throw std::runtime_error("ScriptEngine received an unsupported C++ argument type!");
        }
    }

    static std::any variant_to_any(ScriptType const& var) {
        if (std::holds_alternative<int64_t>(var)) {
            return std::any(std::get<int64_t>(var));
        } else if (std::holds_alternative<double>(var)) {
            return std::any(std::get<double>(var));
        } else if (std::holds_alternative<std::string>(var)) {
            return std::any(std::get<std::string>(var));
        } else if (std::holds_alternative<bool>(var)) {
            return std::any(std::get<bool>(var));
        } else if (std::holds_alternative<nlohmann::json>(var)) {
            return std::any(std::get<nlohmann::json>(var));
        } else {
            return std::any();
        }
    }

    static bool removeFunc(std::string const& funcName) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            return Service::removeFunc(*plugin, funcName);
        }
        return false;
    }

    static bool exportFunc(std::string const& funcName, py::object py_func) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            auto func = [py_func](std::vector<std::any> const& args) -> std::any {
                py::tuple args_tuple(args.size());
                for (size_t i = 0; i < args.size(); ++i) {
                    args_tuple[i] = any_to_py(args[i]);
                }
                py::object result = py_func(*args_tuple);
                return variant_to_any(py::cast<ScriptType>(result));
            };
            return exportAnyFunc(*plugin, funcName, func);
        }
        return false;
    }

    static py::cpp_function importFunc(std::string const& pluginName, std::string const& funcName) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            auto func = importAnyFunc(pluginName, funcName);
            return py::cpp_function([func](py::args args) -> py::object {
                std::vector<std::any> cxx_args;
                for (auto& arg : args) {
                    cxx_args.push_back(variant_to_any(py::cast<ScriptType>(arg)));
                }
                auto result = func(cxx_args);
                return any_to_py(result);
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