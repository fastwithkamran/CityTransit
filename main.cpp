#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
using namespace std;

vector<pair<double, double>> nodes; // each node has (lat, lon)
vector<vector<Edge>> adj;           // adjacency list
unordered_map<string, int> ids;     // unique coordinate pair is a node

struct Edge
{
    int to;
    double len;
};

int getNode(double lat, double lon)
{
    string key = to_string(lat) + "," + to_string(lon); // convert the coord into a key
    auto flag = ids.find(key);                          // check is this key exists in map, returns the address
    if (flag != ids.end())                              // found, reuse the id
        return flag->second;                            // return the id

    int id = nodes.size();
    ids[key] = id;               // add key into the map
    nodes.push_back({lat, lon}); // push the new node
    adj.push_back({});           // edge list for the new node

    return id;
}

int main()
{
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

        vector<int> neighbors;
        for (int i = 0; i < pts.size(); i++)
        {
            neighbors.push_back(getNode(pts[i].first, pts[i].second)); // create the node, or get the neighbor id
        }
    }
}
