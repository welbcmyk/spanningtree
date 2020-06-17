#include <sstream>
#include <string>
#include "SpanTreeGenerator.h"

using namespace std;
char whitespaces[] = { ' ', '\t' };

TreeMatrix SpanTreeGenerator::getSpanTree(const string filename)
{
	spanTree = TreeMatrix();
	ifstream inputFile;
	string line;
	inputFile.open(filename);
	unsigned int charIterator = 0;
	char input;
	do
	{
		input = inputFile.get();
		if (inputFile.eof())
			throw(InvalidSpanTreeFileException("Invalid File: Does not contain \'{\' character."));
	} while (input != '{');

	while (getline(inputFile, line))
	{
		removeCharsFromString(line, whitespaces);
		processLine(line);
		if (containsEndBracket(line))
		{
			inputFile.close();
			break;
		}
	}
	if (inputFile.is_open())
		inputFile.close();
	return spanTree;
}

lineType SpanTreeGenerator::getLineType(const string line)
{
	bool isPossibleNode = line.find('=') != string::npos;
	bool isPossibleLink = line.find('-') != string::npos && line.find(':') != string::npos;
	if (isPossibleNode && isPossibleLink)
		return lineType::empty;
	else if (isPossibleNode)
		return checkNodeSyntax(line) ? lineType::node : lineType::empty;
	else if (isPossibleLink)
		return checkLinkSyntax(line) ? lineType::link : lineType::empty;
	return lineType::empty;
}

void SpanTreeGenerator::processLine(const string line)
{
	lineType type = getLineType(line);
	switch (type)
	{
	case lineType::empty:
		break;
	case lineType::node:
		spanTree.appendNode(getNode(line));
		break;
	case lineType::link:
		readLink(line);
		break;
	default:
		break;
	}
}

node SpanTreeGenerator::getNode(const string nodeStr)
{
	string nodeName = nodeStr.substr(0, nodeStr.find('='));
	string nodeCost = nodeStr.substr(nodeStr.find('=') + 1, nodeStr.find(';'));
	return node(nodeName, stoi(nodeCost));
}

void SpanTreeGenerator::readLink(const string linkStr)
{
	string editedLinkStr = linkStr;
	string node1Name = editedLinkStr.substr(0, editedLinkStr.find('-') - 0);
	string node2Name = editedLinkStr.substr(editedLinkStr.find('-') + 1, editedLinkStr.find(':') - (editedLinkStr.find('-') + 1));
	string linkCost = editedLinkStr.substr(editedLinkStr.find(':') + 1, editedLinkStr.find(';') - (editedLinkStr.find(':') + 1));
	spanTree.createLink(spanTree.getNodeByName(node1Name), spanTree.getNodeByName(node2Name), stoi(linkCost));
}

bool SpanTreeGenerator::containsEndBracket(const string line)
{
	return line.find('}') != string::npos;
}

bool SpanTreeGenerator::checkNodeSyntax(const string line)
{
	nodeSyntaxState state = nodeSyntaxState::start;
	commentSyntaxState commentState = commentSyntaxState::noComment;
	for (unsigned int i = 0; i < line.length(); i++)
	{
		changeCommentState(commentState, line[i]);
		if (commentState == commentSyntaxState::comment)
			break;

		changeNodeState(state, commentState, line[i]);
		if (state == nodeSyntaxState::invalid)
			return false;
	}
	return state == nodeSyntaxState::end;
}

bool SpanTreeGenerator::checkLinkSyntax(const string line)
{
	linkSyntaxState state = linkSyntaxState::start;
	commentSyntaxState commentState = commentSyntaxState::noComment;
	for (unsigned int i = 0; i < line.length(); i++)
	{
		changeCommentState(commentState, line[i]);
		if (commentState == commentSyntaxState::comment)
			break;

		changeLinkState(state, commentState, line[i]);
		if (state == linkSyntaxState::invalid)
			return false;
	}
	return state == linkSyntaxState::end;
}

