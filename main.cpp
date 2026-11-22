#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cmath>
using namespace std;

struct Edge
{
    int to;
    double distance;
};

vector<pair<double, double>> nodes; // each node has (lat, lon)
vector<vector<Edge>> adj;           // adjacency list
unordered_map<string, int> ids;     // unique coordinate pair is a node

// create node or returns the neighbouring node id
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

// find the distance from lat lon btw two nodes
double haversine(pair<double, double> p1, pair<double, double> p2)
{
    const float PI = 3.14;
    double radius = 6371000.0;  // earth radius in meters
    double radian = PI / 180.0; // turn GPS degrees into radians

    double lat1 = p1.first, lon1 = p1.first;
    double lat2 = p2.first, lon2 = p2.first;

    double dlat = (lat2 - lat1) * radian; // convert degrees into radians
    double dlon = (lon2 - lon1) * radian;

    double value = sin(dlat / 2) * sin(dlat / 2) + cos(lat1 * radian) * cos(lat2 * radian) * sin(dlon / 2) * sin(dlon / 2); // haversin formula
    double chord = 2 * atan2(sqrt(value), sqrt(1 - value));
    return radius * chord; // return the distance in meters
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
        string oneway = line.substr(endcoords + 1, endway - endcoords - 1);

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

        // connecting the neighbors into the adj list
        for (int i = 0; i + 1 < neighbors.size(); i++)
        {
            int a = neighbors[i], b = neighbors[i + 1]; // a and b are neighbor nodes so there is a edge btw them
            double distance = haversine(pts[i], pts[i + 1]);
            adj[a].push_back({b, distance}); // add the edge
            if (oneway != "yes")             // if it is an undirected graph add the edge in second node
                adj[b].push_back({a, distance});
        }
    }
}
