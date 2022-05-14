#pragma once
#include <chrono>


class timer final
{
public:
    explicit timer(const double seconds_to_wait = 0.2) : seconds_to_wait_(seconds_to_wait) {}
    double& get_time() {
        return seconds_to_wait_;
    }
    bool is_timer_out() {
        const auto current = std::chrono::high_resolution_clock::now();
        const auto different_time = std::chrono::duration<double>(current - start_).count();
        if (seconds_to_wait_ < static_cast<double>(different_time)) {
            start_ = current;
            return true;
        }
        return false;
    }

    ~timer() = default;

private:
    double seconds_to_wait_;
    std::chrono::steady_clock::time_point start_ = std::chrono::steady_clock::now();
};
