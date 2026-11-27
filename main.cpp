#include <iostream>
using namespace std;

#include "state.h"
#include "graph.h"
#include "menu.h"
#include "route.h"
#include "places.h"

vector<pair<double, double>> nodes; // each node has (lon, lat)
vector<vector<Edge>> adj;           // adjacency list
unordered_map<string, int> ids;     // unique coordinate pair is a node
vector<string> roadName;            // store road names
vector<Place> allPlaces;            // store places with district and town

int main()
{
    // load roads from csv and built an adjacency list
    loadRoads("Karachi_Roads.csv");
    // load places from csv
    loadPlaces("Karachi_Places.csv");

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
