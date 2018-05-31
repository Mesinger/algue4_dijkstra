#pragma once

#include <string>
#include <vector>

using namespace std;

class Node {
private:

    vector <string> neighbourstaton;
public:
    Node(string& stationname,int distance_left, int distance_right);
    ~Node();

    const string stationname;
    const int distance_left, distance_right;

    bool isLonely();
    vector<string>& getNeben();
    void displayInfo();
};
