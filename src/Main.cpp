#include "Version.hpp"
#include "api/Logger.hpp"
#include "api/plugin/PluginEngineRegistry.hpp"
#include "core/PythonPluginEngine.hpp"

extern Logger logger;

// 此处写插件加载时执行的操作
void onEnable() {
    // 注册Python引擎
    PluginEngineRegistry::registerPluginEngine<PythonPluginEngine>();
    logger.info("engine.python.loaded", {ENGINE_VERSION_STRING});
}

// 此处写插件卸载载时执行的操作
void onDisable() { logger.info("engine.python.unloaded"); }