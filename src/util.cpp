#include "util.h"

//Copied from StackOverflow because of course
std::vector<std::string> split(const std::string& input, const std::string& fmt) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(fmt);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}

// Print recieved prediction in CSV format
void printPredictionCSV(std::ostream& os, const std::vector<std::pair<units::time_point, coords>>& p) {
    for (const auto& dpoint : p) {
        units::print_time_point(os, dpoint.first, "%H%M%S");
        os << ',' << dpoint.second << std::endl;
    }
    os << '.' << std::endl;
}

void printPredictionJSON(std::ostream& os, const std::vector<std::pair<units::time_point, coords>>& p) {
    os << '[';
    for (auto dpoint = p.begin(); dpoint != p.end(); ++dpoint) {
        if (dpoint != p.begin()) os << ',' << std::endl;
        os << "{\"tstamp\":\"";
        units::print_time_point(os, dpoint->first, "%FT%TZ");
        os << "\",\"lat\":" << dpoint->second.lat;
        os << ",\"lon\":" << dpoint->second.lon;
        os << ",\"alt\":" << double(dpoint->second.alt);
        os << '}';
    }
    os << ']';
}
