#include "places.h"
#include "state.h"
#include <fstream>
using namespace std;

// function to load places from Karachi_Places.csv
void loadPlaces(const string &filename)
{
    // Menu Option for User
    ifstream places(filename);
    string line;
    getline(places, line);

    while (getline(places, line))
    {
        if (line.empty())
            continue;

        if (line.back() == '\r')
            line.pop_back(); // remove the carriage return from the line

        size_t comma1 = line.find(',', 1); // find comma
        size_t comma2 = line.find(',', comma1 + 1);
        size_t comma3 = line.find(',', comma2 + 1);
        size_t comma4 = line.find(',', comma3 + 1);
        if (comma4 == string::npos)
            continue;

        Place p; // push into the place struct
        p.lon = stod(line.substr(0, comma1));
        p.lat = stod(line.substr(comma1 + 1, comma2 - comma1 - 1));
        p.district = line.substr(comma2 + 1, comma3 - comma2 - 1);
        p.town = line.substr(comma3 + 1, comma4 - comma3 - 1);
        p.name = line.substr(comma4 + 1);
        allPlaces.push_back(p);
    }
}