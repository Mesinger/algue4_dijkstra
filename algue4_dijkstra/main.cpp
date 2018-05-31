#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

int main(int argc, char** argv) {

	/*command line arguments*/
	if(argc != 2){
		std::cerr << "Invalid arguments" << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream input(argv[1]);

	if(input.fail()){
		std::cerr << "Invalid file" << std::endl;
		return EXIT_FAILURE;
	}

	std::string line, sbuf;
	std::vector<std::vector<std::string>> lines;
	std::vector<std::string> stations_distances;

	while(getline(input, line)){

		std::stringstream ss(line);

		while (ss >> sbuf)
			stations_distances.emplace_back(sbuf);

        lines.emplace_back(stations_distances);
	}

	input.close();

	for(auto line : lines){
		for(auto s : line){

			std::cout << s << std::endl;
		}
	}

	return EXIT_SUCCESS;
}