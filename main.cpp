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
    int road = -1;
};

vector<pair<double, double>> nodes; // each node has (lat, lon)
vector<vector<Edge>> adj;           // adjacency list
unordered_map<string, int> ids;     // unique coordinate pair is a node
vector<string> roadname;

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

    double lat1 = p1.first, lon1 = p1.second;
    double lat2 = p2.first, lon2 = p2.second;

    double dlat = (lat2 - lat1) * radian; // convert degrees into radians
    double dlon = (lon2 - lon1) * radian;

    double value = sin(dlat / 2) * sin(dlat / 2) + cos(lat1 * radian) * cos(lat2 * radian) * sin(dlon / 2) * sin(dlon / 2); // haversin formula
    double chord = 2 * atan2(sqrt(value), sqrt(1 - value));
    return radius * chord; // return the distance in meters
}

// Find the unvisited vertex with minimum distance
int minDistance(double dist[], bool visited[])
{
    int n = nodes.size();
    double min = INFINITY;
    int minIndex = -1;

    for (int v = 0; v < n; v++)
    {
        if (!visited[v] && dist[v] < min)
        {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void dijkstra(int source, int destination)
{
    int n = nodes.size();
    double *dist = new double[n];
    bool *visited = new bool[n];

    for (int i = 0; i < n; i++)
    { // Initialization
        dist[i] = INFINITY;
        visited[i] = false;
    }

    // source distance is 0
    dist[source] = 0;

    // process all vertices
    for (int count = 0; count < n; count++)
    {
        int u = minDistance(dist, visited);
        if (u == destination || u == -1)
            break; // reached destination or not reachable

        visited[u] = true;

        for (auto e : adj[u])
        {
            int v = e.to; // get the neighbour node
            if (!visited[v] && dist[u] + e.distance < dist[v])
            {
                dist[v] = dist[u] + e.distance;
            }
        }
    }

    // Shortest distance
    if (dist[destination] == INFINITY)
        cout << "No route" << endl;
    else
        cout << "Shortest distance " << dist[destination] << endl;

    delete[] dist;
    delete[] visited;
}

int main()
{
    ifstream file("Karachi_Roads.csv"); // Extracting the coords from the csv to form adj list
    string line;
    getline(file, line);        // to skip the header
    while (getline(file, line)) // loop through all roads
    {
        size_t endcoords = line.find(',', 1); // find comma to extract coords
        string coords = line.substr(0, endcoords - 1);
        size_t endway = line.find(',', endcoords + 1); // find comma to extract way
        string oneway = line.substr(endcoords + 1, endway - endcoords - 1);
        string name = line.substr(endway + 1);

        roadname.push_back(name);         // push the road name
        int roadId = roadname.size() - 1; // update the road count

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
            adj[a].push_back({b, distance, roadId}); // add the edge
            if (oneway != "yes")                     // if it is an undirected graph add the edge in second node
                adj[b].push_back({a, distance, roadId});
        }
    }
}
