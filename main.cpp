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
}
