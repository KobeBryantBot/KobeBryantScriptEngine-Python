#include "api/Logger.hpp"
#include "api/plugin/IPluginEngine.hpp"
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <thread>
#include <unordered_map>

namespace py = pybind11;

class PythonPluginEngine : public IPluginEngine {
private:
    Logger                                      mLogger{"KobeBryantScriptEngine-Python"};
    py::scoped_interpreter                      mGuard{};
    std::unordered_map<std::string, py::object> mPluginModules;

public:
    PythonPluginEngine();

    ~PythonPluginEngine() override;

    std::string getPluginType() const override;

    bool loadPlugin(std::string const& plugin, std::filesystem::path const& entry) override;

    bool unloadPlugin(std::string const& plugin) override;

    Logger& getLogger();
};
