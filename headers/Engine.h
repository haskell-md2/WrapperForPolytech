#pragma once

#include <any>
#include <string>
#include <unordered_map>

#include "ExecuteResult.h"
#include "IWrapper.h"

class Engine {
   public:
    void register_command(IWrapper *wrapper, const std::string &command_name) {
        commands_[command_name] = wrapper;
    }

    ExecuteResult execute(
        const std::string &command_name,
        const std::unordered_map<std::string, std::any> &args_map = {}) {
        auto it = commands_.find(command_name);
        if (it == commands_.end()) {
            throw std::runtime_error("Команда не найдена: " + command_name);
        }

        std::any result_any = it->second->execute(args_map);

        return std::any_cast<ExecuteResult>(result_any);
    }

   private:
    std::unordered_map<std::string, IWrapper *> commands_;
};