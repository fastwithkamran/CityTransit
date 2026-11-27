#pragma once
#include <string>
struct Edge
{
    int to;
    double distance;
    int road = -1;
};

struct Place
{
    double lon, lat;
    std::string district, town, name;
};