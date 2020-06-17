#pragma once
#include <string>
#define MAXKOSTEN 50
using namespace std;
class link
{
public:
	link() : kosten(0), rootID(0), summeKosten(0) {}
	link(int cost, int rootID = 0, int summeKosten = 0) : kosten(cost), rootID(rootID), summeKosten(summeKosten) {}
	~link() {}
	// Linkkosten von Node_i -> Node_k
	// kosten=0: kein Link vorhanden
	// Entspricht ursprüngliche Initialisierung des eingelesenen Graphen
	int kosten;
	// Über diesen Link erhaltene Nachricht der Nachbarknoten
	// mit Vorschlag der Root incl. Gesamtkosten zur Root
	int rootID;
	int summeKosten;
	bool operator < (const link& rhs) const // must be const-correct
	{
		// return true if, in a sorted sequence, the node *this should appear before the node rhs 
		// for example:
		return this->kosten < rhs.kosten; // ordered on ascending value 
	}
	bool operator ==(link otherLink)
	{
		return kosten == otherLink.kosten &&
			rootID == otherLink.rootID &&
			summeKosten == otherLink.summeKosten;
	}
	string toString();
};

