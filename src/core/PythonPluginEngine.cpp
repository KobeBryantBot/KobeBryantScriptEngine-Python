#include "PythonPluginEngine.hpp"
#include "api/utils/StringUtils.hpp"

PythonPluginEngine::PythonPluginEngine() : IPluginEngine() {}

PythonPluginEngine::~PythonPluginEngine() {}

std::string PythonPluginEngine::getPluginType() const { return "script-python"; }

bool PythonPluginEngine::loadPlugin(std::string const& plugin, std::filesystem::path const& entry) {
    try {
        getLogger().info("正在加载Python插件 {}", plugin);
        Py_Initialize();
        // 设置路径
        std::string path = "import sys; sys.path.append('{PluginPath}')";
        utils::ReplaceStr(path, "{PluginPath}", entry.parent_path().string());
        PyRun_SimpleString(path.c_str());
        // 安装pip包
        std::string requirePath = "./plugins/" + plugin + "/requirements.txt";
        if (std::filesystem::exists(requirePath)) {
            getLogger().info("正在加载Python插件 {} 的pip依赖包...", plugin);
            system(("pip install -r " + requirePath + " >NUL 2>&1").c_str());
        }
        // 加载文件
        FILE* file = fopen(entry.string().c_str(), "r");
        if (file != NULL) {
            int ret = PyRun_SimpleFile(file, entry.string().c_str());
            fclose(file);
            if (ret == 0) {
                getLogger().info("已成功加载Python插件 {}", plugin);
                
                Py_Finalize();
                return true;
            } else {
                getLogger().error("加载Python插件 {} 时发生异常！", plugin);
                PyErr_Print();
            }
        } else {
            getLogger().error("无法打开文件 {}！", entry.string());
        }
    } catch (const std::exception& e) {
        getLogger().error("加载Python插件 {} 时捕获到异常 {}", plugin, e.what());
    } catch (...) {
        getLogger().error("加载Python插件 {} 时出现未知异常", plugin);
    }
    Py_Finalize();
    return false;
}

bool PythonPluginEngine::unloadPlugin(std::string const& plugin) {
    // TODO
    return true;
}

Logger& PythonPluginEngine::getLogger() { return mLogger; }