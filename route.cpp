#include "route.h"
#include "state.h"
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

// Find the unvisited vertex with minimum distance
int minDistance(double dist[], bool visited[])
{
    cout << "Searching for shortest route.." << endl;
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
    {

        double estMinutes = dist[destination] * 0.002;
        cout << endl
             << "Shortest Distance: " << dist[destination] << " m" << endl
             << "Estimated Time: " << estMinutes << " mins" << endl
             << endl;
    }

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
