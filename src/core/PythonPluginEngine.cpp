#include "PythonPluginEngine.hpp"
#include "../resource.hpp"
#include "api/utils/FileUtils.hpp"
#include "api/utils/ModuleUtils.hpp"
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
        if (!mPluginModules.contains(plugin)) {
            // 如果没有导入过这个模块
            py::module pluginModule = py::module::import(moduleName.c_str());
            // 缓存此模块
            mPluginModules[plugin] = std::move(pluginModule);
            mModuleEntrys[plugin]  = entry;
        } else {
            // 如果导入过这个模块
            py::module_ importlib   = py::module_::import("importlib");
            py::object  reload_func = importlib.attr("reload");
            reload_func(mPluginModules[plugin]);
        }
        // 启用插件
        mPluginModules[plugin].attr("on_enable")();
        // 清理模块路径
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
    try {
        getLogger().info("正在卸载Python插件 {}", plugin);
        py::gil_scoped_acquire require{};
        // 禁用插件
        mPluginModules[plugin].attr("on_disable")();
        // 替换为空模块
        auto path = mModuleEntrys[plugin];
        backupEntry(path);
        // 重载模块
        py::module_ importlib   = py::module_::import("importlib");
        py::object  reload_func = importlib.attr("reload");
        reload_func(mPluginModules[plugin]);
        // 还原模块
        resumeEntry(path);
        getLogger().info("已成功卸载Python插件 {}", plugin);
        return true;
    } catch (const std::exception& e) {
        getLogger().error("卸载Python插件 {} 时捕获到异常:\n {}", plugin, e.what());
    } catch (...) {
        getLogger().error("卸载Python插件 {} 时出现未知异常", plugin);
    }
    return false;
}

Logger& PythonPluginEngine::getLogger() { return mLogger; }

void PythonPluginEngine::backupEntry(std::filesystem::path const& entry) {
    auto newPath = entry.string();
    newPath.append(".bak");
    if (std::filesystem::exists(newPath)) {
        std::filesystem::remove(newPath);
    }
    if (std::filesystem::exists(entry)) {
        std::filesystem::copy(entry.string(), newPath);
    }
    auto data = *utils::readCurrentResource(EMPTY_MODULE);
    utils::writeFile(entry, data);
}

void PythonPluginEngine::resumeEntry(std::filesystem::path const& entry) {
    auto newPath = entry.string();
    newPath.append(".bak");
    if (std::filesystem::exists(entry)) {
        std::filesystem::remove(entry);
        std::filesystem::copy_file(newPath, entry);
        std::filesystem::remove(newPath);
    }
}