# Dijkstras Algorithm

## Function Definition:
```
void computeDijkstra(int source, adjacency_list_t &adjacencyList, TStatMap &stations, weight_t &weight_vec, index_t &index_vec)
```
Our implementation takes 4 parameters
- int source: an id, used to find the correct node in our containers
- adjacencyList: this list stores for each node in the graph, their neighbors and weight
- __OUT weight_vec: this container will store the individual weight, from source, to every other node in the graph
- __OUT index_vec: this will store the previous node to take, if you want to achieve the shortest possible distance

## Initialization of data structures

First, we have to get our output containers to an default state. We will set our weights to INT_MAX, our indexs to -1

```
weight_vec.clear();
weight_vec.resize(n, INT_MAX);

index_vec.clear();
index_vec.resize(n, -1);
```

After that, we only need some sort of a queue, to store current used nodes in our algorithm. In this case, we use
```
std::set<std::pair<int, int>> priority_queue;	//<distance,index>
```
which stores a pair of two ints(distance and index of a node). A set is implemented as a binary search tree, which means O(log(n)) erase/insert and sorts its elements internally.
In our case, our pairs will be sorted as their distances(asc), which we need in our algorithm.

## Dijkstras Algorithm

```
while (!priority_queue.empty())
```
will be executed only, and only once for each node in the graph. Why, because our set sorts our pairs of <distance, index> as their distance.
If we remove the first element from the set, we will remove the element with the smallest distance. And this node will not be added to the 
set anymore, because it this the minimum distance from source to node.
```
int dist = priority_queue.begin()->first;
int u = priority_queue.begin()->second;
priority_queue.erase(priority_queue.begin());
```
After that, we will check all neighbor nodes of our deleted node, and update their weight_vec/index_vec if we found a shorter path than the previous.

Whats the complexity of the inner loop?
It is E (E = number of directed edges).

Why? Because, as mentioned above, the outer loop will be called once for each node, meaning you will check all (directed) edges only once.

## Complexity
The outer loop will be called N times (N ... number of nodes), and will erase one element of the priority_queue each time
```
O(N * log(N))
```
The inner loop will be called E times (E ... number of edges), and will earse/insert elements.
```
O(E * log(N))
```
Combined
```
O((N+E) * log(N))
```
