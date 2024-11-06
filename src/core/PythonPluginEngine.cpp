#include "PythonPluginEngine.hpp"


PythonPluginEngine::PythonPluginEngine() : IPluginEngine() {
    Py_Initialize();
    mSystemPath = PySys_GetObject("path");
}

PythonPluginEngine::~PythonPluginEngine() { Py_Finalize(); }

std::string PythonPluginEngine::getPluginType() const { return "script-python"; }

bool PythonPluginEngine::loadPlugin(std::string const& plugin, std::filesystem::path const& entry) {
    if (!mPluginsHandle.contains(plugin)) {
        PyObject* pluginFolder = PyUnicode_FromString(("./plugins/" + plugin).c_str());
        PyList_Append(mSystemPath, pluginFolder);
        Py_DECREF(pluginFolder);
        // 执行entry
        std::string module_name = entry.string();

        // 导入入口模块
        PyObject* pModule          = PyImport_ImportModule(module_name.c_str());
        mPluginsHandle[plugin]     = pModule;
        mPluginsMoudleName[plugin] = module_name;
        if (pModule != NULL) {
            // 调用模块的__main__部分
            PyObject* pFunc = PyObject_GetAttrString(pModule, "__main__");
            if (pFunc && PyCallable_Check(pFunc)) {
                PyObject* pResult = PyObject_CallObject(pFunc, NULL);
                if (pResult != NULL) {
                    Py_DECREF(pResult);
                } else {
                    PyErr_Print();
                }
                Py_DECREF(pFunc);
                return true;
            } else {
                if (PyErr_Occurred()) PyErr_Print();
            }
            Py_DECREF(pModule);
        } else {
            PyErr_Print();
        }
    }
    return false;
}

bool PythonPluginEngine::unloadPlugin(std::string const& plugin) {
    if (mPluginsHandle.contains(plugin)) {
        PyGILState_STATE gil_state = PyGILState_Ensure(); // 确保Python GIL被持有
        auto             pModule   = mPluginsHandle[plugin];
        if (pModule != NULL) {
            // 从sys.modules中移除模块
            PyObject* pModules = PyImport_GetModuleDict();                      // 获取sys.modules字典
            PyDict_DelItemString(pModules, mPluginsMoudleName[plugin].c_str()); // 删除模块条目

            Py_DECREF(pModule); // 减少模块引用计数
            return true;
        }
        PyGILState_Release(gil_state); // 释放Python GIL
    }
    return false;
}