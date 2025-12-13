#pragma once

#include <unordered_map>
#include <string>
#include <any>

#include "IWrapper.h"
#include "ExecuteResult.h"

class Engine {
public:
    void register_command(IWrapper* wrapper, const std::string& command_name) {
        commands_[command_name] = wrapper;
    }

    ExecuteResult execute(const std::string& command_name, 
                        const std::unordered_map<std::string, std::any>& args_map = {}) {
        auto it = commands_.find(command_name);
        if (it == commands_.end()) {
            throw std::runtime_error("Команда не найдена: " + command_name);
        }
        return ExecuteResult(it->second->execute(args_map));
    }

private:
    std::unordered_map<std::string, IWrapper*> commands_;
};
