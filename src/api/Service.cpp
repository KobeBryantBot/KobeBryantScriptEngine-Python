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
    static std::any toAny(const pybind11::handle& obj) {
        if (py::isinstance<py::int_>(obj)) {
            return std::any(static_cast<uint64_t>(obj.cast<uint64_t>()));
        } else if (py::isinstance<py::float_>(obj)) {
            return std::any(obj.cast<double>());
        } else if (py::isinstance<py::str>(obj)) {
            return std::any(obj.cast<std::string>());
        } else if (py::isinstance<py::bool_>(obj)) {
            return std::any(obj.cast<bool>());
        } else if (py::isinstance<py::tuple>(obj)) {
            return std::any(obj.cast<std::tuple<>>());
        } else if (py::isinstance<py::list>(obj)) {
            return std::any(obj.cast<std::vector<py::object>>());
        } else if (py::isinstance<py::dict>(obj)) {
            return std::any(obj.cast<std::unordered_map<std::string, py::object>>());
        } else if (py::isinstance<py::bytes>(obj)) {
            return std::any(obj.cast<std::string>());
        } else if (obj.is_none()) {
            return std::any();
        } else {
            throw std::runtime_error("Unsupported argument type");
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
                    args_tuple[i] = py::cast(args[i]);
                }
                py::object result = py_func(*args_tuple);
                return toAny(result);
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
                    cxx_args.push_back(toAny(arg));
                }
                std::any result = func(cxx_args);
                if (result.type() == typeid(void)) {
                    return py::none();
                }
                return py::cast(result);
            });
        }
        return nullptr;
    }
};

int Test(double a) {
    Logger().warn("call {}", a);
    return false;
}

void initService(py::module_& m) {
    Service::exportFunc<int, double>("test", &Test);
    py::class_<ScriptService>(m, "Service")
        .def_static("hasFunc", &ScriptService::hasFunc)
        .def_static("exportFunc", &ScriptService::exportFunc)
        .def_static("importFunc", &ScriptService::importFunc)
        .def_static("removeFunc", &ScriptService::removeFunc);
}