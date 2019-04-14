#pragma once
#include <queue>
#include <stack>
#include "HelpData.h"

bool bfs(const std::vector<DNode>& graph, uint16_t s, uint16_t t, std::vector<std::pair<uint16_t, uint16_t>>& path)
{
	std::vector<bool> visited(graph.size()); // use bitset container because in general it's better in terms of performance (because of cache-friendly)
	std::queue<uint16_t> q;
	q.push(s);
	visited[s] = true;

	while (!q.empty())
	{
		uint16_t u = q.front();
		q.pop();

		bool visitedT = false;
		const DNode& node = graph[u];
		for (size_t i = 0; i < node.children.size(); ++i)
		{
			uint16_t v = node.children[i];
			if (!visited[v] && node.flow[i] > 0)
			{
				visited[v] = true;
				path[v] = std::make_pair(u, i);
				q.push(v);
				visitedT = v == t;
				if (visitedT)
					return true;
			}
		}
	}

	return visited[t];
}


void _declspec(noinline)  EdmondsKarp(std::vector<DNode>& rGraph)
{
	uint16_t s = 0;
	uint16_t t = rGraph.size() - 1;

	std::vector<std::pair<uint16_t, uint16_t>> path(rGraph.size());
	uint16_t pathSize;

	uint32_t maxFlow = 0;
	// find path in the residual graph
	while (bfs(rGraph, s, t, path))
	{
		// find path flow capacity
		uint8_t pathFlow = UINT8_MAX;
		for (uint16_t v = t; v != s;)
		{
			uint16_t u = path[v].first;
			uint16_t iToV = path[v].second;
			if (pathFlow > rGraph[u].flow[iToV])
			{
				pathFlow = rGraph[u].flow[iToV];
			}
			v = u;
		}

		// update the residual graph
		for (uint16_t v = t; v != s;)
		{
			// flow from u to v
			uint16_t u = path[v].first;
			uint16_t iToV = path[v].second;
			rGraph[u].flow[iToV] -= pathFlow;

			uint16_t iToU = rGraph[u].revertIndex[iToV];
			rGraph[v].flow[iToU] += pathFlow;
			v = u;
		}

		maxFlow += pathFlow;
	}
}

bool dfs(const std::vector<DNode>& graph, uint16_t s, uint16_t t, std::vector<std::pair<uint16_t, uint16_t>>& path)
{
	std::vector<bool> visited(graph.size()); // use bitset container because in general it's better in terms of performance (because of cache-friendly)
	std::stack<uint16_t> q;
	q.push(s);
	

	bool visitedT = false;
	while (!q.empty())
	{
		uint16_t u = q.top();
		q.pop();
		if (visited[u])
		{
			continue;
		}

		visited[u] = true;


		const DNode& node = graph[u];
		for (size_t i = 0; i < node.children.size(); ++i)
		{
			uint16_t v = node.children[i];
			if (!visited[v] && node.flow[i] > 0)
			{
				path[v] = std::make_pair(u, i);
				q.push(v);
				visitedT = v == t;
				if (visitedT)
					return visitedT;
			}
		}

	}

	return visited[t];
}


void _declspec(noinline)  FordFulkerson(std::vector<DNode>& rGraph)
{
	uint16_t s = 0;
	uint16_t t = rGraph.size() - 1;

	std::vector<std::pair<uint16_t, uint16_t>> path(rGraph.size());

	uint32_t maxFlow = 0;
	// find path in the residual graph
	while (dfs(rGraph, s, t, path))
	{
		// find path flow capacity
		uint8_t pathFlow = UINT8_MAX;
		for (uint16_t v = t; v != s;)
		{
			uint16_t u = path[v].first;
			uint16_t iToV = path[v].second;
			if (pathFlow > rGraph[u].flow[iToV])
			{
				pathFlow = rGraph[u].flow[iToV];
			}
			v = u;
		}

		// update the residual graph
		for (uint16_t v = t; v != s;)
		{
			// flow from u to v
			uint16_t u = path[v].first;
			uint16_t iToV = path[v].second;
			rGraph[u].flow[iToV] -= pathFlow;

			uint16_t iToU = rGraph[u].revertIndex[iToV];
			rGraph[v].flow[iToU] += pathFlow;
			v = u;
		}

		maxFlow += pathFlow;
	}
}


bool dBfs(const std::vector<DNode>& graph, uint16_t s, uint16_t t, std::vector<uint16_t>& levels)
{
	std::memset(levels.data(), UINT16_MAX, levels.size() * sizeof(uint16_t));
	
	std::queue<uint16_t> q;
	q.push(s);

	levels[s] = 0;
	while (!q.empty())
	{
		uint16_t u = q.front();
		q.pop();

		const DNode& node = graph[u];
		for (size_t i = 0; i < node.children.size(); ++i)
		{
			uint16_t v = node.children[i];
			if (levels[v] == UINT16_MAX && node.flow[i] > 0)
			{
				levels[v] = levels[u] + 1;
				q.push(v);
				if (v == t)
					return true;
			}
		}
	}

	return (levels[t] != UINT16_MAX);
}

bool sendFlow(const std::vector<DNode>& graph, uint16_t s, uint16_t t, const std::vector<uint16_t>& levels, std::vector<uint16_t>& iBegin, std::vector<std::pair<uint16_t, uint16_t>>& path)
{
	std::stack<uint16_t> q;
	q.push(s);

	
	bool visitedT = false;
	while (!q.empty())
	{
		uint16_t u = q.top();
		q.pop();

		const DNode& node = graph[u];
		for (; iBegin[u] < node.children.size(); ++iBegin[u])
		{
			uint16_t i = iBegin[u];
			uint16_t v = node.children[i];
			if (levels[v] == levels[u]+1 && node.flow[i] > 0)
			{
				path[v] = std::make_pair(u, i);
				q.push(v);
				visitedT = v == t;
				if (visitedT)
					return visitedT;
			}
		}
	}
	return false;
}




void _declspec(noinline)  Dinic(std::vector<DNode>& rGraph)
{
	uint16_t s = 0;
	uint16_t t = rGraph.size() - 1;

	std::vector<uint16_t> levels(rGraph.size());
	std::vector<uint16_t> iBegin(rGraph.size());
	std::vector<std::pair<uint16_t, uint16_t>> path(rGraph.size());
	uint32_t maxFlow = 0;
	// find path in the residual graph


	while (dBfs(rGraph, s, t, levels))
	{
		std::memset(iBegin.data(), 0, iBegin.size() * sizeof(uint16_t));
		while (sendFlow(rGraph, s, t, levels, iBegin, path))
		{
			// find path flow capacity
			uint8_t pathFlow = UINT8_MAX;
			for (uint16_t v = t; v != s;)
			{
				uint16_t u = path[v].first;
				uint16_t iToV = path[v].second;
				if (pathFlow > rGraph[u].flow[iToV])
				{
					pathFlow = rGraph[u].flow[iToV];
				}
				v = u;
			}

			// update the residual graph
			for (uint16_t v = t; v != s;)
			{
				// flow from u to v
				uint16_t u = path[v].first;
				uint16_t iToV = path[v].second;
				rGraph[u].flow[iToV] -= pathFlow;

				uint16_t iToU = rGraph[u].revertIndex[iToV];
				rGraph[v].flow[iToU] += pathFlow;
				v = u;
			}

			maxFlow += pathFlow;
		}
	}
}



