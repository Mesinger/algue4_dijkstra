#pragma once

#include <map>
#include "Node.h"
#include <iostream>

typedef std::vector<std::vector<Node>> adjacency_list_t;
typedef std::pair<std::string, int> TStatPair;
typedef std::map<std::string, int> TStatMap;

//Suchfunktion um Stationen in der Map zu finden
TStatMap::const_iterator FindPrefix(const TStatMap& map, const string& search_for);

//"Debugfunktion" zur Suche
TStatMap::const_iterator Test(const TStatMap& map, const string& search_for);