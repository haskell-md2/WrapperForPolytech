#pragma once

#include "IWrapper.h"

#include <functional>

#include <memory>

template<typename ClassName, typename ReturnType, typename ... Args>
class Wrapper : public IWrapper
{
    private:

        std::function<ReturnType(ClassName*, Args...)> _function;

        /*
            Объект, чья функция оборачиваися, может создаваться и на стеке. shared_ptr не применим.
            Лучше даже будем кидать исключение в случае, когда пытается вызваться функция из удалённого объекта.
        */ 
        ClassName * _subj;

        std::map<std::string, int> _default_arguments;


        void callWithArgs(const std::map<std::string, int> & args_map){
            if constexpr (sizeof...(Args) == 0) {
                _function(_subj);
            } else {
                std::array<int, sizeof...(Args)> args_array;
                
                for (size_t i = 0; i < sizeof...(Args); ++i) {
                    std::string key = "arg" + std::to_string(i + 1);
                    args_array[i] = args_map.at(key);
                }
                
                callWithArray(args_array);
            }
        }

        void callWithArray(const std::array<int, sizeof...(Args)>& args) {
            auto call_func = [this, &args]<size_t... Is>(std::index_sequence<Is...>) {
                _function(_subj, args[Is]...);
            };
            
            call_func(std::index_sequence_for<Args...>{});
        }

    public:


        Wrapper(ClassName* subj, ReturnType (ClassName::*func)(Args...),
        const std::map<std::string, int> default_arguments)
            : _subj(subj), _default_arguments(std::move(default_arguments)) 
        {
            
            _function = [func](ClassName* obj, Args... args) -> ReturnType {
                return (obj->*func)(args...);
            };
        }

        void execute() override {
            callWithArgs(_default_arguments);
        }

        void execute(const std::map<std::string, int> & args_map) override {
            callWithArgs(args_map);
        }

};

//TODO list
/*
-. Сделать проверку, что в словарь подаётся ровно столько аргументов, сколько их в обарачиваемой функции
-. Хочется, чтобы можно было подавать любой типа данных.
-. Разобраться - на стеке или куче будет создаваться обёртка
-. Добавить обработки возможных исключений:
    - Ключ, отличный от вида argn
    - Пропущен какой-то argn
    - Несовместимый тип (если будет реализована соответствующая фича)
    - Несуществующий argn
-. Поддержка оборачивания функций с template<typename... Args>
*/
