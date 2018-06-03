#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include <iterator>
#include "Node.h"
#include "stringsplit.h"
#include "findinmap.h"
#include "dijkstraAlgorythmus.h"

using namespace std;

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


	adjacency_list_t adjacency_list;
	TStatMap statmap;//pair<statname,number adj.list>
	vector<string> LineArray;

	string ring;
	int ringnum;

	{
		int adjsize = 0;
		int dist_to_next, dist_to_prev;
		string stat_next, stat_prev, stat_zw, statraw, searchstring;
		std::string linenumber;//LinienNummer

		std::string sbuf;
		while (getline(input, sbuf)) {

			dist_to_next = 0, dist_to_prev = 0;
			stat_next="", stat_prev="", stat_zw="", statraw="", searchstring="";

			linenumber = sbuf.substr(0, sbuf.find(':'));
			LineArray.emplace_back(linenumber);
			sbuf = sbuf.substr(sbuf.find(':') + 3);	//herausschneiden der Liniennummer aus dem File

			std::vector<std::string> line = split(sbuf, '"');	//Anzahl der Stationen in dieser Linie

			for (int i = 0; i < line.size(); i++) {

				try {

					//distance	//Versuch zum herauslesen der Distanz
					//wenn nicht m�glich -> Stationsname
					dist_to_next = stoi(line[i]);
					dist_to_prev = dist_to_next;
					statmap.insert(TStatPair(statraw, adjsize));
				}
				catch (const std::invalid_argument& e) {

					//station
					stat_prev = stat_zw;
					stat_zw = stat_next;
					stat_next = line[i] + ":" + linenumber;
					statraw = line[i] + ":" + linenumber;
					searchstring = line[i] + ":";
					if (i == 0)
					{
						ring = stat_next;
						ringnum = adjsize;
					}
						
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

			if (ring == statraw)
			{
				adjacency_list[ringnum].emplace_back(stat_zw, dist_to_next);
				//adjacency_list[adjsize-1].emplace_back(ring, dist_to_next);
			}
			else
			{ 
				adjacency_list.emplace_back();	//Letzte Station hinzuf�gen
				adjacency_list[adjsize].emplace_back(stat_zw, dist_to_next);
				statmap.insert(TStatPair(statraw, adjsize));
				adjsize++;
			}
			
			while (adjacency_list.size() > adjsize + 1)//�berfl�ssige (leere) Eintr�ge im Vektor l�schen
			{
				adjacency_list.pop_back();
			}
		}
	}

	input.close();

	//Nebenstationen in AdjList eintragen
	for(auto it = statmap.begin(); it != statmap.end(); ++it){

        string station = it->first.substr(0, it->first.find(':'));

        for(auto lineIt = LineArray.begin(); lineIt != LineArray.end(); ++lineIt){

            string str = station + ":" + *lineIt;
            auto there = statmap.find(str);

            if (there != statmap.end() && there != it)
            {
                //cout << it->second << endl;
                adjacency_list[it->second].emplace_back(str,5);
            }
        }
	}

	//Benenstationen in Node eintragen
    for(auto it = statmap.begin(); it != statmap.end(); ++it){

		string station = it->first.substr(0, it->first.find(':'));

		for(auto lineIt = LineArray.begin(); lineIt != LineArray.end(); ++lineIt){

			string str = station + ":" + *lineIt;
			auto there = statmap.find(str);

			if (there != statmap.end() && there != it) {

				for (int i = 0; i < adjacency_list.size(); i++)
					for (int j = 0; j < adjacency_list[i].size(); j++) {

						if (adjacency_list[i][j].stationname == it->first)
							adjacency_list[i][j].appendNeben(str);
					}
			}
		}
	}

    /*for (int i = 0; i < adjacency_list.size(); i++)
        for (int j = 0; j < adjacency_list[i].size(); j++)
            adjacency_list[i][j].displayInfo();*/

    //user input
	string startin, endin;
    vector<int> startstations, endstations;
	TStatMap::const_iterator mapIt, mapIt2;
    bool validstart = false;
    bool validend = false;

    do{

		if (!validstart)
		{
			std::cout << "Start station: ";
			std::cin >> startin;

			if (cin.fail()) {
				std::cerr << "Invalid station" << std::endl;
				continue;
			}
			
			mapIt = Test(statmap, startin);
			if (mapIt != statmap.end())
			{
				startstations.emplace_back(mapIt->second);
				validstart = true;
				for (int j = 0; j < adjacency_list[mapIt->second].size(); j++) {
					for (auto lineIt:LineArray)//Mögliche Linien
					{
						string test = startin + ":" + lineIt;
						if (adjacency_list[mapIt->second][j].stationname == test)
						{
							mapIt2=Test(statmap, test);
							if (mapIt2 != statmap.end() && mapIt2->second != startstations[0])
							{
								startstations.emplace_back(mapIt2->second);
							}
						}
					}
				}
			}
		}

        if(!validstart){
            std::cerr << "Invalid station" << std::endl;
            continue;
        }

		if (!validend)
		{
			std::cout << "End station: ";
			std::cin >> endin;

			if (cin.fail()) {
				std::cerr << "Invalid station" << std::endl;
				continue;
			}

			mapIt = Test(statmap, endin);
			if (mapIt != statmap.end())
			{
				endstations.emplace_back(mapIt->second);
				validend = true;
				for (int j = 0; j < adjacency_list[mapIt->second].size(); j++) {
					for (auto lineIt : LineArray)//Mögliche Linien
					{
						string test = endin + ":" + lineIt;
						if (adjacency_list[mapIt->second][j].stationname == test)
						{
							mapIt2 = Test(statmap, test);
							if (mapIt2 != statmap.end() && mapIt2->second != endstations[0])
							{
								endstations.emplace_back(mapIt2->second);
							}
						}
					}
				}
			}
		}

        if(!validend){
            std::cerr << "Invalid station" << std::endl;
            continue;
        }

    }while(!validstart || !validend);

    int shortestTravelTime = INT_MAX;
    std::list<int> shortestPath;
    for(int i = 0; i < startstations.size(); i++){

        weight_t weight_vec;
        index_t index_vec;

        //dijkstra from startpoint to every station in the graph
        computeDijkstra(startstations[i], adjacency_list, statmap, weight_vec, index_vec);

        for(int j = 0; j < endstations.size(); j++){

            if(weight_vec[endstations[j]] < shortestTravelTime){

                shortestTravelTime = weight_vec[endstations[j]];
                shortestPath = DijkstraGetShortestPathTo(endstations[j], index_vec);
            }
        }
    }

    std::cout << "Shortest traveltime: " << std::endl;

    std::string startstation, endstation;
    for(auto pathit = shortestPath.begin(); pathit != shortestPath.end(); ++pathit){
        for(auto stationsit = statmap.begin(); stationsit != statmap.end(); ++stationsit){

            if (stationsit->second == *pathit)//station id in path => print station
            {
                std::cout << stationsit->first << " ";
                if (pathit == shortestPath.begin())
                    startstation = stationsit->first;
                endstation = stationsit->first;
            }
        }
    }

    std::cout << endl << "Time from "<< startstation << " to " << endstation << " is " << shortestTravelTime << " minutes" << std::endl << std::endl;

	cin.ignore();
	std::cin.get();

    return 0;
}