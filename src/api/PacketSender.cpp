#include "api/PacketSender.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_EMBEDDED_MODULE(PacketSenderAPI, m) {
    py::class_<Message> MessageClass(m, "Message");
    MessageClass.def(py::init<>())
        .def("at", &Message::at)
        .def("reply", &Message::reply)
        .def("face", &Message::face)
        .def("text", (Message & (Message::*)(const std::string&)) & Message::text)
        .def(
            "image",
            py::overload_cast<std::string const&, Message::ImageType, std::optional<std::string>>(&Message::image)
        );

    py::enum_<Message::ImageType>(MessageClass, "ImageType")
        .value("Path", Message::ImageType::Path)
        .value("Binary", Message::ImageType::Binary)
        .value("Url", Message::ImageType::Url)
        .export_values();

    py::class_<PacketSender>(m, "PacketSender")
        .def_static(
            "getInstance",
            [] { return std::unique_ptr<PacketSender, py::nodelete>(&PacketSender::getInstance()); }
        )
        .def("sendRawPacket", py::overload_cast<std::string const&>(&PacketSender::sendRawPacket))
        .def("sendGroupMessage", py::overload_cast<uint64_t, std::string const&>(&PacketSender::sendGroupMessage))
        .def("sendGroupMessage", py::overload_cast<uint64_t, Message const&>(&PacketSender::sendGroupMessage));
}
