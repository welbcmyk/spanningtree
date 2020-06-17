#include "TreeMatrix.h"
#include "NoNodeFoundException.h"

static vector<LinkWithNode> emptyVector = vector<LinkWithNode>();

void TreeMatrix::appendNode(node newNode)
{
	addXNode(newNode);
	addYNode(newNode);
}

bool TreeMatrix::nodeExistsByName(string nodeName)
{
	for (unsigned int i = 0; i < matrix.size(); i++)
	{
		if (nodeName == matrix[i].Node.name)
			return true;
	}
	return false;
}

node TreeMatrix::getNodeByName(string nodeName)
{
	for (unsigned int i = 0; i < matrix.size(); i++)
	{
		if (nodeName == matrix[i].Node.name)
			return matrix[i].Node;
	}
	throw(NoNodeFoundException("Reuqested Node Couldn't be found by Name."));
}

void TreeMatrix::createLink(node node1, node node2, int cost)
{
	for (unsigned int i = 0; i < matrix.size(); i++)
	{
		if (node1 == matrix[i].Node)
			createLink(matrix[i], node2, cost);
		if (matrix[i].Node == node2)
			createLink(matrix[i], node1, cost);
	}
}

bool TreeMatrix::nodeExists(node nodeItem)
{
	for (unsigned int i = 0; i < matrix.size(); i++)
	{
		if (nodeItem == matrix[i].Node)
			return true;
	}
	return false;
}

void TreeMatrix::printTree(ostream& output)
{
	vector<nodePair> printedNodePairsWithLink;
	output.flush();
	output << "SpanningTree {\n\t// Node-IDs\n";
	for (auto& linkedNode : matrix)
	{
		output << "\t" + linkedNode.Node.toString() + ";\n";
	}
	output << "\n\t// Links und zugeh. Kosten\n";
	for (auto& node1 : getAllNodes())
	{
		for (auto& node2 : getAllNodes())
		{
			nodePair firstNodePairVariant;
			nodePair otherNodePairVariant;
			firstNodePairVariant.first = node1;
			firstNodePairVariant.second = node2;
			otherNodePairVariant.first = node2;
			otherNodePairVariant.second = node1;
			if (areLinked(node1, node2) &&
				(!listConatainsNodePair(printedNodePairsWithLink, firstNodePairVariant) &&
					!listConatainsNodePair(printedNodePairsWithLink, otherNodePairVariant)))
			{
				output << "\t" + node1.name + " - " + node2.name + " : " + getLink(node1, node2).toString() + ";\n";
				printedNodePairsWithLink.insert(printedNodePairsWithLink.end(), firstNodePairVariant);
			}
		}
	}
	output << "}" << endl;
}

void TreeMatrix::addXNode(node Node)
{
	NodeWithLinkVector linkedNode;
	linkedNode.Node = Node;
	linkedNode.linksWithNodes = getDefaultLinkVector();
	matrix.insert(matrix.end(), linkedNode);
}

void TreeMatrix::addYNode(node Node)
{
	for (auto& linkedNode : matrix)
	{
		LinkWithNode linkWithNode;
		linkWithNode.Link = link();
		linkWithNode.Node = Node;
		linkedNode.linksWithNodes.insert(linkedNode.linksWithNodes.end(), linkWithNode);
	}
}

vector<LinkWithNode>& TreeMatrix::getDefaultLinkVector()
{
	if (matrix.empty())
	{
		return emptyVector;
	}
	return matrix[0].linksWithNodes;
}

void TreeMatrix::createLink(NodeWithLinkVector& linkedNode, node otherNode, int cost) 
{
	for (auto& nodeItr : linkedNode.linksWithNodes)
	{
		if (otherNode == nodeItr.Node)
			nodeItr.Link = link(cost, linkedNode.Node.nodeCost);
	}
}

bool TreeMatrix::areLinked(node node1, node node2)
{
	if (node1 == node2)
		return false;
	for (auto& linkedNode : matrix)
	{
		if (linkedNode.Node == node1)
			return isPairedWithValidLink(node2, linkedNode.linksWithNodes);
		if(linkedNode.Node == node2)
			return isPairedWithValidLink(node1, linkedNode.linksWithNodes);
	}
	throw(NoNodeFoundException("Reuqested Node doesn't exist."));
}

