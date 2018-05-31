#include "Node.h"
#include <iostream>

Node::Node(string & _stationname, int _distance_left, int _distance_right)
    :stationname(_stationname), distance_left(_distance_left), distance_right(_distance_right)
{

}

Node::~Node() {

}

bool Node::isLonely() {
    return neighbourstaton.size() == 0;
}

vector<string> &Node::getNeben() {
    return neighbourstaton;
}

void Node::displayInfo() {
    std::cout << stationname << ": " << "distance_left = " << distance_left << ", distance_right = " << distance_right;
    std::cout << std::endl << "number of stations = " << neighbourstaton.size() << std::endl;
}
