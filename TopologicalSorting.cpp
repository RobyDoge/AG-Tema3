#include "TopologicalSorting.h"
#include <vector>
#include <stack>
#include <numeric>
#include <list>

std::vector<int> TopologicalSorting::TopologicalSort(Graph& graph)
{
    if (IsCyclic(graph.GetAdjacencyMatrix()))
    {
        return {};
    }
    return TopologicalSort(graph.GetAdjacencyMatrix());
}


bool TopologicalSorting::IsCyclic(const std::vector<std::vector<int>>& adjacencyMatrix)
{
	const int numNodes = adjacencyMatrix.size();
    std::vector<int> visited(numNodes, 0); // 0: unvisited, 1: visiting, 2: visited
    std::stack<int> nodesStack;

    for (int i = 0; i < numNodes; ++i) 
    {
        if (visited[i] == 0) {
            nodesStack.push(i);
            visited[i] = 1;

            while (!nodesStack.empty()) 
            {
	            const int currentNode = nodesStack.top();
                bool hasUnvisitedNeighbor = false;

                for (int j = 0; j < numNodes; ++j) 
                {
                    if (adjacencyMatrix[currentNode][j] == 1) 
                    {
                        if (visited[j] == 1) 
                        {
                            return true;
                        }
                        if (visited[j] == 0) 
                        {
                            nodesStack.push(j);
                            visited[j] = 1;
                            hasUnvisitedNeighbor = true;
                            break;
                        }
                    }
                }

                if (!hasUnvisitedNeighbor) {
                    visited[currentNode] = 2;
                    nodesStack.pop();
                }
            }
        }
    }

    return false;

}

std::vector<int> TopologicalSorting::TopologicalSort(const std::vector<std::vector<int>>& adjacencyMatrix)
{
    std::stack<int> v;
    v.push(0);
    std::list<int> w;
    std::vector<int> u(adjacencyMatrix.size()-1);
    std::iota(u.begin(), u.end(), 1);
    std::vector<int> p(adjacencyMatrix.size(),0);
    std::vector<int> t1(adjacencyMatrix.size(), -1);
    std::vector<int> t2(adjacencyMatrix.size(), -1);
    int t = 1;

    t1[0] = t;
    while(w.size()!=adjacencyMatrix.size())
    {
	    while(!v.empty())
	    {
            bool verify = true;
            auto aux = u.begin();
            while (aux != u.end())
            {
                if (adjacencyMatrix[v.top()][*aux] == 1)
                {
                    v.push(*aux);
                    const int currentNode = *aux;
                    aux = u.erase(aux);
                    p[currentNode] = v.top();
                    t1[currentNode]= ++t;
                    verify = false;
                    break;
                }
            	++aux;
            }
            if(verify)
            {
	            w.push_front(v.top());
            	t2[v.top()] = ++t;
            	v.pop();
            }
	    }
        if(!u.empty())
        {
            auto aux = u.begin();
            const int currentNode = *aux;
            aux = u.erase(aux);
            v.push(currentNode);
            t1[currentNode]=++t;
        }
    }
    return{w.begin(),w.end()};
}
