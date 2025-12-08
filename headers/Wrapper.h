#pragma once

#include "IWrapper.h"

#include <functional>

template<typename ClassName, typename ReturnType>
class Wrapper : public IWrapper
{
private:
    ClassName * _subj;
    ReturnType (ClassName::*_function)();

public:

    //указатель на объект требуется по ТЗ
    Wrapper(ClassName * subj, ReturnType (ClassName::*function)()): _subj(subj), _function(function) {}

    void execute() {
        (_subj->*_function)();
    }


};

// Wrapper::Wrapper(/* args */)
// {
// }

// Wrapper::~Wrapper()
// {
// }
