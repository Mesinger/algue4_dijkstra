#pragma once

#include <string>
#include <chrono>

class Timer {
public:
    Timer(const std::string& name);
    ~Timer();
private:
    const std::string name;
    std::chrono::time_point<std::chrono::steady_clock> start, end;
};