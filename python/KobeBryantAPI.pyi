from enum import Enum
from typing import Optional, List, Union, Callable, Dict

class CommandRegistry:
    @staticmethod
    def registerCommand(cmd: str, callback: callable[[List[str]], None]) -> bool: ...
    @staticmethod
    def unregisterCommand(cmd: str) -> bool: ...
    @staticmethod
    def executeCommand(cmd: str) -> None: ...

class Listener:
    mId: int
    mType: str

class EventBus:
    staticmethod
    def add(event: str, callback: callable[[str], None]) -> Listener: ...
    staticmethod
    def remove(listener: Listener) -> bool: ...

class Logger:
    class LogLevel(Enum):
        Trace = 0
        Fatal = 1
        Error = 2
        Warn = 3
        Info = 4
        Debug = 5

    @overload
    def __init__(self) -> None: ...
    @overload
    def __init__(self, title: str) -> None: ...
    def setTitle(title: str) -> None: ...
    def setLevel(level: LogLevel) -> None: ...
    def setFile(path: str) -> bool: ...
    def fatal(output: str) -> None: ...
    def error(output: str) -> None: ...
    def warn(output: str) -> None: ...
    def info(output: str) -> None: ...
    def debug(output: str) -> None: ...

class Schedule:
    @staticmethod
    def addDelayTask(delay: int, task: callable[[], None]) -> int: ...
    @overload
    @staticmethod
    def addRepeatTask(
        interval: int, task: callable[[], None], immediately: bool = False
    ) -> int: ...
    @overload
    @staticmethod
    def addRepeatTask(
        interval: int, task: callable[[], None], immediately: bool, times: int
    ) -> int: ...
    @staticmethod
    def cancelTask(taskId: int) -> bool: ...

class Message:
    class ImageType(Enum):
        Path = 0
        Binary = 1
        Url = 2
        Base64 = 3

    def __init__(self) -> None: ...
    def at(self, target: int) -> Message: ...
    def reply(self, messageId: int) -> Message: ...
    def face(self, id: int) -> Message: ...
    def text(self, content: str) -> Message: ...
    def image(
        self, image: str, type: ImageType, summary: Optional[str] = None
    ) -> Message: ...
    @override
    def avatar(self, target: int, size: int, isGroup: bool) -> Message: ...
    @override
    def avatar(self, target: int, isGroup: bool) -> Message: ...
    def record(self, path: str) -> Message: ...
    def video(self, path: str) -> Message: ...
    def rps(self) -> Message: ...
    def dice(self) -> Message: ...
    def shake(self) -> Message: ...
    def contact(self, target: int, isGroup: bool = False) -> Message: ...
    def json(self, data: str) -> Message: ...

class RequestSubType(Enum):
    Add = 0
    Invite = 1

class PacketSender:
    @staticmethod
    def getInstance() -> PacketSender: ...
    @override
    def sendRawPacket(self, packet: str) -> None: ...
    @override
    def sendRawPacket(
        self,
        packet: str,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @overload
    def sendGroupMessage(self, target: int, msg: str) -> None: ...
    @overload
    def sendGroupMessage(self, target: int, msg: Message) -> None: ...
    @overload
    def sendPrivateMessage(self, target: int, msg: str) -> None: ...
    @overload
    def sendPrivateMessage(self, target: int, msg: Message) -> None: ...
    def sendFriendPoke(self, target: int) -> None: ...
    def sendGroupPoke(self, group: int, target: int) -> None: ...
    def deleteMessage(self, msgId: int) -> None: ...
    def sendLike(self, target: int, times: int = 10) -> None: ...
    def kickGroupMember(
        self, group: int, target: int, reject: bool = False
    ) -> None: ...
    def setGroupMemberMute(
        self, group: int, target: int, duration: int = 1800
    ) -> None: ...
    def setGroupGlobalMute(self, group: int, enable: bool = True) -> None: ...
    def setGroupAdmin(self, group: int, target: int, enable: bool = True) -> None: ...
    def setGroupCard(self, group: int, target: int, card: str) -> None: ...
    def setGroupName(self, group: int, name: str) -> None: ...
    def leaveGroup(self, group: int, dismiss: bool = False) -> None: ...
    def handleFriendAddRequest(
        self, approve: bool, flag: str, remark: str = ""
    ) -> None: ...
    def handleGroupAddRequest(
        self, approve: bool, type: RequestSubType, flag: str, reason: str = ""
    ) -> None: ...
    def getMessage(self, messageId: int) -> None: ...
    def getGroupsListInfo(
        self,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getForwardMessage(
        self,
        messageId: int,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getLoginInfo(
        self,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getStrangerInfo(
        self,
        target: int,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getFriendsListInfo(
        self,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getFriendsList(
        self,
        callback: callable[[List[int]], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getGroupInfo(
        self,
        groupId: int,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getGroupMemberInfo(
        self,
        groupId: int,
        target: int,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getGroupMembersListInfo(
        self,
        groupId: int,
        callback: callable[[str], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getGroupMembersList(
        self,
        groupId: int,
        callback: callable[[List[int]], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def chooseRandomGroupMember(
        self,
        groupId: int,
        callback: callable[[int], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    def getGroupsList(
        self,
        groupId: int,
        callback: callable[[List[int]], None],
        timeoutCallback: callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...

ServiceFuncType = Union[
    int,
    bool,
    str,
    float,
    List[Union[int, bool, str, float]],
    Dict[str, Union[int, bool, str, float]],
]

class Service:
    @staticmethod
    def hasFunc(pluginName: str, funcName: str) -> bool: ...
    @staticmethod
    def exportFunc(funcName: str, func: ServiceFuncType) -> bool: ...
    @staticmethod
    def importFunc(pluginName: str, funcName: str) -> ServiceFuncType: ...
    @staticmethod
    def removeFunc(funcName: str) -> bool: ...
