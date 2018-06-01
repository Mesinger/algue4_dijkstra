#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include "Node.h"

template<typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

using namespace std;

typedef std::vector<std::vector<Node>> adjacency_list_t;
typedef pair<string, int> TStatPair;
typedef map<string, int> TStatMap;

//Suchfunktion um Stationen in der Map zu finden
TStatMap::const_iterator FindPrefix(const TStatMap& map, const string& search_for) {
	TStatMap::const_iterator i = map.lower_bound(search_for);
	if (i != map.end()) {
		const string& key = i->first;
		if (key.compare(0, search_for.size(), search_for) == 0) // Really a prefix?
			return i;
	}
	return map.end();
}

//"Debugfunktion" zur Suche
TStatMap::const_iterator Test(const TStatMap& map, const string& search_for) {
	auto i = FindPrefix(map, search_for);
	if (i != map.end())
	{
		cout << "Schon da: " << i->first << ", " << i->second << endl;
		return i;
	}
	else
		return map.end();
}

int main(int argc, char** argv) {
	
	/*command line arguments*/
	if (argc != 2) {
		std::cerr << "Invalid arguments" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input(argv[1]);

	if (input.fail()) {
		std::cerr << "Invalid file" << std::endl;
		return EXIT_FAILURE;
	}

	std::string sbuf;
	std::vector<std::vector<std::string>> linestations;
    std::vector<std::vector<int>> linedistances;
	adjacency_list_t adjacency_list;
	int adjsize = 0;
	TStatMap statmap;//pair<statname,number adj.list>
	vector<string> LineArray;

	std::vector<int> distances;
	std::vector<std::string> stations;
	int dist_to_next, dist_to_prev;
	string stat_next, stat_prev, stat_zw, statraw, searchstring;
	std::string linenumber;//LinienNummer

	while (getline(input, sbuf)) {

		dist_to_next = 0, dist_to_prev = 0;
		stat_next="", stat_prev="", stat_zw="", statraw="", searchstring="";

		linenumber = sbuf.substr(0, sbuf.find(':'));
		LineArray.emplace_back(linenumber);
		sbuf = sbuf.substr(sbuf.find(':') + 3);	//herausschneiden der Liniennummer aus dem File

		std::vector<std::string> line = split(sbuf, '"');	//Anzahl der Stationen in dieser Linie

		for (int i = 0; i < line.size(); i++) {

			try {

				//distance
				distances.emplace_back(stoi(line[i]));	//Versuch zum herauslesen der Distanz
														//wenn nicht m�glich -> Stationsname
				dist_to_prev = dist_to_next;
				dist_to_next = stoi(line[i]);
				statmap.insert(TStatPair(statraw, adjsize));
				distances.emplace_back(stoi(line[i]));

			}
			catch (const std::invalid_argument& e) {

				//station
				stations.emplace_back(line[i] + ":" + linenumber);
				stat_prev = stat_zw;
				stat_zw = stat_next;
				stat_next = line[i] + ":" + linenumber;
				statraw = line[i] + ":" + linenumber;
				searchstring = line[i] + ":";
			}

			if (i % 2 == 0 && i != 0)
			{
				adjacency_list.emplace_back();
				if (stat_next != "")
					adjacency_list[adjsize].emplace_back(stat_next, dist_to_next);//N�chste Station im Bezug auf die derzeitige eintragen
				if (stat_prev != "")
					adjacency_list[adjsize].emplace_back(stat_prev, dist_to_prev);//Vorherige Station im Bezug auf die derzeitige eintragen
				adjsize++;
			}
		}
		adjacency_list.emplace_back();	//Letzte Station hinzuf�gen
		adjacency_list[adjsize].emplace_back(stat_zw, dist_to_next);
		statmap.insert(TStatPair(statraw, adjsize));
		adjsize++;

		linestations.emplace_back(stations);	//[0][x]->erste Linie Stationen, [1][x]->zweite Linie Stationen,.....
		linedistances.emplace_back(distances);	//[0][x]->Distanzen erste Linie, [1][x]->Distanzen zweite Linie

		while (adjacency_list.size() > adjsize + 1)//�berfl�ssige (leere) Eintr�ge im Vektor l�schen
		{
			adjacency_list.pop_back();
		}

		linestations.emplace_back(stations);
		linedistances.emplace_back(distances);
	}


	//Adjaceny-List Verkn�pfungen erstellen bei selber Station mit mehreren Linien :)
	TStatMap::const_iterator it = statmap.begin();
	TStatMap::const_iterator there;
	TStatMap::const_iterator end = statmap.end();
	vector<string>::const_iterator lineIt;
	vector<string>::const_iterator lineEnd = LineArray.end();
	std::vector<std::vector<Node>>::const_iterator AdjIt;
	std::vector<std::vector<Node>>::const_iterator AdjEnd = adjacency_list.end();
	std::vector<Node>::const_iterator NodeIt;
	std::vector<Node>::const_iterator NodeEnd;

	while (it != end)
	{
		lineIt = LineArray.begin();
		string station = it->first.substr(0, it->first.find(':'));
		while (lineIt != lineEnd)
		{
			string str = station + ":" + *lineIt;
			there = statmap.find(str);
			if (there != end && there != it)
			{
				//cout << it->second << endl;
				adjacency_list[it->second].emplace_back(str,5);
				//Noch zu machen: in node als nachbar setzen
				for (auto& row : adjacency_list)
				{
					for (auto& node : row)
					{
						if (node.getName() == it->first)
							node.appendNeben(str);
					}
				}
			}
			lineIt++;
		}
		
		it++;
	}

	input.close();

    if(linestations.size() != linedistances.size()){
        std::cerr << "Invalid file" << std::endl;
        return EXIT_FAILURE;
    }

    for(std::vector<Node> line : adjacency_list){
        for(Node station : line)
            station.displayInfo();
    }

	std::cin.ignore();
	std::cin.get();

    return EXIT_SUCCESS;
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}