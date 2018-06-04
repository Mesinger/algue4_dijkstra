#pragma once
#include "Node.h"
#include "stringsplit.h"
#include "Timer.h"
#include "findinmap.h"

#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>
#include <list>


typedef vector<int> weight_t;
typedef vector<int> index_t;

void computeDijkstra(int source, adjacency_list_t &adjacencyList, TStatMap &stations, weight_t &weight_vec, index_t &index_vec);

std::list<int> DijkstraGetShortestPathTo(int index, index_t &index_vec);

void printTrip(std::list<int>& path, TStatMap& stations, int weight);