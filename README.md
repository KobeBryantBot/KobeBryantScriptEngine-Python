# KobeBryant 脚本引擎 - Python

[![Latest Tag](https://img.shields.io/github/v/tag/KobeBryantBot/KobeBryantScriptEngine-Python?label=最新版本&style=for-the-badge)](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/releases)
[![Downloads](https://img.shields.io/github/downloads-pre/KobeBryantBot/KobeBryantScriptEngine-Python/latest/total?style=for-the-badge&logoColor=41a3ed&label=DOWNLOADS&color=2fffdc)
](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/releases)    
![C++](https://img.shields.io/badge/C++-23-blue?logo=C%2B%2B&logoColor=41a3ed&style=for-the-badge)
![Python](https://img.shields.io/badge/python-3.13-blue?logo=python&logoColor=edb641&style=for-the-badge)  
[![Contributors](https://img.shields.io/github/contributors/KobeBryantBot/KobeBryantScriptEngine-Python.svg?style=for-the-badge)](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/graphs/contributors)
[![Stars](https://img.shields.io/github/stars/KobeBryantBot/KobeBryantScriptEngine-Python.svg?style=for-the-badge)](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/stargazers)
[![Forks](https://img.shields.io/github/forks/KobeBryantBot/KobeBryantScriptEngine-Python.svg?style=for-the-badge)](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/network/members)
[![Issues](https://img.shields.io/github/issues/KobeBryantBot/KobeBryantScriptEngine-Python.svg?style=for-the-badge)](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/issues)
[![Pull Requests](https://img.shields.io/github/issues-pr/KobeBryantBot/KobeBryantScriptEngine-Python?style=for-the-badge)](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/issues)   
[![License](https://img.shields.io/github/license/KobeBryantBot/KobeBryantScriptEngine-Python.svg?style=for-the-badge)](LICENSE)
  
[![982714789](https://img.shields.io/badge/QQ交流群%20982714789-pink?style=for-the-badge&logo=tencent%20qq)](https://qm.qq.com/q/78bKZ18A9O)

觉得 `C++` 太难？学习成本太高？不会编写 `KobeBryant` 框架的原生插件？  
KobeBryant框架没有要求插件必须使用 `C++` 编写。  

通过本插件（引擎），现在你可以在 `KobeBryant` 框架上面编写并运行Python插件


# 使用方法

- 下载最新版，并解压，将整个文件夹放置到plugins文件夹中。确保放置目录结构如下所示。
```text
├── KobeBryant.exe
└── plugins
    └── KobeBryantScriptEngine-Python
        ├── Python313（文件夹）
        ├── KobeBryantScriptEngine-Python.dll
        └── manifest.json
```
- 添加你的Python插件
- 重启你的 `KobeBryant` 机器人。
- 现在你可以使用Python插件了。

> 你不需要在你的电脑或者服务器上面安装 `Python 3.13`，Python运行所需的依赖都在此引擎（插件）的文件夹里面，该引擎会使用文件夹里面的Python而非系统Python。

# 构建项目
如果你愿意自己构建项目，或者向 `KobeBryantScriptEngine-Python` 贡献代码，您可以按照以下说明自行构建项目

- 克隆仓库到本地，并使用你熟悉的 IDE （VS / VSCode / CLion等）打开项目目录，对 Cmake 项目进行编译生成。
- 正常构建该项目即可。 

# Python插件介绍

> KobeBryant本体只能加载原生插件  
> KobeBryant在读取到非原生插件（`manifest.json`里面的`"type"`字段不是`"native"`时），会寻找已注册的插件引擎，并交由对应的插件引擎加载  
> 本引擎注册的`"type"`为`"script-python"`

## 插件结构

和原生插件一样，每一个Python插件应该是一个文件夹，里面包含了插件运行需要的内容，和一个插件清单 `manifest.json`  
如果你的插件需要使用pip包，请在插件文件夹内放置一个`requirements.txt`，引擎会自动安装需要的pip包  
插件结构应该如下所示

```text
├── ...
├── KobeBryant.exe
└── plugins
    └── PluginName（插件所在文件夹）
        ├── main.py（插件入口文件）
        ├── manifest.json
        ├── requirements.txt（可选，如果需要）
        └── ...（可选）
``` 

你必须保持 `manifest.json` 里面的 `name` 字段和插件文件夹名称一致！

## 插件清单

```json
// manifest.json
{
    "name": "PluginName",       // 必需：插件名，必须和文件夹名称一致
    "entry": "PluginEntry.py",  // 必需：插件入口文件
    "type": "script-python",    // 必需：需要通过Python引擎加载的Python插件必须填"script-python"
    "preload": [],              // 可选：需要预加载的DLL
    "passive": false,           // 可选：如果插件不被其它插件依赖，是否跳过加载（一般纯前置插件才填写true）
    "dependence": [],           // 可选：插件依赖项（硬依赖）
    "optional_dependence": [],  // 可选：插件依赖项（可选依赖）
}
```

# Python插件开发
> KobeBryantScriptEngine-Python里面使用的Python版本是 `cpython-3.13`，请确保你编写的插件符合 `cpython-3.13` 的功能。  

## 开发流程
- 这里推荐使用 `Visual Studio Code`，并安装扩展 `Python` 和 `Pylance` 扩展
- 克隆我们的 [插件模板](https://github.com/KobeBryantBot/Python-Plugin-Template) ，向项目里面添加我们提供的 `KobeBryantAPI.pyi` 用于提供自动补全（模板里面已经自带）
- 如果需要，你可以使用我们提供的 `KobeBryantEvent.py` 模块，更方便的监听事件。
- 开始编写你的插件
- 测试、发布插件

## 代码示例
```Python
# main.py
from KobeBryantAPI import Logger  # type: ignore

logger = Logger()

# 这里写插件加载时需要执行的操作
def on_enable():
    logger.info("Python插件模板已加载")


# 这里写插件加载时需要执行的操作
def on_disable():
    # 卸载插件时，你需要释放插件的所有资源
    # 你需要在这里执行清理全部后台任务，结束全部线程等操作
    # 其中监听的事件、注册的命令可以不手动清理，系统会自动清理
    # 仅使用 KobeBryant 提供的 ScheduleAPI 添加的定时任务可以不手动清理，系统会自动清理
    logger.info("Python插件模板已卸载")
```

# 开源许可

版权所有 © 2024 KobeBryantBot, 保留所有权利.

本项目采用 LGPL-3.0 许可证发行 - 阅读 [LICENSE](LICENSE) 文件获取更多信息。   

- **开发者不对您负责，开发者没有义务为你编写代码、为你使用造成的任何后果负责**
1. 本插件（引擎）全部代码采用 `LGPL 3.0` 开源协议。
2. 您可以使用任何开源许可证编写基于 `KobeBryantScriptEngine-Python` 运行的Python插件，甚至不发布您的源代码。
3. 如果你修改了 `KobeBryantScriptEngine-Python` 的源代码，或者基于 `KobeBryantScriptEngine-Python` 编写了一个新的插件（引擎），你必须开源它。
4. 如果你想要分发，转载本插件（引擎），你必须得到我们的授权！

## 参与贡献

欢迎参与贡献！  

- 你可以通过 [反馈问题](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/issues/new/choose) 帮助我们进行改进  
- 如果你有能力帮助我们贡献代码，你也可以 Fork 本仓库，并 [提交 Pull Requests](https://github.com/KobeBryantBot/KobeBryantScriptEngine-Python/compare)

## 贡献者

这个项目之所以存在，离不开本项目所有的贡献者。

![贡献者](https://contrib.rocks/image?repo=KobeBryantBot/KobeBryantScriptEngine-Python)