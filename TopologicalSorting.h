#pragma once
#include "Graph.h"
class TopologicalSorting
{
public:
	static std::vector<int> TopologicalSort(Graph& graph);


private:
	static bool IsCyclic(const std::vector<std::vector<int>>& adjacencyMatrix);
	static std::vector<int> TopologicalSort(const std::vector<std::vector<int>>& adjacencyMatrix);
};

