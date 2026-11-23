#pragma once
#include "types.h"
#include <vector>
#include <string>
using namespace std;

// user enter the selected option
int selectInput(vector<string> &options, string label);
// user can select a place from the Place struct
Place pickPlace();