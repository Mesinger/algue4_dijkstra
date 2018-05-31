
#include <iostream>
#include <string>
#include <unordered_set>
#include <map>


using namespace std;

struct NodeHasher;
struct NodeComperator;

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
	string getEntfernung() const
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