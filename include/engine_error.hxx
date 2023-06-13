#pragma once

#include <sstream>
#include <stdexcept>

namespace sdk
{
class engine_error final : public std::runtime_error
{
public:
    using runtime_error::runtime_error;

    explicit engine_error(const char *message, const char *function_name)
        : std::runtime_error(message), function_name_(function_name)
    {
    }

    explicit engine_error(const char *message, const char *function_name,
                          const char *object_name)
        : std::runtime_error(message), function_name_(function_name),
          object_name_(object_name)
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        std::ostringstream oss;

        if (object_name_ != nullptr)
        {
            oss << object_name_ << ".";
        }

        if (function_name_ != nullptr)
        {
            oss << function_name_ << " : ";
        }

        oss << std::runtime_error::what();

        info_ = oss.str();
        return info_.c_str();
    }

private:
    const char *function_name_ = nullptr;
    const char *object_name_   = nullptr;

    mutable std::string info_;
};
} // namespace sdk