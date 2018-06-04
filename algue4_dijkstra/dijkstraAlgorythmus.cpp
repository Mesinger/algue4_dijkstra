#include "dijkstraAlgorythmus.h"

TStatMap::const_iterator mapIt;

void computeDijkstra(int source, adjacency_list_t &adjacencyList, TStatMap &stations, weight_t &weight_vec, index_t &index_vec)
{
    //nr of stations in adjacencylist
	int n = adjacencyList.size();

	//init weight vec with INT_MAX, every weight_vec element represents a distance from the start to the station by index
	weight_vec.clear();
	weight_vec.resize(n, INT_MAX);

	//source weight is 0(it takes 0 min from A to A)
	weight_vec[source] = 0;

	//index_vec stores the previous stations by id, means(From A to B) weight_vec(index_b) = dist, index_vec(index_b) = index_a
	index_vec.clear();
	index_vec.resize(n, -1);

	//priority que stores current used stations, set autosorts elements after their distance(asc)
	std::set<std::pair<int, int> > priority_queue;	//<distance,index>
	priority_queue.insert(std::make_pair(weight_vec[source], source));

	while (!priority_queue.empty())
	{
	    //do this only once for each node, after we erased it, it wont be added anymore, shortest distance already found
		int dist = priority_queue.begin()->first;
		int u = priority_queue.begin()->second;
		priority_queue.erase(priority_queue.begin());

		//loop through each neighbor station of the current node
        //executed twice for each kante in the graph
		const std::vector<Node> &neighbors = adjacencyList[u];
		for (std::vector<Node>::const_iterator neighbor_iter = neighbors.begin();
			neighbor_iter != neighbors.end();
			neighbor_iter++)
		{
			string str = neighbor_iter->stationname;
			mapIt = Test(stations, str);
			if (mapIt != stations.end())
			{
			    //check for new shorter path, if set according and add to the priority_queue the updated pair
				int v = mapIt->second;
				int weight = neighbor_iter->distance;
				int distance_through_u = dist + weight;

				if (distance_through_u < weight_vec[v]) {
					priority_queue.erase(std::make_pair(weight_vec[v], v));

					weight_vec[v] = distance_through_u;
					index_vec[v] = u;
					priority_queue.insert(std::make_pair(weight_vec[v], v));
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

void printTrip(std::list<int> &path, TStatMap &stations, int weight) {

    std::cout << "Shortest trip: " << std::endl;

    std::string startstation, endstation;
    for(auto pathit = path.begin(); pathit != path.end(); ++pathit){
        for(auto stationsit = stations.begin(); stationsit != stations.end(); ++stationsit){

            if (stationsit->second == *pathit)//station id in path => print station
            {
                std::cout << stationsit->first << " ";
                if (pathit == path.begin())
                    startstation = stationsit->first;
                endstation = stationsit->first;
            }
        }
    }

    std::cout << endl << "Time from "<< startstation << " to " << endstation << " is " << weight << " minutes" << std::endl << std::endl;
}
