#pragma once

#include <any>

class IWrapper
{

public:
    virtual ~IWrapper() = default;
    virtual void execute() = 0;
    virtual void execute(const std::map<std::string, std::any> & args_map) = 0;
};

