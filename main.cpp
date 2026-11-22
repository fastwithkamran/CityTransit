#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
using namespace std;

struct Edge
{
    int to;
    double len;
};

int main()
{
    vector<pair<double, double>> nodes; // each node has (lat, lon)
    vector<vector<Edge>> adj;           // adjacency list
    unordered_map<string, int> id;      // unique coordinate pair is a node

    ifstream file("Karachi_Roads.csv"); // Extracting the coords from the csv to form adj list
    string line;
    getline(file, line);        // to skip the header
    while (getline(file, line)) // loop through all roads
    {
        size_t endcoords = line.find(',', 1); // find comma to extract coords
        string coords = line.substr(1, endcoords - 1);
        size_t endway = line.find(',', endcoords + 1); // find comma to extract way
        string oneway = line.substr(endway - 1);

        vector<pair<double, double>> pts;
        stringstream ss(coords); // to convert it into a file stream
        string p;
        while (getline(ss, p, ';'))
        {
            size_t gap = p.find(' ');            // find the space to separate lon lat
            double lon = stod(p.substr(0, gap)); // stod converts string to double
            double lat = stod(p.substr(gap + 1));
            pts.push_back({lat, lon}); // push the lat lon
        }
    }
}
