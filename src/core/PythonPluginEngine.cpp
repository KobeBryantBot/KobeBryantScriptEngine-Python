#include "PythonPluginEngine.hpp"
#include "api/utils/FileUtils.hpp"
#include "api/utils/StringUtils.hpp"

// 必须释放全局 GIL 锁
static std::unique_ptr<py::gil_scoped_release> release = nullptr;

PythonPluginEngine::PythonPluginEngine() : IPluginEngine() {
    if (!release) {
        release = std::make_unique<py::gil_scoped_release>();
    }
};

PythonPluginEngine::~PythonPluginEngine() {
    release.reset();
    release = nullptr;
}

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
        py::object pluginModule = py::module::import(moduleName.c_str());
        pluginModule.attr("on_enable")();
        // 缓存此模块
        mPluginModules["Test"] = std::move(pluginModule);
        sys_path.attr("remove")(py::str(entry.parent_path().string()));
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
    // TODO: 完成卸载
    getLogger().warn("卸载Python插件的功能尚未完成！");
    return false;
}

Logger& PythonPluginEngine::getLogger() { return mLogger; }