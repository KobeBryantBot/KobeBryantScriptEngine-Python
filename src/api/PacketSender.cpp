#include "api/PacketSender.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(PacketSenderAPI, m) {
    py::class_<PacketSender>(m, "PacketSender")
        .def_static(
            "getInstance",
            [] { return std::unique_ptr<PacketSender, py::nodelete>(&PacketSender::getInstance()); }
        )
        .def("sendRawPacket", py::overload_cast<std::string const&>(&PacketSender::sendRawPacket))
        .def("sendGroupMessage", py::overload_cast<uint64_t, std::string const&>(&PacketSender::sendGroupMessage));
}
