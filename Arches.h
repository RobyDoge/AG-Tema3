#pragma once
#include "Node.h"
class Arch
{
public:
	Arch();
	Arch(Node firstNode, Node secondNode);

	void setFirstNode(Node firstNode);
	void setSecondNode(Node secondNode);

	Node getFirstNode() const;
	Node getSecondNode() const;


private:
	Node m_firstNode;
	Node m_secondNode;
};

