#include "PythonPluginEngine.hpp"
#include "api/utils/FileUtils.hpp"
#include "api/utils/StringUtils.hpp"

std::string PythonPluginEngine::getPluginType() const { return "script-python"; }

bool PythonPluginEngine::loadPlugin(std::string const& plugin, std::filesystem::path const& entry) {
    try {
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
        // 构建模块名
        auto moduleName = entry.string();
        utils::ReplaceStr(moduleName, ".py", "");
        utils::ReplaceStr(moduleName, "./", "");
        utils::ReplaceStr(moduleName, "/", ".");
        // 储存插件信息
        mPluginsMap[plugin] = moduleName;
        // 导入模块运行
        py::module_ my_module = py::module_::import(moduleName.c_str());
        getLogger().info("已成功加载Python插件 {}", plugin);
        return true;
    } catch (const std::exception& e) {
        getLogger().error("加载Python插件 {} 时捕获到异常:\n {}", plugin, e.what());
    } catch (...) {
        getLogger().error("加载Python插件 {} 时出现未知异常", plugin);
    }
    return false;
}

bool PythonPluginEngine::unloadPlugin(std::string const& plugin) {
    if (mPluginsMap.contains(plugin)) {
        auto        moduleName = mPluginsMap[plugin];
        py::module_ sys        = py::module_::import("sys");
        py::dict    modules    = sys.attr("modules");
        if (modules.contains(moduleName)) {
            modules[moduleName.c_str()] = py::none();
            mPluginsMap.erase(plugin);
            return true;
        }
    }
    return false;
}

Logger& PythonPluginEngine::getLogger() { return mLogger; }