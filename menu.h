#pragma once
#include "types.h"
#include <vector>
#include <string>

// user enter the selected option
int selectInput(std::vector<std::string> &options, std::string label);
// user can select a place from the Place struct
Place pickPlace();