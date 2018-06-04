#pragma once

#include <string>
#include <vector>

using namespace std;

class Node {
public:
    Node(string& stationname, int dist);
    ~Node();

    const string stationname;
    const int distance;

	const string getName();
    void displayInfo();
};
