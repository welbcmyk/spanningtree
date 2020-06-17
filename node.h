#pragma once
#include<vector>
#include <string>
#include "link.h"
#define MAX_NODE_ID 20

using namespace std;
class node
{
public:
	node() : name("0"), msgCnt(0), nextHop(0), nodeCost(0) { }
	node(string name, int nodeCost);
	~node() {
		
	}

	string name; // Bezeichner des Knotens
	int nodeCost; // Knoten Gewicht > 0
	int nextHop; // Berechneter Link zum nächsten Knoten in
	// Richtung Root // using vector
	int msgCnt; // Zählt mit, wie oft der Knoten bei der
	// Bearbeitung des Algorithmus aufgerufen wird
	bool operator ==(node otherNode)
	{
		return name == otherNode.name &&
			nodeCost == otherNode.nodeCost;
	}
	bool operator !=(node otherNode)
	{
		return !(*this == otherNode);
	}
	bool operator < (const node& rhs) const // must be const-correct
	{
		// return true if, in a sorted sequence, the node *this should appear before the node rhs 
		// for example:
		return this->nodeCost < rhs.nodeCost; // ordered on ascending value 
	}
	string toString();
};

