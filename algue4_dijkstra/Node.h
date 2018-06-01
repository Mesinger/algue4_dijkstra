#pragma once

#include <string>
#include <vector>

using namespace std;

class Node {
private:

<<<<<<< HEAD
    vector <string> neighbourstation;
public:
    Node(string& stationname, int dist);
    ~Node();

    const string stationname;
    const int distance;

	const string getName();
    bool isLonely();
    vector<string>& getNeben();
	void appendNeben(string str);
=======
    vector <string> neighbourstaton;
public:
    Node(string& stationname,int distance_left, int distance_right);
    ~Node();

    const string stationname;
    const int distance_left, distance_right;

    bool isLonely();
    vector<string>& getNeben();
>>>>>>> 267b5cd8153be3a912f0faf6d409b8051c562ade
    void displayInfo();
};
