#pragma once

#include <map>
#include <string>

#include "IWrapper.h"

class Engine
{

public:
    void register_command(IWrapper * wrapper, const std::string& command_name) {
        commands_[command_name] = wrapper;
    }


    void execute(const std::string& command_name) {
        auto it = commands_.find(command_name);
        it->second->execute();
    }


    void execute(const std::string& command_name, const std::map<std::string, int> args_map) {
        auto it = commands_.find(command_name);
        it->second->execute(args_map);
    }

    private:
        std::map<std::string, IWrapper *> commands_;
};

