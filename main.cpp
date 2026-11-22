#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>
using namespace std;

struct Edge
{
    int to;
    double distance;
    int road = -1;
};

struct Place
{
    double lon, lat;
    string district, town, name;
};

vector<pair<double, double>> nodes; // each node has (lon, lat)
vector<vector<Edge>> adj;           // adjacency list
unordered_map<string, int> ids;     // unique coordinate pair is a node
vector<string> roadName;            // store road names
vector<Place> allPlaces;            // store places with district and town

// create node or returns the neighbouring node id
int getNode(double lon, double lat)
{
    string key = to_string(lon) + "," + to_string(lat); // convert the coord into a key
    auto flag = ids.find(key);                          // check is this key exists in map, returns the address
    if (flag != ids.end())                              // found, reuse the id
        return flag->second;                            // return the id

    int id = nodes.size();
    ids[key] = id;               // add key into the map
    nodes.push_back({lon, lat}); // push the new node
    adj.push_back({});           // edge list for the new node

    return id;
}

// find the distance from lat lon btw two nodes
double haversine(pair<double, double> p1, pair<double, double> p2)
{
    const double PI = 3.14159265358979;
    double radius = 6371000.0;  // earth radius in meters
    double radian = PI / 180.0; // turn GPS degrees into radians

    double lon1 = p1.first, lat1 = p1.second;
    double lon2 = p2.first, lat2 = p2.second;

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

// returns the name of a road id
string nameOfRoad(int r)
{
    if (r == -1 || roadName[r].empty())
        return "Unnamed road";
    return roadName[r];
}

void dijkstra(int source, int destination)
{
    int n = nodes.size();
    double *dist = new double[n];
    bool *visited = new bool[n];
    int *route = new int[n];
    int *roads = new int[n];

    for (int i = 0; i < n; i++)
    { // Initialization
        dist[i] = INFINITY;
        visited[i] = false;
        route[i] = -1;
        roads[i] = -1;
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
                route[v] = u;      // v is reached through u
                roads[v] = e.road; // road used to enter v
            }
        }
    }

    // Shortest distance
    if (dist[destination] == INFINITY)
        cout << endl
             << "No route" << endl;
    else
        cout << endl
             << "Shortest Distance: " << dist[destination] << endl
             << endl;

    cout << "Shortest Path: " << endl;
    // Print the road with distance
    vector<pair<int, double>> steps;
    for (int v = destination; route[v] != -1; v = route[v]) // loop backwards till source
    {
        int u = route[v]; // u is the node that connect v
        double d = dist[v] - dist[u];
        steps.push_back({roads[v], d});
    }

    reverse(steps.begin(), steps.end());

    // one road name is repeated multiple times
    for (long long i = 0; i < steps.size();)
    {
        string cur = nameOfRoad(steps[i].first); // name of road
        double total = 0;                        // length

        while (i < steps.size() && nameOfRoad(steps[i].first) == cur) // loop till same road pass
        {
            total += steps[i++].second; // sum the distance
        }
        cout << cur << " (" << total << "m)" << endl;
    }

    delete[] dist;
    delete[] visited;
    delete[] route;
    delete[] roads;
}

// Find the nearest road of user source and destination
int nearestNode(double lon, double lat)
{
    pair<double, double> place = {lon, lat};
    double minDist = INFINITY;
    int bestId = -1;

    for (int i = 0; i < nodes.size(); i++)
    {
        double val = haversine(place, nodes[i]);
        if (val < minDist)
        {
            minDist = val;
            bestId = i;
        }
    }
    return bestId;
}

// user enter the selected option
int selectInput(vector<string> &options, string label)
{
    cout << "Select " << label << ";" << endl;
    for (int i = 0; i < options.size(); i++)
    {
        cout << i + 1 << " " << options[i] << endl;
    }

    int userinput;
    cout << "Enter: ";
    cin >> userinput;

    return userinput - 1; // since index starts from 0
}

// user can select a place from the Place struct
Place pickPlace()
{
    // display the districts
    vector<string> districts;
    // Extract the districts
    for (Place &p : allPlaces)
    {
        if (find(districts.begin(), districts.end(), p.district) == districts.end())
            districts.push_back(p.district);
    }

    int index = selectInput(districts, "District");
    string district = districts[index];

    vector<string> towns;
    // Extract the towns
    for (Place &p : allPlaces)
    {
        if (p.district == district && find(towns.begin(), towns.end(), p.town) == towns.end())
            towns.push_back(p.town);
    }

    index = selectInput(towns, "Town");
    string town = towns[index];

    // Extract the places
    vector<Place> matches; // store the match places
    vector<string> names;
    for (Place &p : allPlaces)
    {
        if (p.district == district && p.town == town)
        {
            matches.push_back(p);
            names.push_back(p.name);
        }
    }

    index = selectInput(names, "Place");
    return matches[index]; // return the selected place
}

int main()
{
    ifstream file("Karachi_Roads.csv"); // Extracting the coords from the csv to form adj list
    string line;
    getline(file, line);        // to skip the header
    while (getline(file, line)) // loop through all roads
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back(); // remove the carriage return from the line

        size_t endcoords = line.find(',', 1); // find comma to extract coords
        string coords = line.substr(0, endcoords);
        size_t endway = line.find(',', endcoords + 1); // find comma to extract way
        string oneway = line.substr(endcoords + 1, endway - endcoords - 1);
        string name = line.substr(endway + 1);

        roadName.push_back(name);         // push the road name
        int roadId = roadName.size() - 1; // update the road count

        vector<pair<double, double>> pts;
        stringstream ss(coords); // to convert it into a file stream
        string p;
        while (getline(ss, p, ';'))
        {
            size_t gap = p.find(' ');            // find the space to separate lon lat
            double lon = stod(p.substr(0, gap)); // stod converts string to double
            double lat = stod(p.substr(gap + 1));
            pts.push_back({lon, lat}); // push the lon lat
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

    // Menu Option for User
    ifstream places("Karachi_Places.csv");
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

    // User select the source and destination
    cout << "Choose Source: " << endl;
    Place s = pickPlace();
    cout << "Choose Destination: " << endl;
    Place d = pickPlace();

    // Find the nearest road
    int src = nearestNode(s.lon, s.lat);
    int dest = nearestNode(d.lon, d.lat);

    dijkstra(src, dest);
}
