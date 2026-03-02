// Source - https://stackoverflow.com/q/53460214
// Posted by Phamily, modified by community. See post 'Timeline' for change history
// Retrieved 2026-02-27, License - CC BY-SA 4.0

#pragma once
#include <chrono>

class Timer {
public:
    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    }

    long long getTime() {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        return duration.count();
    }

    double getTimeSeconds() {
        auto duration = std::chrono::duration<double>(endTime - startTime);
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
};