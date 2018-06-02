#include "findinmap.h"

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
        //std::cout << "Schon da: " << i->first << ", " << i->second << std::endl;
        return i;
    }
    else
        return map.end();
}