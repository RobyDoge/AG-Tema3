#pragma once
#include "Graph.h"
class StronglyConnectedComponents
{
public:
	
	static std::list<std::list<int>> CreateStronglyConnectedComponents(const Graph& graph);

private:
	static std::vector<int> DepthFirstSearch(const std::vector<std::vector<int>>& adjacencyMatrix);
	static Graph InverseGraph(const Graph& graph);
	static std::list<std::list<int>> DepthFirstSearch(const std::vector<std::vector<int>>& adjacencyMatrix,
	                                                  const std::vector<int>& t2FirstSearch);
	static std::list<std::list<int>> Sort(const std::list<std::list<int>>& list);

};