vector<LinkWithNode>& TreeMatrix::getLinkVector(node Node)
{
	for (auto& linkedNode : matrix)
	{
		if (linkedNode.Node == Node)
			return linkedNode.linksWithNodes;
	}
	throw(NoNodeFoundException("Reuqested Node doesn't exist."));
}

bool TreeMatrix::isPairedWithValidLink(node Node, vector<LinkWithNode> linkNodeVector)
{
	for (auto& linkNodePair : linkNodeVector)
	{
		if (linkNodePair.Link.rootID != 0 && linkNodePair.Node == Node)
			return true;
	}
	return false;
}

vector<node> TreeMatrix::getAllNodes()
{
	vector<node> allNodes;
	for (auto& linkedNode : matrix)
	{
		allNodes.insert(allNodes.end(), linkedNode.Node);
	}
	return allNodes;
}

link& TreeMatrix::getLink(node destinationNode, node sourceNode)
{
	for (auto& linkNodePair : getLinkVector(destinationNode))
	{
		if (sourceNode == linkNodePair.Node)
			return linkNodePair.Link;
	}
	throw(NoNodeFoundException("Reuqested Nodes doen't exist."));
}

bool TreeMatrix::listConatainsNodePair(vector<nodePair> nodePairs, nodePair nodes)
{
	for (auto& pairItr : nodePairs)
	{
		if (pairItr.first == nodes.first && nodes.second == pairItr.second)
			return true;
	}
	return false;
}

void TreeMatrix::printNodes(ostream& output)
{
	output << "Node\tNH\tRID\tCost" << endl;
	for (auto& nodeItr : getAllNodes())
	{
		string rootName;
		node nextHopNode = getNodeByID(nodeItr.nextHop);
		link linkToNextHop = getLink(nodeItr, nextHopNode);
		if (nodeItr == nextHopNode)
		{
			rootName = nodeItr.name;
		}
		else if (linkToNextHop.rootID == 0)
		{
			rootName = "null";
		}
		else
		{
			rootName = getNodeByID(getLink(nodeItr, nextHopNode).rootID).name;
		}
		//string rootName = (getLink(nodeItr, nextHopNode).rootID == 0 ? "null" : (getNodeByID(getLink(nodeItr, nextHopNode).rootID)).name);
		output << nodeItr.name << "\t" << getNodeByID(nodeItr.nextHop).name << "\t" << rootName << "\t" << to_string(getLink(nodeItr, getNodeByID(nodeItr.nextHop)).kosten) << endl;
	}
}

node TreeMatrix::getNodeByID(int id)
{
	for (auto& nodeItr : getAllNodes())
	{
		if (nodeItr.nodeCost == id)
			return nodeItr;
	}
	throw(NoNodeFoundException("Reuqested Nodes doen't exist."));
}

void TreeMatrix::updateRootIdFromLinks(vector<LinkWithNode>& links, int rootID)
{
	for (auto& lnkItr : links)
	{
		lnkItr.Link.rootID = rootID;
	}
}

void TreeMatrix::updateNode(string nodeName)
{
	NodeWithLinkVector& linkedNode = getNodeWithLinkVectorFromMatrix(getNodeByName(nodeName));
	updateOutgoingLinks(linkedNode);
	linkedNode.Node.msgCnt++;
}

NodeWithLinkVector& TreeMatrix::getNodeWithLinkVectorFromMatrix(node Node)
{
	for (auto& i : matrix)
	{
		if (Node == i.Node)
			return i;
	}
	throw(NoNodeFoundException("Reuqested Nodes doen't exist."));
}

int TreeMatrix::calcCostToRoot(link lnk)
{
	return lnk.kosten + lnk.summeKosten;
}


int TreeMatrix::getLowestRootID(NodeWithLinkVector linkedNode)
{
	int retVal = linkedNode.Node.nodeCost;
	for (auto& lnk : linkedNode.linksWithNodes)
	{
		if (lnk.Link.rootID < retVal && lnk.Link.rootID != 0)
			retVal = lnk.Link.rootID;
	}
	return retVal;
}

