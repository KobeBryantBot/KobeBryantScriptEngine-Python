from functools import wraps
from typing import Callable
from KobeBryantAPI import EventBus, Event  # type: ignore


class KobeBryantEvent:
    class message:
        class group:
            @staticmethod
            def normal(func: Callable[[Event], None]) -> None:
                @wraps(func)
                def wrapper(data: Event) -> None:
                    func(data)

                EventBus.add("message.group.normal", wrapper)
                return wrapper

            @staticmethod
            def anonymous(func: Callable[[Event], None]) -> None:
                @wraps(func)
                def wrapper(data: Event) -> None:
                    func(data)

                EventBus.add("message.group.anonymous", wrapper)
                return wrapper

            @staticmethod
            def notice(func: Callable[[Event], None]) -> None:
                @wraps(func)
                def wrapper(data: Event) -> None:
                    func(data)

                EventBus.add("message.group.notice", wrapper)
                return wrapper

        class private:
            @staticmethod
            def friend(func: Callable[[Event], None]) -> None:
                @wraps(func)
                def wrapper(data: Event) -> None:
                    func(data)

                EventBus.add("message.private.friend", wrapper)
                return wrapper

            @staticmethod
            def notice(func: Callable[[Event], None]) -> None:
                @wraps(func)
                def wrapper(data: Event) -> None:
                    func(data)

                EventBus.add("message.private.notice", wrapper)
                return wrapper

            @staticmethod
            def group(func: Callable[[Event], None]) -> None:
                @wraps(func)
                def wrapper(data: Event) -> None:
                    func(data)

                EventBus.add("message.private.group", wrapper)
                return wrapper

    class meta_event:
        @staticmethod
        def lifecycle(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("meta_event.lifecycle", wrapper)
            return wrapper

        @staticmethod
        def heartbeat(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("meta_event.heartbeat", wrapper)
            return wrapper

    class request:
        @staticmethod
        def friend(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("request.friend", wrapper)
            return wrapper

        @staticmethod
        def group(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("request.group", wrapper)
            return wrapper

    class notice:
        @staticmethod
        def group_upload(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.group_upload", wrapper)
            return wrapper

        @staticmethod
        def group_admin(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.group_admin", wrapper)
            return wrapper

        @staticmethod
        def group_decrease(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.group_decrease", wrapper)
            return wrapper

        @staticmethod
        def group_increase(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.group_increase", wrapper)
            return wrapper

        @staticmethod
        def group_ban(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.group_ban", wrapper)
            return wrapper

        @staticmethod
        def friend_add(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.friend_add", wrapper)
            return wrapper

        @staticmethod
        def group_recall(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.group_recall", wrapper)
            return wrapper

        @staticmethod
        def friend_recall(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.friend_recall", wrapper)
            return wrapper

        @staticmethod
        def group_card(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.group_card", wrapper)
            return wrapper

        @staticmethod
        def offline_file(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.offline_file", wrapper)
            return wrapper

        @staticmethod
        def client_status(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.client_status", wrapper)
            return wrapper

        @staticmethod
        def essence(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.essence", wrapper)
            return wrapper

        @staticmethod
        def notify(func: Callable[[Event], None]) -> None:
            @wraps(func)
            def wrapper(data: Event) -> None:
                func(data)

            EventBus.add("notice.notify", wrapper)
            return wrapper
