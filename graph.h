#pragma once
#include <string>
#include <utility>
using namespace std;

// load roads from csv and built an adjacency list
void loadRoads(const string &filename);

// create node or returns the neighbouring node id
int getNode(double lon, double lat);

// find the distance from lat lon btw two nodes
double haversine(pair<double, double> p1, pair<double, double> p2);

// Find the nearest road of user source and destination
int nearestNode(double lon, double lat);