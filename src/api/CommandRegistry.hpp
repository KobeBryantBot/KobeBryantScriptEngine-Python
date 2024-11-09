#pragma once
#include "api/CommandRegistry.hpp"
#include <map>
#include <set>

class ScriptCommandManager {
    std::map<std::string, std::set<std::string>> mPluginCommands;

public:
    static ScriptCommandManager& getInstance();

    void addPluginCommand(std::string const& plugin, std::string const& cmd);

    void removePluginCommand(std::string const& plugin, std::string const& cmd);

    void removePluginCommands(std::string const& plugin);
};
