#pragma once

#include <chrono>

namespace EG {

class Timer {
public:
    Timer() {
        Reset();
    }

    void Reset() {
        start_ = std::chrono::high_resolution_clock::now();
    }

    float Elapsed() {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start_).count() * 0.001f * 0.001f * 0.001f;
    }

    float ElapsedMillis() {
        return Elapsed() * 1000.0f;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

} // namespace EG
