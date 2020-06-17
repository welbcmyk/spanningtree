#pragma once
#include <fstream>
#include <iostream>
#include "TreeMatrix.h"
#include "InvalidSpanTreeFileException.h"
enum class lineType
{
	empty,
	node,
	link
};

enum class nodeSyntaxState
{
	start,
	nodeName,
	// spaceBeforeEqualSign, // delete blanks before checking Syntax
	equalsign,
	// spaceAfterEqualSign,  // delete blanks before checking Syntax
	nodeCost,
	end,
	invalid
};

enum class linkSyntaxState
{
	start,
	node1Name,
	dash,
	node2Name,
	doubleDot,
	cost,
	end,
	invalid
};

enum class commentSyntaxState
{
	noComment,
	firstBacksslash,
	comment
};

using namespace std;
class SpanTreeGenerator
{
public:
	TreeMatrix getSpanTree(const string filename);
private:
	TreeMatrix spanTree;
	lineType getLineType(const string line);
	void processLine(const string line);
	node getNode(const string nodeStr);
	void readLink(const string linkStr);
	bool containsEndBracket(const string line);
	bool checkNodeSyntax(const string line);
	bool checkLinkSyntax(const string line);
	void changeNodeState(nodeSyntaxState& state, commentSyntaxState& commentState, char charItem);
	void changeLinkState(linkSyntaxState& state, commentSyntaxState& commentState, char charItem);
	void changeCommentState(commentSyntaxState& state, char charItem);
	void removeCharsFromString(string& str, char* charsToRemove);
};

