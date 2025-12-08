#pragma once

class IWrapper
{

public:
    virtual ~IWrapper() = default;
    virtual void execute() = 0;
};