void SpanTreeGenerator::removeCharsFromString(string& str, char* charsToRemove) {
	for (unsigned int i = 0; i < strlen(charsToRemove); i++) {
		while (str.find(charsToRemove[i]) != string::npos)
		{
			str.erase(str.find(charsToRemove[i]), sizeof(char));
		}
	}
}

void SpanTreeGenerator::changeNodeState(nodeSyntaxState& state, commentSyntaxState& commentState, char charItem)
{
	switch (state)
	{
	case nodeSyntaxState::invalid:
		return;
		break;
	case nodeSyntaxState::start:
		if (charItem == ' ' || charItem == '\t')
			break;
		else if (isalpha(charItem))
			state = nodeSyntaxState::nodeName;
		else
			state = nodeSyntaxState::invalid;
		break;
	case nodeSyntaxState::nodeName:
		if (isalnum(charItem))
			break;
		else if (charItem == '=')
			state = nodeSyntaxState::equalsign;
		else
			state = nodeSyntaxState::invalid;
		break;
	case nodeSyntaxState::equalsign:
		if (isdigit(charItem))
			state = nodeSyntaxState::nodeCost;
		else
			state = nodeSyntaxState::invalid;
		break;
	case nodeSyntaxState::nodeCost:
		if (isdigit(charItem))
			break;
		else if (charItem == ';')
			state = nodeSyntaxState::end;
		else
			state = nodeSyntaxState::invalid;
		break;
	case nodeSyntaxState::end:
		if (!(charItem == ' ' || charItem == '\t' || charItem == '\n' || charItem == '/') || commentState == commentSyntaxState::firstBacksslash)
			state = nodeSyntaxState::invalid;
		break;
	default:
		break;
	}
}

void SpanTreeGenerator::changeLinkState(linkSyntaxState& state, commentSyntaxState& commentState, char charItem)
{

	switch (state)
	{
	case linkSyntaxState::invalid:
		return;
		break;
	case linkSyntaxState::start:
		if (charItem == ' ' || charItem == '\t')
			break;
		else if (isalpha(charItem))
			state = linkSyntaxState::node1Name;
		else
			state = linkSyntaxState::invalid;
		break;
	case linkSyntaxState::node1Name:
		if (isalnum(charItem))
			break;
		else if (charItem == '-')
			state = linkSyntaxState::dash;
		else
			state = linkSyntaxState::invalid;
		break;
	case linkSyntaxState::dash:
		if (isalpha(charItem))
			state = linkSyntaxState::node2Name;
		else
			state = linkSyntaxState::invalid;
		break;
	case linkSyntaxState::node2Name:
		if (isalnum(charItem))
			break;
		else if (charItem == ':')
			state = linkSyntaxState::doubleDot;
		else
			state = linkSyntaxState::invalid;
		break;
	case linkSyntaxState::doubleDot:
		if (isdigit(charItem))
			state = linkSyntaxState::cost;
		else
			state = linkSyntaxState::invalid;
	case linkSyntaxState::cost:
		if (isdigit(charItem))
			break;
		else if (charItem == ';')
			state = linkSyntaxState::end;
		else
			state = linkSyntaxState::invalid;
		break;
	case linkSyntaxState::end:
		if (!(charItem == ' ' || charItem == '\t' || charItem == '\n' || charItem == '/') || commentState == commentSyntaxState::firstBacksslash)
			state = linkSyntaxState::invalid;
		break;
	default:
		break;
	}
}

void SpanTreeGenerator::changeCommentState(commentSyntaxState& state, char charItem)
{
	switch (state)
	{
	case commentSyntaxState::noComment:
		if (charItem == '/')
			state = commentSyntaxState::firstBacksslash;
		break;
	case commentSyntaxState::firstBacksslash:
		if (charItem == '/')
			state = commentSyntaxState::comment;
		break;
	case commentSyntaxState::comment:
		break;
	default:
		break;
	}
}