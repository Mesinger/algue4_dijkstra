#include "Timer.h"
#include <chrono>
#include <iostream>

Timer::Timer(const std::string & _name)
:name(_name), start(std::chrono::high_resolution_clock::now())
{

}

Timer::~Timer() {

    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;

    float ms = duration.count() * 1000.f;

    std::cout << name << " took " << ms << " ms" << std::endl;
}
