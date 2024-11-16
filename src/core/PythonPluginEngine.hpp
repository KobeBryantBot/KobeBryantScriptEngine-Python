#pragma once
#include "api/Logger.hpp"
#include "api/i18n/LangI18n.hpp"
#include "api/plugin/IPluginEngine.hpp"
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <thread>
#include <unordered_map>

namespace py = pybind11;

class PythonPluginEngine : public IPluginEngine {
private:
    Logger                                                 mLogger;
    std::unique_ptr<i18n::LangI18n>                        mI18n;
    py::scoped_interpreter                                 mGuard{};
    std::unordered_map<std::string, py::module>            mPluginModules;
    std::unordered_map<std::string, std::filesystem::path> mModuleEntrys;

public:
    PythonPluginEngine();

    ~PythonPluginEngine() override;

    std::string getPluginType() const override;

    bool loadPlugin(std::string const& plugin, std::filesystem::path const& entry) override;

    bool unloadPlugin(std::string const& plugin) override;

    Logger& getLogger();

    void backupEntry(std::filesystem::path const& entry);

    void resumeEntry(std::filesystem::path const& entry);

    static std::optional<std::string> getCallingPlugin();
};
