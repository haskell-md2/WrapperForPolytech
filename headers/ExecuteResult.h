#pragma once

#include <any>

class ExecuteResult {
private:
    std::any result_;
    
public:
    ExecuteResult(std::any result) : result_(std::move(result)) {}
    
    template<typename T>
    operator T() const {
        if constexpr (std::is_void_v<T>) {
            return;
        } else {
            return std::any_cast<T>(result_);
        }
    }
};
