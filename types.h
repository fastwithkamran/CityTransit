#pragma once
#include <string>
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