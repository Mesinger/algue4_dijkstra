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

void Node::displayInfo() {
	std::cout << stationname << ": " << ", distance = " << distance;
}
