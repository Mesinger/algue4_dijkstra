#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include "Node.h"
#include "stringsplit.h"
#include "findinmap.h"

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

			while (adjacency_list.size() > adjsize + 1)//�berfl�ssige (leere) Eintr�ge im Vektor l�schen
			{
				adjacency_list.pop_back();
			}
		}
	}

	input.close();

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

    for (int i = 0; i < adjacency_list.size(); i++)
        for (int j = 0; j < adjacency_list[i].size(); j++)
            adjacency_list[i][j].displayInfo();

    //user input
    std::string startstation, endstation;
    bool validstart = false;
    bool validend = false;

    do{

        std::string buf;

		if (!validstart)
		{
			std::cout << "Start station: ";
			std::cin >> startstation;

			if (cin.fail()) {
				std::cerr << "Invalid station" << std::endl;
				continue;
			}

			for (auto it = statmap.begin(); it != statmap.end(); ++it) {

				buf = it->first.substr(0, it->first.find(':'));

				if (startstation == buf) {
					validstart = true;
					break;
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
			std::cin >> endstation;

			if (cin.fail()) {
				std::cerr << "Invalid station" << std::endl;
				continue;
			}

			for (auto it = statmap.begin(); it != statmap.end(); ++it) {

				buf = it->first.substr(0, it->first.find(':'));

				if (endstation == buf) {
					validend = true;
					break;
				}
			}
		}

        if(!validend){
            std::cerr << "Invalid station" << std::endl;
            continue;
        }

    }while(!validstart || !validend);

    std::vector<Node> startstations, endstations;

	//std::cin.get();

    return EXIT_SUCCESS;
}