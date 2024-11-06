#include "PythonPluginEngine.hpp"
#include "api/utils/StringUtils.hpp"

PythonPluginEngine::PythonPluginEngine() : IPluginEngine() {
    Py_Initialize();
    mSystemPath = PySys_GetObject("path");
}

PythonPluginEngine::~PythonPluginEngine() { Py_Finalize(); }

std::string PythonPluginEngine::getPluginType() const { return "script-python"; }

bool PythonPluginEngine::loadPlugin(std::string const& plugin, std::filesystem::path const& entry) {
    if (!mPluginsHandle.contains(plugin)) {
        std::string path = "import sys; sys.path.append('{PluginPath}')";
        FILE*       file = fopen(entry.string().c_str(), "r");
        if (file != NULL) {
            int ret = PyRun_SimpleFile(file, entry.string().c_str());
            fclose(file);
            if (ret == 0) {
                getLogger().info("已成功加载Python插件 {}", plugin);
                return true;
            } else {
                getLogger().error("加载Python插件 {} 时发生异常！", plugin);
                PyErr_Print();
            }
        } else {
            getLogger().error("无法打开文件 {}！", entry.string());
        }
    }
    return false;
}

bool PythonPluginEngine::unloadPlugin(std::string const& plugin) {
    // TODO
    return true;
}

Logger& PythonPluginEngine::getLogger() { return mLogger; }