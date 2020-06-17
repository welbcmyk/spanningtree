#pragma once
#include "node.h"
#include "link.h"

#define MAX_ITEMS 300

struct nodePair
{
	node first;
	node second;
};

struct LinkWithNode
{
	node Node;
	link Link;
};
struct NodeWithLinkVector
{
	node Node;
	vector<LinkWithNode> linksWithNodes;
};

class TreeMatrix
{
public:
	TreeMatrix() {}
	~TreeMatrix() {}
	vector<NodeWithLinkVector> matrix;

	void appendNode(node newNode);
	bool nodeExistsByName(string nodeName);
	node getNodeByName(string nodeName);
	void createLink(node node1, node node2, int cost);
	bool nodeExists(node nodeItem);
	void printTree(ostream& output);
	void printNodes(ostream& output);
	void printMatrix(ostream& output);
	void printResult(ostream& output);
	void updateAllNodes();
	bool allNodesReceivedMessages(int minMsg);

private:
	void updateNode(string nodeName);
	void addXNode(node Node); // adds the node on the top row (X-Axis) of the matrix
	void addYNode(node Node); // adds the node on the left column (Y-Axis) of the matrix
	vector<LinkWithNode>& getDefaultLinkVector();
	void createLink(NodeWithLinkVector& linkedNode, node otherNode, int cost);
	bool areLinked(node node1, node node2);
	vector<LinkWithNode>& getLinkVector(node Node);
	bool isPairedWithValidLink(node Node, vector<LinkWithNode> linkNodeVector);
	vector<node> getAllNodes();
	link& getLink(node destinationNode, node sourceNode);
	bool listConatainsNodePair(vector<nodePair> nodePairs, nodePair nodes);
	node getNodeByID(int id);
	void updateRootIdFromLinks(vector<LinkWithNode>& links, int rootID);
	NodeWithLinkVector& getNodeWithLinkVectorFromMatrix(node Node);
	int calcCostToRoot(link lnk);
	int getLowestRootID(NodeWithLinkVector linkedNode);
	void updateOutgoingLinks(NodeWithLinkVector linkedNode);
	void updateRootID(link& Link, int rootID);
	static void updateCostToRoot(link& outGoingLink, int costToRoot);
	int calcCostToRoot(int nodeId, int rootId);
	int calcNextHop(int nodeId, int rootId);
	LinkWithNode getCheapestLinkToRoot(int nodeId, int rootId);
	LinkWithNode getCheapestLinkToRoot(vector<LinkWithNode> links, int rootId);
};

