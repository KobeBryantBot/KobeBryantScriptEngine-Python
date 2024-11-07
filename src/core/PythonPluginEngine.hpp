#include "api/Logger.hpp"
#include "api/plugin/IPluginEngine.hpp"
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <unordered_map>

namespace py = pybind11;

class PythonPluginEngine : public IPluginEngine {
private:
    Logger                                       mLogger{"KobeBryantScriptEngine-Python"};
    py::scoped_interpreter                       mGILGuard{};
    std::unordered_map<std::string, std::string> mPluginsMap;

public:
    PythonPluginEngine();

    ~PythonPluginEngine() override;

    std::string getPluginType() const override;

    bool loadPlugin(std::string const& plugin, std::filesystem::path const& entry) override;

    bool unloadPlugin(std::string const& plugin) override;

    Logger& getLogger();
};
