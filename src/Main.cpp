#include "Version.hpp"
#include "api/plugin/PluginEngineRegistry.hpp"
#include "core/PythonPluginEngine.hpp"

// 此处写插件加载时执行的操作
void onEnable() {
    // 注册Python引擎
    PluginEngineRegistry::registerPluginEngine<PythonPluginEngine>();
}

// 此处写插件卸载载时执行的操作
void onDisable() {
    // 卸载插件需要释放全部资源
    // 注册的监听器和命令可以不显式清理，会自动销毁
}