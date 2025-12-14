#pragma once

#include <functional>

#include "ExecuteResult.h"
#include "IWrapper.h"

using ArgumentMap = std::vector<std::pair<std::string, std::any>>;
template <typename ClassName, typename ReturnType, typename... Args>
class Wrapper : public IWrapper {
   private:
    std::function<ReturnType(ClassName *, Args...)> _function;
    ClassName *_subj;
    std::unordered_map<std::string, int> _nameOfArg_to_NumberInArgs;
    std::array<std::any, sizeof...(Args)> _default_args;

    template <std::size_t... Is>
    std::any callWithArrayImpl(
        const std::array<std::any, sizeof...(Args)> &args,
        std::index_sequence<Is...>) {
        if constexpr (std::is_void_v<ReturnType>) {
            _function(_subj, std::any_cast<Args>(args[Is])...);
            return std::any(ExecuteResult(std::any()));
        } else {
            ReturnType result =
                _function(_subj, std::any_cast<Args>(args[Is])...);
            return std::any(ExecuteResult(result));
        }
    }

    std::any callWithArray(const std::array<std::any, sizeof...(Args)> &args) {
        return callWithArrayImpl(args, std::index_sequence_for<Args...>{});
    }

    std::any callWithArgs(
        const std::unordered_map<std::string, std::any> &args_map) {
        if constexpr (sizeof...(Args) == 0) {
            if constexpr (std::is_void_v<ReturnType>) {
                _function(_subj);
                return std::any(ExecuteResult(std::any()));
            } else {
                ReturnType result = _function(_subj);
                return std::any(ExecuteResult(result));
            }
        } else {
            std::array<std::any, sizeof...(Args)> args_array = _default_args;

            for (const auto &[key, value] : args_map) {
                auto it = _nameOfArg_to_NumberInArgs.find(key);
                if (it != _nameOfArg_to_NumberInArgs.end()) {
                    args_array[it->second] = value;
                } else {
                    throw std::invalid_argument("Неизвестный параметр: " + key);
                }
            }
            return callWithArray(args_array);
        }
    }

   public:
    Wrapper(ClassName *subj, ReturnType (ClassName::*func)(Args...),
            ArgumentMap default_arguments)
        : _subj(subj) {
        int i = 0;
        for (const auto &[name, value] : default_arguments) {
            _nameOfArg_to_NumberInArgs[name] = i;
            _default_args[i] = value;
            i++;
        }

        _function = [func](ClassName *obj, Args... args) -> ReturnType {
            return (obj->*func)(args...);
        };
    }

    std::any execute(
        const std::unordered_map<std::string, std::any> &args_map) override {
        return callWithArgs(args_map);
    }
};
