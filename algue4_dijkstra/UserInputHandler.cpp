#include "UserInputHandler.h"

UserInputHandler::UserInputHandler() {

}

UserInputHandler::~UserInputHandler() {

}

void UserInputHandler::getStartStation(std::vector<int> &startstations, adjacency_list_t& adjacencyList, TStatMap &stations,
                                       std::vector<std::string> &lines) {

    std::string sbuf;

    do{

        std::cout << "Start station: ";
        std::cin >> sbuf;

        if (cin.fail()) {
            std::cerr << "Invalid station" << std::endl;
            continue;
        }

        auto stationsIt = Test(stations, sbuf);

        if(stationsIt == stations.end()) {
            std::cerr << "Invalid station" << std::endl;
            continue;
        }

        int stationid = stationsIt->second;

        startstations.emplace_back(stationid);

        //check for neighbor stations
        for (int i = 0; i < adjacencyList[stationid].size(); i++) {
            for (int j = 0; j < lines.size(); j++)
            {
                string stationname = sbuf + ":" + lines[j];
                if (adjacencyList[stationid][i].stationname == stationname)
                {
                    auto stationsIt2 = Test(stations, stationname);
                    if (stationsIt2 != stations.end() && stationsIt2->second != startstations[0])
                    {
                        startstations.emplace_back(stationsIt2->second);
                    }
                }
            }
        }

    }while(startstations.size() == 0);
}

void UserInputHandler::getEndStation(std::vector<int> &endstations, adjacency_list_t &adjacencyList, TStatMap &stations,
                                     std::vector<std::string> &lines) {

    std::string sbuf;

    do{

        std::cout << "Start station: ";
        std::cin >> sbuf;

        if (cin.fail()) {
            std::cerr << "Invalid station" << std::endl;
            continue;
        }

        auto stationsIt = Test(stations, sbuf);

        if(stationsIt == stations.end()) {
            std::cerr << "Invalid station" << std::endl;
            continue;
        }

        int stationid = stationsIt->second;

        endstations.emplace_back(stationid);

        //check for neighbor stations
        for (int i = 0; i < adjacencyList[stationid].size(); i++) {
            for (int j = 0; j < lines.size(); j++)
            {
                string stationname = sbuf + ":" + lines[j];
                if (adjacencyList[stationid][i].stationname == stationname)
                {
                    auto stationsIt2 = Test(stations, stationname);
                    if (stationsIt2 != stations.end() && stationsIt2->second != endstations[0])
                    {
                        endstations.emplace_back(stationsIt2->second);
                    }
                }
            }
        }

    }while(endstations.size() == 0);
}
