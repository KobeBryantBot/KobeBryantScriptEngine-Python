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
        )
        .def("record", &Message::record)
        .def("video", &Message::video)
        .def("rps", &Message::rps)
        .def("dice", &Message::dice)
        .def("shake", &Message::shake)
        .def("contact", &Message::contact)
        .def("json", &Message::json);

    py::enum_<Message::ImageType>(MessageClass, "ImageType")
        .value("Path", Message::ImageType::Path)
        .value("Binary", Message::ImageType::Binary)
        .value("Url", Message::ImageType::Url)
        .export_values();

    py::enum_<RequestSubType>(m, "RequestSubType")
        .value("Add", RequestSubType::Add)
        .value("Invite", RequestSubType::Invite)
        .export_values();

    py::class_<PacketSender>(m, "PacketSender")
        .def_static(
            "getInstance",
            [] { return std::unique_ptr<PacketSender, py::nodelete>(&PacketSender::getInstance()); }
        )
        .def("sendRawPacket", py::overload_cast<std::string const&>(&PacketSender::sendRawPacket))
        .def("sendGroupMessage", py::overload_cast<uint64_t, std::string const&>(&PacketSender::sendGroupMessage))
        .def("sendGroupMessage", py::overload_cast<uint64_t, Message const&>(&PacketSender::sendGroupMessage))
        .def(
            "sendRawPacket",
            [](PacketSender&                           self,
               std::string const&                      rawPacket,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.sendRawPacket(
                    rawPacket,
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def(
            "sendPrivateMessage",
            py::overload_cast<PacketSender::UserId, std::string const&>(&PacketSender::sendPrivateMessage)
        )
        .def(
            "sendPrivateMessage",
            py::overload_cast<PacketSender::UserId, Message const&>(&PacketSender::sendPrivateMessage)
        )
        .def("sendFriendPoke", &PacketSender::sendFriendPoke)
        .def("sendGroupPoke", &PacketSender::sendGroupPoke)
        .def("deleteMessage", &PacketSender::deleteMessage)
        .def("sendLike", &PacketSender::sendLike)
        .def("kickGroupMember", &PacketSender::kickGroupMember)
        .def("setGroupMemberMute", &PacketSender::setGroupMemberMute)
        .def("setGroupGlobalMute", &PacketSender::setGroupGlobalMute)
        .def("setGroupAdmin", &PacketSender::setGroupAdmin)
        .def("setGroupCard", &PacketSender::setGroupCard)
        .def("setGroupName", &PacketSender::setGroupName)
        .def("leaveGroup", &PacketSender::leaveGroup)
        .def("handleFriendAddRequest", &PacketSender::handleFriendAddRequest)
        .def("handleGroupAddRequest", &PacketSender::handleGroupAddRequest)
        .def(
            "getMessage",
            [](PacketSender&                           self,
               int64_t                                 messageId,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getMessage(
                    messageId,
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def(
            "getGroupsListInfo",
            [](PacketSender&                           self,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getGroupsListInfo(
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def(
            "getForwardMessage",
            [](PacketSender&                           self,
               std::string                             messageId,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getForwardMessage(
                    messageId,
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def(
            "getLoginInfo",
            [](PacketSender&                           self,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getLoginInfo(
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def(
            "getStrangerInfo",
            [](PacketSender&                           self,
               PacketSender::UserId                    target,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getStrangerInfo(
                    target,
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def(
            "getFriendsListInfo",
            [](PacketSender&                           self,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getFriendsListInfo(
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def("getFriendsList", &PacketSender::getFriendsList)
        .def(
            "getGroupInfo",
            [](PacketSender&                           self,
               PacketSender::GroupId                   groupId,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getGroupInfo(
                    groupId,
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def(
            "getGroupMemberInfo",
            [](PacketSender&                           self,
               PacketSender::GroupId                   groupId,
               PacketSender::UserId                    target,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getGroupMemberInfo(
                    groupId,
                    target,
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def(
            "getGroupMembersListInfo",
            [](PacketSender&                           self,
               PacketSender::GroupId                   groupId,
               std::function<void(std::string const&)> callback,
               std::function<void()>                   timeoutCallback,
               uint64_t                                seconds) {
                return self.getGroupMembersListInfo(
                    groupId,
                    [=](nlohmann::json_abi_v3_11_3::json const& data) {
                        if (callback) {
                            callback(data.dump());
                        }
                    },
                    std::move(timeoutCallback),
                    seconds
                );
            }
        )
        .def("getGroupMembersList", &PacketSender::getGroupMembersList)
        .def("chooseRandomGroupMember", &PacketSender::chooseRandomGroupMember)
        .def("getGroupsList", &PacketSender::getGroupsList);
}
