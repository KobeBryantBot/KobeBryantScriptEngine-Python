#pragma once
#include "api/CommandRegistry.hpp"
#include <unordered_map>
#include <unordered_set>

class ScriptCommandManager {
    std::unordered_map<std::string, std::unordered_set<std::string>> mPluginCommands;

public:
    static ScriptCommandManager& getInstance();

    void addPluginCommand(std::string const& plugin, std::string const& cmd);

    void removePluginCommand(std::string const& plugin, std::string const& cmd);

    void removePluginCommands(std::string const& plugin);
};
