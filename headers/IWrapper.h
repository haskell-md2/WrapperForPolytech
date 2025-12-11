#pragma once

class IWrapper
{

public:
    virtual ~IWrapper() = default;
    virtual void execute() = 0;
    virtual void execute(const std::map<std::string, int> & args_map) = 0;
};

