#include "state.h"
#include "menu.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
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
    while (true)
    {
        cout << "Enter: ";

        if (!(cin >> userinput) || userinput <= 0 || userinput > options.size())
        {
            cout << "Wrong Input.." << endl;
            cin.clear();             // reset error flag
            cin.ignore(10000, '\n'); // ignore the bad characters stuck in the memory buffer
        }
        else
        {
            break;
        }
    }
    
    cin.ignore(10000, '\n'); // ignore the bad characters stuck in the memory buffer
    return userinput - 1; // since index starts from 0
}

string toLower(string s)
{
    for (char &c : s)
    {
        c = tolower(c);
    }
    return s;
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
    while (true)
    {
        // Search places
        string text;
        cout << "Search place: ";
        getline(cin, text);
        string s = toLower(text);

        vector<Place> matches; // store the match places
        vector<string> names;
        for (Place &p : allPlaces)
        {
            if (p.district == district && p.town == town && toLower(p.name).find(s) != string::npos)
            {
                matches.push_back(p);
                names.push_back(p.name);
            }
        }

        if (matches.empty())
        {
            cout << "No such place found.." << endl;
        }
        else if (matches.size() > 30)
        {
            cout << matches.size() - 30 << " more places matches, type more letters to find your desire place." << endl;
        }
        else
        {
            index = selectInput(names, "Place");
            return matches[index]; // return the selected place
        }
    }
}
