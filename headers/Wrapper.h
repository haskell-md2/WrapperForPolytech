#pragma once

#include <functional>
#include <sstream>
#include <typeindex>
#include <typeinfo>

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
    std::array<std::type_index, sizeof...(Args)> _expected_types;

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

    template <std::size_t... Is>
    static constexpr std::array<std::type_index, sizeof...(Args)>
    makeExpectedTypes(std::index_sequence<Is...>) {
        if constexpr (sizeof...(Args) > 0) {
            return {std::type_index(typeid(Args))...};
        } else {
            return {};
        }
    }

   public:
    Wrapper(ClassName *subj, ReturnType (ClassName::*func)(Args...),
            ArgumentMap default_arguments)
        : _subj(subj),
          _expected_types(
              makeExpectedTypes(std::index_sequence_for<Args...>{})) {
        if (default_arguments.size() != sizeof...(Args)) {
            std::ostringstream error_msg;
            error_msg << "Несовпадение количества параметров: ожидается "
                      << sizeof...(Args) << ", передано "
                      << default_arguments.size();
            throw std::invalid_argument(error_msg.str());
        }

        int i = 0;
        for (const auto &[name, value] : default_arguments) {
            if constexpr (sizeof...(Args) > 0) {
                std::type_index expected_type = _expected_types[i];
                std::type_index actual_type = value.type();

                if (expected_type != actual_type) {
                    std::ostringstream error_msg;
                    error_msg << "Несовпадение типа параметра \"" << name
                              << "\" (позиция " << i << "): ожидается "
                              << expected_type.name() << ", передан "
                              << actual_type.name();
                    throw std::invalid_argument(error_msg.str());
                }
            }

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