void TreeMatrix::updateOutgoingLinks(NodeWithLinkVector linkedNode)
{
	int rootID = getLowestRootID(linkedNode);
	getNodeWithLinkVectorFromMatrix(getNodeByID(linkedNode.Node.nodeCost)).Node.nextHop = calcNextHop(linkedNode.Node.nodeCost, rootID);
	int costToRoot = calcCostToRoot(linkedNode.Node.nodeCost, rootID);
	for (auto& nodeItr : matrix)
	{
		if (areLinked(linkedNode.Node, nodeItr.Node))
		{
			link& linkOut = getLink(nodeItr.Node, linkedNode.Node);
			updateRootID(linkOut, rootID);
			updateCostToRoot(linkOut, costToRoot);
		}
	}
}

void TreeMatrix::updateRootID(link& Link, int rootID)
{
	Link.rootID = rootID;
}

void TreeMatrix::updateCostToRoot(link& outGoingLink, int costToRoot)
{
	outGoingLink.summeKosten = costToRoot;
}

int TreeMatrix::calcCostToRoot(int nodeId, int rootId)
{
	if (rootId == nodeId)
		return 0;
	LinkWithNode cheapestLink = getCheapestLinkToRoot(nodeId, rootId);
	return cheapestLink.Link.kosten + cheapestLink.Link.summeKosten;
}

LinkWithNode TreeMatrix::getCheapestLinkToRoot(int nodeId, int rootId)
{
	return getCheapestLinkToRoot(getNodeWithLinkVectorFromMatrix(getNodeByID(nodeId)).linksWithNodes, rootId);
}

LinkWithNode TreeMatrix::getCheapestLinkToRoot(vector<LinkWithNode> links, int rootId)
{
	LinkWithNode retVal;
	int i = 0;
	do
	{
		retVal = links[i++];
	} while (retVal.Link.rootID == 0);
	for (auto& lnk : links)
	{
		if (rootId == lnk.Link.rootID && retVal.Link.summeKosten + retVal.Link.kosten > lnk.Link.kosten + lnk.Link.summeKosten)
			retVal = lnk;
	}
	return retVal;
}

int TreeMatrix::calcNextHop(int nodeId, int rootId)
{
	if (rootId == nodeId)
		return nodeId;
	LinkWithNode cheapestLink = getCheapestLinkToRoot(nodeId, rootId);
	return cheapestLink.Node.nodeCost;
}

void TreeMatrix::updateAllNodes()
{
	for (auto& nodeItr : matrix)
	{
		updateNode(nodeItr.Node.name);
	}
}

void TreeMatrix::printMatrix(ostream& output)
{
	output << "\n";
	for (auto& i : getAllNodes())
	{
		output << "\t" << i.name;
	}
	output << "\n";
	for (auto& i : getAllNodes())
	{
		output << i.name << " " << i.nodeCost << " " << i.nextHop << " |\t";
		for (auto& y : getAllNodes())
		{
			output << getLink(y, i).rootID << ":" << getLink(y, i).summeKosten << "\t";
		}
		output << endl;
	}
}

void TreeMatrix::printResult(ostream& output)
{
	string result;
	int rootId = getLink(matrix[0].Node, getNodeByID(matrix[0].Node.nextHop)).rootID;
	result = "Spanning-Tree of mygraph {\n\n";
	result.append("\tRoot: " + getNodeByID(rootId).name + ";\n");
	for (auto& i : getAllNodes())
	{
		if (getLink(i, getNodeByID(i.nextHop)).rootID != rootId && i.nodeCost != rootId)
		{
			output << "\n\nRoot couldn't be found.\n\n";
			return;
		}
		if (getNodeByID(rootId) != i)
		{
			result.append("\t" + i.name + " - " + getNodeByID(i.nextHop).name + ";\n");
		}
	}
	result.append("}\n");
	output << result;
}


bool TreeMatrix::allNodesReceivedMessages(int minMsg)
{
	bool retVal = true;
	for (auto& i : matrix)
	{
		if (retVal)
		{
			retVal = i.Node.msgCnt >= minMsg;
		}
		else return false;
	}
	return retVal;
}