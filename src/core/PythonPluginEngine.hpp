#include "Python/Python.h"
#include "api/plugin/IPluginEngine.hpp"
#include <unordered_map>

class PythonPluginEngine : public IPluginEngine {
private:
    PyObject*                                    mSystemPath;
    std::unordered_map<std::string, PyObject*>   mPluginsHandle;
    std::unordered_map<std::string, std::string> mPluginsMoudleName;

public:
    PythonPluginEngine();

    ~PythonPluginEngine() override;

    std::string getPluginType() const override;

    bool loadPlugin(std::string const& plugin, std::filesystem::path const& entry) override;

    bool unloadPlugin(std::string const& plugin) override;
};
