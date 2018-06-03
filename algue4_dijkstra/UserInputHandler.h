#pragma once

#include <vector>
#include "findinmap.h"

class UserInputHandler {
public:
    UserInputHandler();
    ~UserInputHandler();

    void getStartStation(std::vector<int>& startstations, adjacency_list_t& adjacencyList, TStatMap& stations, std::vector<std::string>& lines);
    void getEndStation(std::vector<int>& endstations, adjacency_list_t& adjacencyList,TStatMap& stations, std::vector<std::string>& lines);
};