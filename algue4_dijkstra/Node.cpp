#include "Node.h"
#include <iostream>

<<<<<<< HEAD
Node::Node(string & _stationname, int _dist)
    :stationname(_stationname), distance(_dist)
=======
Node::Node(string & _stationname, int _distance_left, int _distance_right)
    :stationname(_stationname), distance_left(_distance_left), distance_right(_distance_right)
>>>>>>> 267b5cd8153be3a912f0faf6d409b8051c562ade
{

}

Node::~Node() {

}

<<<<<<< HEAD
const string Node::getName()
{
	size_t pos = stationname.find(":");
	return stationname.substr(0,pos);
}

bool Node::isLonely() {
    return neighbourstation.size() == 0;
}

vector<string> &Node::getNeben() {
    return neighbourstation;
}

void Node::appendNeben(string str)
{
	neighbourstation.push_back(str);
}

void Node::displayInfo() {
    std::cout << stationname << ": " << ", distance = " << distance;
    std::cout << std::endl << "number of stations = " << neighbourstation.size() << std::endl;
=======
bool Node::isLonely() {
    return neighbourstaton.size() == 0;
}

vector<string> &Node::getNeben() {
    return neighbourstaton;
}

void Node::displayInfo() {
    std::cout << stationname << ": " << "distance_left = " << distance_left << ", distance_right = " << distance_right;
    std::cout << std::endl << "number of stations = " << neighbourstaton.size() << std::endl;
>>>>>>> 267b5cd8153be3a912f0faf6d409b8051c562ade
}
