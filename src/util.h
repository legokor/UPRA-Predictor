#pragma once
#include <iostream>
#include <string_view>
#include <vector>
#include <regex>
#include "units.hpp"
#include "coords.h"

std::vector<std::string> split(const std::string& input, const std::string& fmt);

// Print recieved prediction in CSV format
void printPredictionCSV(std::ostream& os, const std::vector<std::pair<units::time_point, coords>>& p);

void printPredictionJSON(std::ostream& os, const std::vector<std::pair<units::time_point, coords>>& p);
