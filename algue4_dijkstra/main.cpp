#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Node.h"

template<typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

using namespace std;

typedef std::vector<std::vector<Node>> adjacency_list_t;

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

    while(getline(input, sbuf)){

        std::vector<int> distances;
        std::vector<std::string> stations;

        std::string linenumber;

        linenumber = sbuf.substr(0, sbuf.find(':'));
        sbuf = sbuf.substr(sbuf.find(':') + 3);

        std::vector<std::string> line = split(sbuf, '"');

        for(int i = 0; i < line.size(); i++){

            try{

                //distance
                distances.emplace_back(stoi(line[i]));

            }catch (const std::invalid_argument& e){

                //station
                stations.emplace_back(line[i] + ":" + linenumber);
            }
        }

        linestations.emplace_back(stations);
        linedistances.emplace_back(distances);
    }

	input.close();

	adjacency_list_t adjacency_list(1);//Netz mit 1 Station Initialisieren

    if(linestations.size() != linedistances.size()){
        std::cerr << "Invalid file" << std::endl;
        return EXIT_FAILURE;
    }

    for(int i = 0; i < linestations.size(); i++){

        std::cout << linestations[i].size() << linedistances[i].size();

        if(linestations[i].size() != linedistances[i].size() + 1){
            std::cerr << "Invalid file" << std::endl;
            return EXIT_FAILURE;
        }

        adjacency_list[i].reserve(linestations[i].size());

        //first station
        adjacency_list[i].emplace_back(linestations[i][0], 0, linedistances[i].front());

        for(int j = 1; j < linestations[i].size() - 1; j++){

            adjacency_list[i].emplace_back(linestations[i][j], linedistances[i][j - 1], linedistances[i][j]);
        }

        //last station
        adjacency_list[i].emplace_back(linestations[i][0], linedistances[i].back(), 0);
    }

    for(std::vector<Node> line : adjacency_list){
        for(Node station : line)
            station.displayInfo();
    }

//	string St;
//	int E;
//	St = "Schwedenplatz:U1";
//	E = 5;
//	adjacency_list[0].push_back(Nodes(St, E));
//	St = "Stephansplatz:U1";
//	E = 2;
//	adjacency_list[0].push_back(Nodes(St, E));
//	St = "Karlsplatz:U1";
//	E = 3;
//	adjacency_list[0].push_back(Nodes(St, E));
//
//	adjacency_list.push_back(vec);	//Neue Station hinzuf�gen
//	adjacency_list[1].push_back(adjacency_list[0][1]);
//
//	adjacency_list[0][0].displayInfo();
//	adjacency_list[0][1].displayInfo();
//	adjacency_list[0][2].displayInfo();
//	adjacency_list[1][0].displayInfo();

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