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
	TStatMap statmap;	//pair<statname,number adj.list>

    while(getline(input, sbuf)){

        std::vector<int> distances;
        std::vector<std::string> stations;
		int dist_to_next=0,dist_to_prev=0;
		string stat_next,stat_prev,stat_zw,statraw,searchstring;

        std::string linenumber;//LinienNummer

        linenumber = sbuf.substr(0, sbuf.find(':'));
        sbuf = sbuf.substr(sbuf.find(':') + 3);	//herausschneiden der Liniennummer aus dem File

        std::vector<std::string> line = split(sbuf, '"');	//Anzahl der Stationen in dieser Linie

        for(int i = 0; i < line.size(); i++){

            try{

                //distance
                distances.emplace_back(stoi(line[i]));	//Versuch zum herauslesen der Distanz
														//wenn nicht möglich -> Stationsname
				dist_to_prev = dist_to_next;
				dist_to_next = stoi(line[i]);
				statmap.insert(TStatPair(statraw,adjsize));
            }catch (const std::invalid_argument& e){

                //station
                stations.emplace_back(line[i] + ":" + linenumber);
				stat_prev = stat_zw;
				stat_zw = stat_next;
				stat_next = line[i] + ":" + linenumber;
				statraw = line[i] + ":" + linenumber;
				searchstring = line[i]+":";
            }

			if (i%2==0 && i!=0)
			{
				int k = 0;
				string str;
				auto ret = Test(statmap, searchstring);	//Suche ob Station schon vorhanden 
				//(Daweil nur mit max. 2 Linien pro Station gültig)
				if (ret != statmap.end())//2te Linie auf selbe Station zb Praterstern
				{
					str = ret->first;
					int itr = ret->second;
					adjacency_list[itr].emplace_back(stat_next, 5);	//Nebenstation in aktueller Station setzen
					adjacency_list.emplace_back();
					adjacency_list.emplace_back();
					k = 1;
					//Nachbarnode in Node setzen
					vector< vector<Node> >::iterator start = adjacency_list.begin();
					start += itr;
					vector<Node>::iterator rowst = start->begin();
					vector<Node>::iterator rowen = start->end();
					for (; rowst != rowen; ++rowst)
					{
						size_t pos = str.find(":");

						if (str.substr(0,pos) == rowst->getName())	//Eintragen des Nachbarvektors in der jetzt aktuellen Node
							rowst->appendNeben(str);
					}

				}
				if(k!=1)
					adjacency_list.emplace_back();
				if(stat_next != "")
					adjacency_list[adjsize].emplace_back(stat_next,dist_to_next);//Nächste Station im Bezug auf die derzeitige eintragen
				if (stat_prev!= "")
				adjacency_list[adjsize].emplace_back(stat_prev, dist_to_prev);//Vorherige Station im Bezug auf die derzeitige eintragen
				adjsize++;
				if (k == 1)
				{
					adjacency_list[adjsize].emplace_back(str, 5);
					adjacency_list[adjsize][0].appendNeben(stat_next);//Eintragung des Nachbarvektors in der 1. Node für spätere Suche
					k = 0;
					if (adjacency_list.size() - adjsize >= 2)
						adjacency_list.pop_back();
				}
			}
        }
		adjacency_list.emplace_back();	//Letzte Station hinzufügen
		adjacency_list[adjsize].emplace_back(stat_zw, dist_to_next);

        linestations.emplace_back(stations);	//[0][x]->erste Linie Stationen, [1][x]->zweite Linie Stationen,.....
        linedistances.emplace_back(distances);	//[0][x]->Distanzen erste Linie, [1][x]->Distanzen zweite Linie
    }

	while (adjacency_list.size() > adjsize + 1)//Überflüssige (leere) Einträge im Vektor löschen
	{
		adjacency_list.pop_back();
	}

	input.close();

    if(linestations.size() != linedistances.size()){
        std::cerr << "Invalid file" << std::endl;
        return EXIT_FAILURE;
    }

	//adjacency_list_t adjacency_list;
	/*adjacency_list.reserve(linestations.size());

    for(int i = 0; i < linestations.size(); i++){

        //std::cout << linestations[i].size() << linedistances[i].size();

        if(linestations[i].size() != linedistances[i].size() + 1){
            std::cerr << "Invalid file" << std::endl;
            return EXIT_FAILURE;
        }

		adjacency_list.emplace_back();

        //first station
        adjacency_list[i].emplace_back(linestations[i][0], 0, linedistances[i].front());

        for(int j = 1; j < linestations[i].size() - 1; j++){

            adjacency_list[i].emplace_back(linestations[i][j], linedistances[i][j - 1], linedistances[i][j]);
        }

        //last station
        adjacency_list[i].emplace_back(linestations[i][0], linedistances[i].back(), 0);
    }
	*/

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