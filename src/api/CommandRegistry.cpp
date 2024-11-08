#include <api/CommandRegistry.hpp>
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(CommandRegistryAPI, m) {
    py::class_<CommandRegistry>(m, "CommandRegistry")
        .def_static(
            "getInstance",
            [] { return std::unique_ptr<CommandRegistry, py::nodelete>(&CommandRegistry::getInstance()); }
        )
        .def(
            "registerCommand",
            py::overload_cast<std::string const&, std::function<void(std::vector<std::string> const&)>>(
                &CommandRegistry::registerCommand
            )
        )
        .def("unregisterCommand", py::overload_cast<std::string const&>(&CommandRegistry::unregisterCommand))
        .def("executeCommand", &CommandRegistry::executeCommand);
}
