#pragma once

#include <any>

class IWrapper
{

public:
    virtual ~IWrapper() = default;
    virtual std::any execute(const std::map<std::string, std::any> & args_map) = 0;
};

