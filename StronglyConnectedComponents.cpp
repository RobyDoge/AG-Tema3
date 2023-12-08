#include "StronglyConnectedComponents.h"
#include <stack>
#include <algorithm>
#include <algorithm>
std::list<std::list<int>> StronglyConnectedComponents::CreateStronglyConnectedComponents(const Graph& graph)
{
	const std::vector<int> t2FirstSearch{ DepthFirstSearch(graph.GetAdjacencyMatrix()) };
    const Graph newGraph{ InverseGraph(graph) };
    return Sort(DepthFirstSearch(newGraph.GetAdjacencyMatrix(), t2FirstSearch));
}

std::vector<int> StronglyConnectedComponents::DepthFirstSearch(const std::vector<std::vector<int>>& adjacencyMatrix)
{
    std::stack<int> v;
    v.push(0);
    std::list<int> w;
    std::vector<int> u(adjacencyMatrix.size() - 1);
    std::iota(u.begin(), u.end(), 1);
    std::vector<int> p(adjacencyMatrix.size(), 0);
    std::vector<int> t1(adjacencyMatrix.size(), -1);
    std::vector<int> t2(adjacencyMatrix.size(), -1);
    int t = 1;

    t1[0] = t;
    while (w.size() != adjacencyMatrix.size())
    {
        while (!v.empty())
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
                    t1[currentNode] = ++t;
                    verify = false;
                    break;
                }
                ++aux;
            }
            if (verify)
            {
                w.push_front(v.top());
                t2[v.top()] = ++t;
                v.pop();
            }
        }
        if (!u.empty())
        {
            auto aux = u.begin();
            const int currentNode = *aux;
            aux = u.erase(aux);
            v.push(currentNode);
            t1[currentNode] = ++t;
        }
    }
    return t2;
}

Graph StronglyConnectedComponents::InverseGraph(const Graph& graph)
{
	Graph newGraph;
	for(const auto& node :graph.getNodes())
	{
		newGraph.addNode(node);
	}
	for(const auto& arch :  graph.getArches())
	{
		newGraph.addArch(arch.getSecondNode(), arch.getFirstNode(), true);
	}
	return newGraph;
}

std::list<std::list<int>> StronglyConnectedComponents::DepthFirstSearch(
	const std::vector<std::vector<int>>& adjacencyMatrix,
	const std::vector<int>& t2FirstSearch)
{

    std::list<int> w;
    std::vector<int> p(adjacencyMatrix.size(), 0);
    std::vector<int> t1(adjacencyMatrix.size(), -1);
    std::vector<int> t2(adjacencyMatrix.size(), -1);
    int t = 1;
    std::stack<int> v{};
    std::vector<int> u{};
    std::list<std::list<int>> stronglyConnectedComponents{};

    int maxIndex=-1;
    int maxT2=-1;
    for(int i=0;i<adjacencyMatrix.size();i++)
    {
	    if(t2FirstSearch[i]>maxT2)
	    {
            maxT2 = t2FirstSearch[i];
            maxIndex = i;
	    }
    }
    v.push(maxIndex);
    t1[maxIndex] = t;
    for(int i = 0; i < adjacencyMatrix.size(); i++)
    {
        if (i != maxIndex)
        {
	        u.push_back(i);
        }
    }
   
    while ( w.size() != adjacencyMatrix.size())
    {
        std::list<int> auxList{};
        while (!v.empty())
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
                    t1[currentNode] = ++t;
                    verify = false;
                    break;
                }
                ++aux;
            }
            if (verify)
            {
                w.push_front(v.top());
                t2[v.top()] = ++t;
                auxList.emplace_front(v.top());
                v.pop();
            }
        }
        stronglyConnectedComponents.emplace_front(auxList);
        if (!u.empty())
        {
            maxT2 = -1;
            for (const auto& uElement : u)
            {
                if (t2FirstSearch[uElement] > maxT2)
                {
                    maxT2 = t2FirstSearch[uElement];
                    maxIndex = uElement;
                }
            }
            auto aux =std::find(u.begin(),u.end(),maxIndex);
            const int currentNode = *aux;
            aux = u.erase(aux);
            v.push(currentNode);
            t1[currentNode] = ++t;
        }
    }
    return {stronglyConnectedComponents};
}

std::list<std::list<int>> StronglyConnectedComponents::Sort(const std::list<std::list<int>>& list)
{
    std::list<std::list<int>> newList;
	for(auto aux : list)
	{
        aux.sort();
        newList.push_back(aux);
	}
    newList.sort([](const std::list<int>& aux1, const std::list<int>& aux2) {
        return aux1.front() < aux2.front(); 
    });
    return newList;
}
