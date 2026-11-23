#pragma once
#include "types.h"
#include <vector>
#include <unordered_map>
using namespace std;

extern vector<pair<double, double>> nodes; // each node has (lon, lat)
extern vector<vector<Edge>> adj;           // adjacency list
extern unordered_map<string, int> ids;     // unique coordinate pair is a node
extern vector<string> roadName;            // store road names
extern vector<Place> allPlaces;            // store places with district and town
