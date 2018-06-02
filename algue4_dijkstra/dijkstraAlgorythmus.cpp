#include "dijkstraAlgorythmus.h"
#include "findinmap.h"

TStatMap::const_iterator mapIt;

void computeDijkstra(int source, adjacency_list_t &adjacencyList, TStatMap &map, weight_t &weight_vec, index_t &index_vec)
{
	int n = adjacencyList.size();
	weight_vec.clear();
	weight_vec.resize(n, INT_MAX);
	weight_vec[source] = 0;
	index_vec.clear();
	index_vec.resize(n, -1);
	std::set<std::pair<int, int> > vertex_queue;	//<distance,index>
	vertex_queue.insert(std::make_pair(weight_vec[source], source));

	while (!vertex_queue.empty())
	{
		int dist = vertex_queue.begin()->first;
		int u = vertex_queue.begin()->second;
		vertex_queue.erase(vertex_queue.begin());

		// Visit each edge exiting u
		const std::vector<Node> &neighbors = adjacencyList[u];
		for (std::vector<Node>::const_iterator neighbor_iter = neighbors.begin();
			neighbor_iter != neighbors.end();
			neighbor_iter++)
		{
			string str = neighbor_iter->stationname;
			mapIt = Test(map, str);
			if (mapIt != map.end())
			{
				int v = mapIt->second;
				int weight = neighbor_iter->distance;
				int distance_through_u = dist + weight;

				if (distance_through_u < weight_vec[v]) {
					vertex_queue.erase(std::make_pair(weight_vec[v], v));

					weight_vec[v] = distance_through_u;
					index_vec[v] = u;
					vertex_queue.insert(std::make_pair(weight_vec[v], v));
				}
			}
		}
	}
}

std::list<int> DijkstraGetShortestPathTo(
	int index, index_t &index_vec)
{
	std::list<int> path;
	for (; index != -1; index = index_vec[index])
		path.push_front(index);
	return path;
}