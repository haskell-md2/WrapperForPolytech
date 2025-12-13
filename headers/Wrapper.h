#pragma once

#include "IWrapper.h"

#include <functional>


using ArgumentMap = std::vector<std::pair<std::string, std::any>>;

template<typename ClassName, typename ReturnType, typename... Args>
class Wrapper : public IWrapper {

    private:
        std::function<ReturnType(ClassName*, Args...)> _function;
        ClassName * _subj;
        std::map<std::string, int> _nameOfArg_to_NumberInArgs;
        std::array<std::any, sizeof...(Args)> _default_args;

        std::any callWithArgs(const std::map<std::string, std::any>& args_map) {
            if constexpr (sizeof...(Args) == 0) {
                if constexpr (std::is_void_v<ReturnType>) {
                    _function(_subj);
                    return {};
                } else {
                    return _function(_subj);
                }
            } else {
                std::array<std::any, sizeof...(Args)> args_array = _default_args;
                for(const auto& [key, value] : args_map) {
                    auto it = _nameOfArg_to_NumberInArgs.find(key);
                    if(it != _nameOfArg_to_NumberInArgs.end()) {
                        args_array[it->second] = value;
                    } else {
                        throw std::invalid_argument("Неизвестный параметр: " + key);
                    }
                }
                return callWithArray(args_array);
            }
        }

        std::any callWithArray(const std::array<std::any, sizeof...(Args)>& args) {
            auto call_func = [this, &args]<std::size_t... Is>(std::index_sequence<Is...>) {
                if constexpr (std::is_void_v<ReturnType>) {
                    _function(_subj, std::any_cast<Args>(args[Is])...);
                    return std::any{};
                } else {
                    return std::any(_function(_subj, std::any_cast<Args>(args[Is])...));
                }
            };
            return call_func(std::index_sequence_for<Args...>{});
        }

    public:
        Wrapper(ClassName* subj, ReturnType (ClassName::*func)(Args...),
                ArgumentMap default_arguments)
            : _subj(subj)
        {
            int i = 0;
            for(const auto& d_a : default_arguments) {
                _nameOfArg_to_NumberInArgs[d_a.first] = i;
                _default_args[i] = d_a.second;
                i++;
            }

            _function = [func](ClassName* obj, Args... args) -> ReturnType {
                return (obj->*func)(args...);
            };
        }

        std::any execute() override {
            return callWithArgs({});
        }

        std::any execute(const std::map<std::string, std::any>& args_map) override {
            return callWithArgs(args_map);
        }
};

template<typename ClassName, typename ReturnType, typename... Args>
Wrapper(ClassName*, ReturnType (ClassName::*)(Args...), ArgumentMap)
    -> Wrapper<ClassName, ReturnType, Args...>;

//TODO list
/*
-. Сделать проверку, что в словарь подаётся ровно столько аргументов, сколько их в обарачиваемой функции
-. Разобраться - на стеке или куче будет создаваться обёртка
-. Добавить обработки возможных исключений:
    - Несовместимый тип (если будет реализована соответствующая фича)
*/
