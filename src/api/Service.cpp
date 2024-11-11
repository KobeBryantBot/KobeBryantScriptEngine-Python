#include "api/Service.hpp"
#include "../core/PythonPluginEngine.hpp"
#include <any>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

namespace py = pybind11;

class ScriptService : public Service {
    friend Service;

public:
    static py::object any_to_py(std::any const& any_value) {
        auto& type = any_value.type();
        if (type == typeid(ScriptType)) {
            return py::cast(std::any_cast<ScriptType>(any_value));
        } else if (type == typeid(void) || type == typeid(std::nullptr_t)) {
            return py::none();
        } else if (type == typeid(int8_t)) {
            return py::cast(std::any_cast<int8_t>(any_value));
        } else if (type == typeid(uint8_t)) {
            return py::cast(std::any_cast<uint8_t>(any_value));
        } else if (type == typeid(int16_t)) {
            return py::cast(std::any_cast<int16_t>(any_value));
        } else if (type == typeid(uint16_t)) {
            return py::cast(std::any_cast<uint16_t>(any_value));
        } else if (type == typeid(int)) {
            return py::cast(std::any_cast<int>(any_value));
        } else if (type == typeid(uint32_t)) {
            return py::cast(std::any_cast<uint32_t>(any_value));
        } else if (type == typeid(int64_t)) {
            return py::cast(std::any_cast<int64_t>(any_value));
        } else if (type == typeid(uint64_t)) {
            return py::cast(std::any_cast<uint64_t>(any_value));
        } else if (type == typeid(double)) {
            return py::cast(std::any_cast<double>(any_value));
        } else if (type == typeid(float)) {
            return py::cast(std::any_cast<float>(any_value));
        } else if (type == typeid(std::string)) {
            return py::cast(std::any_cast<std::string>(any_value));
        } else if (type == typeid(bool)) {
            return py::cast(std::any_cast<bool>(any_value));
        } else {
            throw std::runtime_error("Unsupported C++ type in function result");
        }
    }

    static std::any variant_to_any(Service::ScriptType const& var) {
        if (std::holds_alternative<int64_t>(var)) {
            return std::any(std::get<int64_t>(var));
        } else if (std::holds_alternative<double>(var)) {
            return std::any(std::get<double>(var));
        } else if (std::holds_alternative<std::string>(var)) {
            return std::any(std::get<std::string>(var));
        } else if (std::holds_alternative<bool>(var)) {
            return std::any(std::get<bool>(var));
        } else if (std::holds_alternative<std::vector<Service::ScriptTypeBase>>(var)) {
            return std::any(std::get<std::vector<Service::ScriptTypeBase>>(var));
        } else if (std::holds_alternative<std::unordered_map<std::string, Service::ScriptTypeBase>>(var)) {
            return std::any(std::get<std::unordered_map<std::string, Service::ScriptTypeBase>>(var));
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
                    args_tuple[i] = py::cast(cast_type<ScriptType>(args[i]));
                }
                py::object result = py_func(*args_tuple);
                return std::any(py::cast<ScriptType>(result));
            };
            return exportAnyFunc(*plugin, funcName, func);
        }
        return false;
    }

    static py::cpp_function importFunc(std::string const& pluginName, std::string const& funcName) {
        if (auto plugin = PythonPluginEngine::getCallingPlugin()) {
            auto func = importAnyFunc(pluginName, funcName);
            return py::cpp_function([func](py::args args) -> py::object {
                std::any result;
                try {
                    std::vector<std::any> cxx_args;
                    for (auto& arg : args) {
                        cxx_args.push_back(std::any(py::cast<ScriptType>(arg)));
                    }
                    result = func(cxx_args);
                } catch (std::exception const& e) {
                    try {
                        std::vector<std::any> cxx_args;
                        for (auto& arg : args) {
                            cxx_args.push_back(variant_to_any(py::cast<ScriptType>(arg)));
                        }
                        result = func(cxx_args);
                    } catch (...) {
                        throw std::runtime_error(e.what());
                    }
                }
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