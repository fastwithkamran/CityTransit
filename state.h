#pragma once
#include "types.h"
#include <vector>
#include <unordered_map>
#include <utility>

extern std::vector<std::pair<double, double>> nodes; // each node has (lon, lat)
extern std::vector<std::vector<Edge>> adj;           // adjacency list
extern std::unordered_map<std::string, int> ids;     // unique coordinate pair is a node
extern std::vector<std::string> roadName;            // store road names
extern std::vector<Place> allPlaces;                 // store places with district and town
