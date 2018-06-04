#pragma once

#include <string>
#include <vector>

using namespace std;

class Node {
private:

    vector <string> neighborstations;
public:
    Node(string& stationname, int dist);
    ~Node();

    const string stationname;
    const int distance;

	const string getName();
    /*bool isLonely();
    vector<string>& getNeben();*/
	/*void appendNeben(string str);*/
    void displayInfo();
};
