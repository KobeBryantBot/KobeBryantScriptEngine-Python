#include "Python/Python.h"
#include "api/Logger.hpp"
#include "api/plugin/IPluginEngine.hpp"
#include <unordered_map>

class PythonPluginEngine : public IPluginEngine {
private:
    Logger mLogger{"KobeBryantScriptEngine-Python"};

public:
    PythonPluginEngine();

    ~PythonPluginEngine() override;

    std::string getPluginType() const override;

    bool loadPlugin(std::string const& plugin, std::filesystem::path const& entry) override;

    bool unloadPlugin(std::string const& plugin) override;

    Logger& getLogger();
};
