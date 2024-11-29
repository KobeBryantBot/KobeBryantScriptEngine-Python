#include "api/PacketSender.hpp"
#include <pybind11/chrono.h>
#include <pybind11/complex.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>
#include <pybind11_json/pybind11_json.hpp>

namespace py = pybind11;

void initPacket(py::module_& m) {
    py::class_<Message> MessageClass(m, "Message");

    py::enum_<Message::ImageType>(MessageClass, "ImageType")
        .value("Path", Message::ImageType::Path)
        .value("Binary", Message::ImageType::Binary)
        .value("Url", Message::ImageType::Url)
        .value("Base64", Message::ImageType::Base64)
        .export_values();

    MessageClass.def(py::init<>())
        .def("at", &Message::at)
        .def("reply", &Message::reply)
        .def("face", &Message::face)
        .def("text", (Message & (Message::*)(const std::string&)) & Message::text)
        .def(
            "image",
            py::overload_cast<std::string const&, Message::ImageType, std::optional<std::string>>(&Message::image),
            py::arg(),
            py::arg() = Message::ImageType::Path,
            py::arg() = std::nullopt
        )
        .def(
            "avatar",
            py::overload_cast<int64_t, uint16_t, bool>(&Message::avatar),
            py::arg(),
            py::arg() = 640,
            py::arg() = false
        )
        .def("avatar", py::overload_cast<int64_t, bool>(&Message::avatar))
        .def("record", &Message::record)
        .def("video", &Message::video)
        .def("rps", &Message::rps)
        .def("dice", &Message::dice)
        .def("shake", &Message::shake)
        .def("contact", &Message::contact)
        .def("json", &Message::json);

    py::enum_<RequestSubType>(m, "RequestSubType")
        .value("Add", RequestSubType::Add)
        .value("Invite", RequestSubType::Invite)
        .export_values();

    py::class_<PacketSender>(m, "PacketSender")
        .def_static("sendRawPacket", py::overload_cast<std::string const&>(&PacketSender::sendRawPacket))
        .def_static(
            "sendGroupMessage",
            py::overload_cast<uint64_t, std::string const&>(&PacketSender::sendGroupMessage)
        )
        .def_static("sendGroupMessage", py::overload_cast<uint64_t, Message const&>(&PacketSender::sendGroupMessage))
        .def_static(
            "sendRawPacket",
            py::overload_cast<
                std::string const&,
                std::function<void(nlohmann::json const&)>,
                std::function<void()>,
                uint64_t>(&PacketSender::sendRawPacket),
            py::arg(),
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static(
            "sendPrivateMessage",
            py::overload_cast<PacketSender::UserId, std::string const&>(&PacketSender::sendPrivateMessage)
        )
        .def_static(
            "sendPrivateMessage",
            py::overload_cast<PacketSender::UserId, Message const&>(&PacketSender::sendPrivateMessage)
        )
        .def_static("sendFriendPoke", &PacketSender::sendFriendPoke)
        .def_static("sendGroupPoke", &PacketSender::sendGroupPoke)
        .def_static("deleteMessage", &PacketSender::deleteMessage)
        .def_static("sendLike", &PacketSender::sendLike, py::arg(), py::arg() = 10)
        .def_static("kickGroupMember", &PacketSender::kickGroupMember, py::arg(), py::arg(), py::arg() = false)
        .def_static("setGroupMemberMute", &PacketSender::setGroupMemberMute, py::arg(), py::arg(), py::arg() = 1800)
        .def_static("setGroupGlobalMute", &PacketSender::setGroupGlobalMute, py::arg(), py::arg() = true)
        .def_static("setGroupAdmin", &PacketSender::setGroupAdmin, py::arg(), py::arg(), py::arg() = true)
        .def_static("setGroupCard", &PacketSender::setGroupCard)
        .def_static("setGroupName", &PacketSender::setGroupName)
        .def_static("leaveGroup", &PacketSender::leaveGroup, py::arg(), py::arg() = false)
        .def_static(
            "handleFriendAddRequest",
            &PacketSender::handleFriendAddRequest,
            py::arg(),
            py::arg(),
            py::arg() = ""
        )
        .def_static(
            "handleGroupAddRequest",
            &PacketSender::handleGroupAddRequest,
            py::arg(),
            py::arg(),
            py::arg(),
            py::arg() = ""
        )
        .def_static("getMessage", &PacketSender::getMessage, py::arg(), py::arg(), py::arg() = nullptr, py::arg() = 5)
        .def_static(
            "getGroupsListInfo",
            &PacketSender::getGroupsListInfo,
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static(
            "getForwardMessage",
            &PacketSender::getForwardMessage,
            py::arg(),
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static("getLoginInfo", &PacketSender::getLoginInfo, py::arg(), py::arg() = nullptr, py::arg() = 5)
        .def_static(
            "getStrangerInfo",
            py::overload_cast<uint64_t, std::function<void(nlohmann::json const&)>, std::function<void()>, uint64_t>(
                &PacketSender::getStrangerInfo
            ),
            py::arg(),
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static(
            "getFriendsListInfo",
            &PacketSender::getFriendsListInfo,
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static("getFriendsList", &PacketSender::getFriendsList)
        .def_static(
            "getGroupInfo",
            py::overload_cast<uint64_t, std::function<void(nlohmann::json const&)>, std::function<void()>, uint64_t>(
                &PacketSender::getGroupInfo
            ),
            py::arg(),
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static(
            "getGroupMemberInfo",
            py::overload_cast<
                uint64_t,
                uint64_t,
                std::function<void(nlohmann::json const&)>,
                std::function<void()>,
                uint64_t>(&PacketSender::getGroupMemberInfo),
            py::arg(),
            py::arg(),
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static(
            "getGroupMembersListInfo",
            &PacketSender::getGroupMembersListInfo,
            py::arg(),
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static(
            "getGroupMembersList",
            &PacketSender::getGroupMembersList,
            py::arg(),
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static(
            "chooseRandomGroupMember",
            &PacketSender::chooseRandomGroupMember,
            py::arg(),
            py::arg(),
            py::arg() = nullptr,
            py::arg() = 5
        )
        .def_static("getGroupsList", &PacketSender::getGroupsList, py::arg(), py::arg() = nullptr, py::arg() = 5);
}
