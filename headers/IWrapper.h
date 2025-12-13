#pragma once

#include <any>
#include <unordered_map>

class IWrapper
{

public:
    virtual ~IWrapper() = default;
    virtual std::any execute(const std::unordered_map<std::string, std::any> & args_map) = 0;
};

