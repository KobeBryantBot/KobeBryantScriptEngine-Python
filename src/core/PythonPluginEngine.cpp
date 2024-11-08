#include "PythonPluginEngine.hpp"
#include "api/utils/FileUtils.hpp"
#include "api/utils/StringUtils.hpp"

PythonPluginEngine::PythonPluginEngine() : IPluginEngine() {
    static std::unique_ptr<py::gil_scoped_release> release;
    if (!release) {
        release = std::make_unique<py::gil_scoped_release>();
    }
};

std::string PythonPluginEngine::getPluginType() const { return "script-python"; }

bool PythonPluginEngine::loadPlugin(std::string const& plugin, std::filesystem::path const& entry) {
    try {
        py::gil_scoped_acquire require{};
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
        // 导入模块并启用
        auto plugin_module = py::module::import(moduleName.c_str());
        plugin_module.attr("on_enable")();
        // 缓存此模块
        mPluginModules[plugin] = std::move(plugin_module);
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
    try {
        if (mPluginModules.contains(plugin)) {
            getLogger().warn("尝试卸载");
            mPluginModules[plugin].attr("on_enable")();
            //mPluginModules.erase(plugin);
        }
    } catch (const std::exception& e) {
        getLogger().error("卸载Python插件 {} 时捕获到异常:\n {}", plugin, e.what());
    } catch (...) {
        getLogger().error("卸载Python插件 {} 时出现未知异常", plugin);
    }
    return false;
    /*
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
    */
}

Logger& PythonPluginEngine::getLogger() { return mLogger; }