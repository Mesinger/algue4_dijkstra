#Installation
To run the programm you have to call it via commandline as following:
- Go into the directory "algue4_dijkstra/x64/Release" and open the command line window
- type: algue4_dijkstra.exe <Name of File with trainlines>

Then you should be able to enter your wished start and end destination


#Nodes

##What are our Nodes?
Our Nodes are basically objects of the class "node" which hold the information of the stationname as string and an distance value as integer
They are the key storing component of our programm

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


# Read inputfile

## Function definition:
Due to the fact, that the fileread is within the main() function there is no discrete read function, but it consists of 4 major parts:

- Opening the file and reading it line by line
- Combining the line name with the station name for later search purpose
- Setting the adjacencylist
(- Creating cross-references between the stations) --> is not within the reading loop

## Initialization of data structures
To be able to create the adjacencylist correctly, there are a number of variables nessecery in our solution, which come from the way we read in the inputfile
```
adjacency_list_t adjacency_list;
TStatMap statmap;//pair<statname,number adj.list>
vector<string> LineArray;
string stat_next, stat_prev, stat_zw, statraw, searchstring, linenumber, ring, sbuf;
int ringnum, adjsize = 0, dist_to_next, dist_to_prev;
```

This are the typedefinitions from the adj.list and station map "statmap":
```
typedef std::vector<std::vector<Node>> adjacency_list_t;
typedef std::map<std::string, int> TStatMap;
```
The adjacencylist is an vector array of our class "Node" wich holds the information of the name of the station and an distance parameter

Adjacencylist Structure:
[0]	- [0]- connected Node station1
[1]	- [0]- connected Node station0
	- [1]- connected Node station2
[2]	- [0]- connected Node station1
	- [1]- connected Node station3
...

As you can see does each station entry within the adjacencylist only hold the node entries for its connected stations and not itself. To know wich station is connected with each other the hashmap is needed.
it holds the name of the searched station and its number in the adjacency list:

Hashmap "statmap":
<station0:line0,0>
<station1:line0,1>
<station2:line0,2>

The hashmap is an string to integer map and is used to find the Nodeentries at a much higher speed O(log(n)) instead of O(n) with n as the total number of stations

the string parameters are needed to build the station names. A stationname has following structure:
<Name of the station>:<Name of the trainline>
this makes it possible to know everytime which line a station belongs to and is further needed for building the cross-references between lines in the same station

The LineArray string vector holds all names of lines which were read in.

## Reading the file and building the adjacencylist / station map 
The reading of the file needs basically c*(n+m) steps with c beeing the number of lines, n beeing the number ob stations within this line and m beeing the number of edges between the stations of this line read in.
This comes by how the input file is constructed

Example:
<Linename>: "Name_Station_0" distance0 "Name_Station1" distance1 "Name_Station2" ...

In our approach the adjacencylist is always written 1 "step" after the reading. this has the reason that if you wait for another reading cycle you have
always the 2 stations and the distance between them read in.
To be able to remember at which station you are currently at to write the parameter "adjsize" is used. it basicaly says in wich Node within the vector the station should be written

Because of the fact that we write 1 step after the read we have to write the last station outside the loop

## Complexity
The outer loop will be called C times with C beeing the number of lines to read
```
O(C)
```
The Inner loop is called N + M times with N beeing the number of stations and M beeing the number edges between the stations
```
O(N+M) --> O(N)
```
```
O(C*(N+M)) --> O(C*N)
```

!Performaceboost by combining hashmap with vector-array.
As an vector array always has an afford of O(N) to search an vector-entry in the badest case, we decided to extract the searching into a hashmap. this has the advantage that searching a value within hashmap
has the complexity of O(log(n)) which is a huge increase in performance, the more stations are needed.

#Creating Cross-References

##Why Cross-references were needed
Due to the fact that each station could have multiple lines attached to it, an the specification, that if you have to change the line you have to 
consider 5 minutes of walking time, we had to consider each line within a station as own node. Because the Dijkstra algorithm cannot handle additional information like this on its own (A Node beeing splitted up)

Example: Station A has 2 lines (l1,l2) --> A = A1,l1 --- 5 --- A2,l2 

##Establishing Cross-References
To be able to connect Node C with node W and vis versa which have the same stationname, but different lines you need to loop through the map which holds every stationname exactly one time.
Then you need to compare this sattionname without its line with the stationnames of the other nodes. if the name without the line is the same, you know that there needs to be a connection between them,
with a distance of 5 minutes

- Outer loop:
The outer loop goes through the map and loads each existing stationname-line combination that was read in exactly one time.
It is called N times with N beeing the total number of all stations read in --> O(N)

-Inner loop:
The inner loop goes through the string-vector "LineArray" which holds all lines that were read in. Because you do not know from here which station has multiple lines you have to combine the map-station
with each possible line and try to find it within the map. Due to the fact that the map holds the number of the node in the adjacencylist you can exactly tell where to put in the found second station.
If for example in your map you load the station S1:L1 and find the station S1:L2 (L1/2 beeing linennames), you can directly add S1:L2 as Node to the adjacencylist with the number stored in the maped for S1:L1
The inner loop is called M times with M beeing the number of lines read in.
Due to that the map has an search complexity of O(log(n)) and this is much smaller than O(N) at high amounts of stations, we can ignore these factor

## Complexity
Due to that fact that the outer loop calles the inner loop every time there is following complexity:
```
O(N*M*C)	
```
--> N - Number of Stations, M - number of lines, C - log(N) (binary tree search in map) 
--> O(N*M)
