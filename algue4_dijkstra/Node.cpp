#include "Node.h"
#include <iostream>


Node::Node(string & _stationname, int _dist)
    :stationname(_stationname), distance(_dist)
{

}

Node::~Node() {

}

const string Node::getName()
{
	size_t pos = stationname.find(":");
	return stationname.substr(0,pos);
}

//bool Node::isLonely() {
//    return neighborstations.size() == 0;
//}
//
//vector<string> &Node::getNeben() {
//    return neighborstations;
//}

//void Node::appendNeben(string str)
//{
//	neighborstations.push_back(str);
//}

void Node::displayInfo() {
	std::cout << stationname << ": " << ", distance = " << distance;
	std::cout << std::endl << "number of stations = " << neighborstations.size() << std::endl;
}
