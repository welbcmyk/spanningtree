#include "node.h"
#include "NoNodeFoundException.h"
#include <string.h>

node::node(string name, int nodeCost)
	: msgCnt(0), nextHop(0)
{
	this->name = name;
	this->nodeCost = nodeCost;
	this->nextHop = nodeCost;
}

string node::toString()
{
	return name + " = " + to_string(nodeCost);
}