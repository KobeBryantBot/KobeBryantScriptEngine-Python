#include "PythonPluginEngine.hpp"
#include "api/utils/FileUtils.hpp"
#include "api/utils/StringUtils.hpp"

PythonPluginEngine::PythonPluginEngine() : IPluginEngine() { Logger().warn("construct"); }

PythonPluginEngine::~PythonPluginEngine() { Logger().warn("destruct"); }

std::string PythonPluginEngine::getPluginType() const { return "script-python"; }

bool PythonPluginEngine::loadPlugin(std::string const& plugin, std::filesystem::path const& entry) {
    try {
        if (auto data = utils::readFile(entry)) {
            getLogger().info("正在加载Python插件 {}", plugin);
            // 安装pip包
            std::string requirePath = "./plugins/" + plugin + "/requirements.txt";
            if (std::filesystem::exists(requirePath)) {
                getLogger().info("正在加载Python插件 {} 的pip依赖包...", plugin);
                system(("pip install -r " + requirePath + " >NUL 2>&1").c_str());
            }
            // 注册模块路径到 sys.path
            py::module sys      = py::module::import("sys");
            py::object sys_path = sys.attr("path");
            sys_path.attr("append")(py::str(entry.parent_path().string()));
            // 执行Python代码
            py::exec(*data, mGlobalNamespace);
            sys_path.attr("remove")(py::str(entry.parent_path().string()));
            getLogger().info("已成功加载Python插件 {}", plugin);
            return true;
        } else {
            getLogger().error("无法打开文件 {}！", entry.string());
        }
    } catch (const std::exception& e) {
        getLogger().error("加载Python插件 {} 时捕获到异常:\n {}", plugin, e.what());
    } catch (...) {
        getLogger().error("加载Python插件 {} 时出现未知异常", plugin);
    }
    return false;
}

bool PythonPluginEngine::unloadPlugin(std::string const& plugin) {
    // TODO
    return true;
}

Logger& PythonPluginEngine::getLogger() { return mLogger; }