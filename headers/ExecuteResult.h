#pragma once

#include <any>
#include <functional>
#include <iostream>

class ExecuteResult {
   private:
    std::any result_;
    std::function<void(std::ostream &, const std::any &)> printer_;

   public:
    template <typename T>
    ExecuteResult(T result)
        : result_(std::move(result)),
          printer_([](std::ostream &os, const std::any &value) {
              if constexpr (!std::is_void_v<T>) {
                  os << std::any_cast<T>(value);
              } else {
                  os << "<void>";
              }
          }) {}

    ExecuteResult(std::any result)
        : result_(std::move(result)),
          printer_([](std::ostream &os, const std::any &value) {
              if (!value.has_value()) {
                  os << "<empty>";
              } else {
                  os << "<unknown type: " << value.type().name() << ">";
              }
          }) {}

    template <typename T>
    operator T() const {
        if constexpr (std::is_void_v<T>) {
            return;
        } else {
            return std::any_cast<T>(result_);
        }
    }

    void print(std::ostream &os) const { printer_(os, result_); }
};

inline std::ostream &operator<<(std::ostream &os, const ExecuteResult &result) {
    result.print(os);
    return os;
}