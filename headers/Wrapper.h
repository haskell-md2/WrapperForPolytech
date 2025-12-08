#pragma once

#include "IWrapper.h"

#include <functional>

#include <memory>

template<typename ClassName, typename ReturnType>
class Wrapper : public IWrapper
{
    private:

        std::function<ReturnType(ClassName&)> _function;

        /*
        Из ТЗ подразумевается, что оборачиваемый объект, вероятнее всего,
        создаётся на стеке. Поэтому, умными указателями можно пренебречь.
        */ 
        ClassName * _subj;

    public:


        Wrapper(ClassName* subj, ReturnType (ClassName::*func)())
            : _subj(subj) 
        {
            _function = [subj, func](ClassName& obj) -> ReturnType {
                return (obj.*func)();
            };
        }

        void execute() override {
            _function(*_subj);
        }

};


