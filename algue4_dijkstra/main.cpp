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
#include "UserInputHandler.h"

using namespace std;

int main(int argc, char** argv) {
	
	adjacency_list_t adjacency_list;
	TStatMap statmap;//pair<statname,number adj.list>
	vector<string> LineArray;
	string stat_next, stat_prev, stat_zw, statraw, searchstring, linenumber, ring, sbuf;
	int ringnum, adjsize = 0, dist_to_next, dist_to_prev;

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

	while (getline(input, sbuf)) {

		dist_to_next = 0, dist_to_prev = 0;
		stat_next="", stat_prev="", stat_zw="", statraw="", searchstring="";

		linenumber = sbuf.substr(0, sbuf.find(':'));//herausschneiden der Liniennummer aus dem File
		LineArray.emplace_back(linenumber);
		sbuf = sbuf.substr(sbuf.find(':') + 3);	

		std::vector<std::string> line = split(sbuf, '"');

		int tmp=0;
		for (int i = 0; i < line.size(); i++) {

			if(i%2==1)
				dist_to_prev = tmp;
			try {

				//distance	//Versuch zum herauslesen der Distanz
				//wenn nicht m�glich -> Stationsname
				
				dist_to_next = stoi(line[i]);
				tmp = dist_to_next;
				statmap.insert(TStatPair(statraw, adjsize));
			}
			catch (const std::invalid_argument& e) {

				//Stationen --> werden immer im nachhinein eingetragen nicht sofort, wenn sie eingelesen werden,
				//da dann alle Informationen wie verbundene Stationen auf der Linie zur Verfügung stehen
				stat_prev = stat_zw;	//Stat_prev = Station 2 vor jetzt eingelesener
				stat_zw = stat_next;	//stat_zw = vorherige station
				stat_next = line[i] + ":" + linenumber;	//stat_next = jetzige Station die man gerade einliest
				statraw = line[i] + ":" + linenumber;
				searchstring = line[i] + ":";

				//Merken der ersten Station um nachher prüfen zu können, ob es sich um eine Ringlinie (Anfang=Ende) handelt
				if (i == 0)
				{
					ring = stat_next;
					ringnum = adjsize;
				}
						
			}

			//Da erst im nachhinein eingetragen wird, muss hier beim ersten durchlauf gewartet werden, dass Daten bereit stehen
			//i%2==0 --> so steht immer der Name der Station als auch die Entfernugn dazu bereit
			//Einlesen ist immer um eines früher als eintragen --> stat_next ist beim einlesen die gerade eingelesene station, wobei sie beim eintragen
			//wirklich als die "nächste" station behandelt wird
			if (i % 2 == 0 && i != 0)
			{
				adjacency_list.emplace_back();//Neue Reihe in der AdjListe einfügen
				if (stat_next != "")
					adjacency_list[adjsize].emplace_back(stat_next, dist_to_next);//Nächste Station im Bezug auf die derzeitige in AdjListe eintragen
				if (stat_prev != "")
					adjacency_list[adjsize].emplace_back(stat_prev, dist_to_prev);//Vorherige Station im Bezug auf die derzeitige eintragen
				adjsize++;
			}
		}

		//Vergleich, ob es sich bei der Linie um eine Ringlinie handelt (Start=Ende), Wenn ja kein neuer Eintrag in AdjListe,
		//sondern Vorletzte Station in 1. Station der Linie eintragen
		if (ring == statraw)
		{
			adjacency_list[ringnum].emplace_back(stat_zw, dist_to_next);
			//adjacency_list[adjsize-1].emplace_back(ring, dist_to_next);
		}
		else//Letzte Station hinzufuegen in AdjListe, wenn es keine Ringlinie ist
		{ 
			adjacency_list.emplace_back();	
			adjacency_list[adjsize].emplace_back(stat_zw, dist_to_next);	//In letzte Station wird nur die vorletzte eingetragen
			statmap.insert(TStatPair(statraw, adjsize));
			adjsize++;
		}
		//Ueberfluessige (leere) Eintraege in AdjListe loeschen
		while (adjacency_list.size() > adjsize + 1)
		{
			adjacency_list.pop_back();
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

	//Nebenstationen in Nodes eintragen
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

	//user input
	std::vector<int>startstations, endstations;

	UserInputHandler userInputHandler;

	userInputHandler.getStartStation(startstations, adjacency_list, statmap, LineArray);
	userInputHandler.getEndStation(endstations, adjacency_list, statmap, LineArray);

	//calculating shortest trip
    int shortestTravelTime = INT_MAX;
    std::list<int> shortestPath;
    for(int i = 0; i < startstations.size(); i++)
	{//loop through all startstations
        weight_t weight_vec;
        index_t index_vec;

        //dijkstra from startpoint to every station in the graph
        computeDijkstra(startstations[i], adjacency_list, statmap, weight_vec, index_vec);

        for(int j = 0; j < endstations.size(); j++){//loop through all endstations

            if(weight_vec[endstations[j]] < shortestTravelTime){//find shortest trip
                shortestTravelTime = weight_vec[endstations[j]];
                shortestPath = DijkstraGetShortestPathTo(endstations[j], index_vec);
            }
        }
    }

    //output
	printTrip(shortestPath, statmap, shortestTravelTime);

	cin.ignore();
	std::cin.get();

    return 0;
}