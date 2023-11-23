#include "Graph.h"

Graph::Graph()
{
}

std::vector<Node> Graph::getNodes() const
{
	return m_nodes;
}

std::vector<Arch> Graph::getArches() const
{
	return m_arches;
}

void Graph::SetNodes(const std::vector<Node>& nodes)
{
	m_nodes = nodes;
}

std::vector<std::vector<int>> Graph::GetAdjacencyMatrix() const
{
	return m_adjacencyMatrix;
}

void Graph::addNode(Node node)
{
	m_nodes.push_back(node);
	createNewSpace();
}

void Graph::addNode(QPoint coordinate)
{
	Node node;
	node.setValue(m_nodes.size());
	node.setCoordinates(coordinate);
	m_nodes.push_back(node);
	createNewSpace();
}

void Graph::addArch(Arch arch, bool typeOfGraph)
{
	if (typeOfGraph && validateDirectedArch(arch))
	{
		m_arches.push_back(arch);
		createNewDirectedArch(arch);
	}
	else if (!typeOfGraph && validateNonDirectedArch(arch))
	{
		m_arches.push_back(arch);
		createNewNonDirectedArch(arch);
	}
}


void Graph::addArch(Node firstNode, Node secondNode, bool typeOfGraph)
{
	Arch arch(firstNode, secondNode);
	if (typeOfGraph && validateDirectedArch(arch))
	{
		m_arches.push_back(arch);
		createNewDirectedArch(arch);
	}
	else if (!typeOfGraph && validateNonDirectedArch(arch))
	{
		m_arches.push_back(arch);
		createNewNonDirectedArch(arch);
	}
}

void Graph::createNewSpace()
{
	std::vector<int> newCollom(m_adjacencyMatrix.size() + 1, 0);
	if (!m_adjacencyMatrix.empty())
	{
		for (auto& i : m_adjacencyMatrix)
		{
			i.push_back(0);
		}
	}
	m_adjacencyMatrix.emplace_back(newCollom);
	updateFile();
}

void Graph::createNewNonDirectedArch(Arch arch)
{
	m_adjacencyMatrix[arch.getFirstNode().getValue()][arch.getSecondNode().getValue()] = 1;
	m_adjacencyMatrix[arch.getSecondNode().getValue()][arch.getFirstNode().getValue()] = 1;
	updateFile();
}

bool Graph::IsEmpty() const
{
	return m_nodes.empty();
}

void Graph::createNewDirectedArch(Arch arch)
{
	m_adjacencyMatrix[arch.getFirstNode().getValue()][arch.getSecondNode().getValue()] = 1;
	updateFile();
}

bool Graph::validateDirectedArch(Arch arch)
{
	for (auto aux : m_arches)
	{
		if (aux.getFirstNode().getValue() == arch.getFirstNode().getValue()
			&& aux.getSecondNode().getValue() == arch.getSecondNode().getValue())
		{
			return false;
		}
	}
	return true;
}

bool Graph::validateNonDirectedArch(Arch arch)
{
	for (auto aux : m_arches)
	{
		if ((aux.getFirstNode().getValue() == arch.getFirstNode().getValue()
				&& aux.getSecondNode().getValue() == arch.getSecondNode().getValue())
			|| (aux.getFirstNode().getValue() == arch.getSecondNode().getValue()
				&& aux.getSecondNode().getValue() == arch.getFirstNode().getValue()))
			return false;
	}
	return true;
}


void Graph::updateFile()
{
	std::ofstream fileOut("adjacencyMatrix.txt", std::ios::trunc);
	if (fileOut.is_open())
	{
		fileOut << m_nodes.size() << std::endl;
		for (auto aux : m_adjacencyMatrix)
		{
			for (auto aux2 : aux)
				fileOut << aux2 << " ";
			fileOut << std::endl;
		}
	}
}
