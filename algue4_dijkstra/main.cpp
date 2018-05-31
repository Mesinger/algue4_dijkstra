
#include <iostream>
#include <string>
#include <unordered_set>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class Nodes {
private:
	string Stationsname;
	string Linie;
	int Entfernung;
public:
	Nodes(string StName, string Lin,int Entf) {
		Stationsname = StName;
		Linie = Lin;
		Entfernung = Entf;
	}
	~Nodes() {
	}
	string getStName() const
	{
		return this->Stationsname;
	}
	string getLinie() const
	{
		return this->Linie;
	}
	int getEntfernung() const
	{
		return this->Entfernung;
	}
	void displayInfo()
	{
		cout << " :: " << this->Stationsname << " :: " << this->Linie << " :: " << this->Entfernung << endl;
	}
};


typedef std::vector<std::vector<Nodes> > adjacency_list_t;


int main(int argc, char** argv) {
	adjacency_list_t adjacency_list(1);//Netz mit 1 Station Initialisieren
	vector<Nodes> vec;
	
	/*command line arguments*/
	/*
	if (argc != 2) {
		std::cerr << "Invalid arguments" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input(argv[1]);

	if (input.fail()) {
		std::cerr << "Invalid file" << std::endl;
		return EXIT_FAILURE;
	}

	std::string line, sbuf;
	std::vector<std::vector<std::string>> lines;
	std::vector<std::string> stations_distances;

	while (getline(input, line)) {

		std::stringstream ss(line);

		while (ss >> sbuf)
			stations_distances.emplace_back(sbuf);

		lines.emplace_back(stations_distances);
	}*/

	string St, L;
	int E;
	St = "Schwedenplatz";
	L = "U1";
	E = 5;
	adjacency_list[0].push_back(Nodes(St, L, E));
	St = "Stephansplatz";
	L = "U1";
	E = 2;
	adjacency_list[0].push_back(Nodes(St, L, E));
	St = "Karlsplatz";
	L = "U1";
	E = 3;
	adjacency_list[0].push_back(Nodes(St, L, E));

	adjacency_list.push_back(vec);	//Neue Station hinzufügen
	adjacency_list[1].push_back(adjacency_list[0][1]);
		
	adjacency_list[0][0].displayInfo();
	adjacency_list[0][1].displayInfo();
	adjacency_list[0][2].displayInfo();
	adjacency_list[1][0].displayInfo();
	//return EXIT_SUCCESS;
	return 0;
}