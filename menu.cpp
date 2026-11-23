#include "state.h"
#include "menu.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

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
