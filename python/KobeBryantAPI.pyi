from enum import Enum
from typing import Optional, List, Union, Callable, Dict, overload, override, Any

class CommandRegistry:
    @staticmethod
    def registerSimpleCommand(
        cmd: str, callback: Callable[[List[str]], None]
    ) -> bool: ...
    @staticmethod
    def unregisterCommand(cmd: str) -> bool: ...
    @staticmethod
    def executeCommand(cmd: str) -> None: ...

class Listener:
    mId: int
    mType: str

class Event:
    mEventData: Dict[str, Any]
    @staticmethod
    def newEvent(name: str, data: Dict[str, Any]) -> Event: ...
    def block_pass(self) -> None: ...

class EventBus:
    @staticmethod
    def add(
        event: str, callback: Callable[[Event], None], priority: int = 500
    ) -> Listener: ...
    @staticmethod
    def remove(listener: Listener) -> bool: ...
    @staticmethod
    def emit(event: str, data: Event) -> Listener: ...

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
    def setTitle(self, title: str) -> None: ...
    def setLevel(self, level: LogLevel) -> None: ...
    def setFile(self, path: str) -> bool: ...
    def log(self, level: LogLevel, output: str) -> None: ...
    def fatal(self, output: str) -> None: ...
    def error(self, output: str) -> None: ...
    def warn(self, output: str) -> None: ...
    def info(self, output: str) -> None: ...
    def debug(self, output: str) -> None: ...

class Schedule:
    @staticmethod
    def addDelayTask(delay: int, task: Callable[[], None]) -> int: ...
    @overload
    @staticmethod
    def addRepeatTask(
        interval: int, task: Callable[[], None], immediately: bool = False
    ) -> int: ...
    @overload
    @staticmethod
    def addRepeatTask(
        interval: int, task: Callable[[], None], immediately: bool, times: int
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
    @override
    def sendRawPacket(packet: str) -> None: ...
    @staticmethod
    @override
    def sendRawPacket(
        packet: str,
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    @overload
    def sendGroupMessage(target: int, msg: str) -> None: ...
    @staticmethod
    @overload
    def sendGroupMessage(target: int, msg: Message) -> None: ...
    @staticmethod
    @overload
    def sendPrivateMessage(target: int, msg: str) -> None: ...
    @staticmethod
    @overload
    def sendPrivateMessage(target: int, msg: Message) -> None: ...
    @staticmethod
    def sendFriendPoke(target: int) -> None: ...
    @staticmethod
    def sendGroupPoke(group: int, target: int) -> None: ...
    @staticmethod
    def deleteMessage(msgId: int) -> None: ...
    @staticmethod
    def sendLike(target: int, times: int = 10) -> None: ...
    @staticmethod
    def kickGroupMember(group: int, target: int, reject: bool = False) -> None: ...
    @staticmethod
    def setGroupMemberMute(group: int, target: int, duration: int = 1800) -> None: ...
    @staticmethod
    def setGroupGlobalMute(group: int, enable: bool = True) -> None: ...
    @staticmethod
    def setGroupAdmin(group: int, target: int, enable: bool = True) -> None: ...
    @staticmethod
    def setGroupCard(group: int, target: int, card: str) -> None: ...
    @staticmethod
    def setGroupName(group: int, name: str) -> None: ...
    @staticmethod
    def leaveGroup(group: int, dismiss: bool = False) -> None: ...
    @staticmethod
    def handleFriendAddRequest(approve: bool, flag: str, remark: str = "") -> None: ...
    @staticmethod
    def handleGroupAddRequest(
        approve: bool, type: RequestSubType, flag: str, reason: str = ""
    ) -> None: ...
    @staticmethod
    def getMessage(messageId: int) -> None: ...
    @staticmethod
    def getGroupsListInfo(
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getForwardMessage(
        messageId: int,
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getLoginInfo(
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getStrangerInfo(
        target: int,
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getFriendsListInfo(
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getFriendsList(
        callback: Callable[[List[int]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getGroupInfo(
        groupId: int,
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getGroupMemberInfo(
        groupId: int,
        target: int,
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getGroupMembersListInfo(
        groupId: int,
        callback: Callable[[Dict[str, Any]], None],
        timeoutCallback: Callable[[], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getGroupMembersList(
        groupId: int,
        callback: Callable[[List[int]], None],
        timeoutCallback: Callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def chooseRandomGroupMember(
        groupId: int,
        callback: Callable[[int], None],
        timeoutCallback: Callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...
    @staticmethod
    def getGroupsList(
        groupId: int,
        callback: Callable[[List[int]], None],
        timeoutCallback: Callable[[str], None] = None,
        timeout: int = 5,
    ) -> None: ...

ServiceFuncType = Union[
    None,
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
    def exportFunc(funcName: str, func: Callable[..., ServiceFuncType]) -> bool: ...
    @staticmethod
    def importFunc(pluginName: str, funcName: str) -> ServiceFuncType: ...
    @staticmethod
    def removeFunc(funcName: str) -> bool: ...
