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

void print_info(std::string_view msg) {
    std::cout << "[ii] " << msg;
}

void print_ok(std::string_view msg) {
    std::cout << "[ok] " << msg;
}

void print_warning(std::string_view msg) {
    std::cout << "[WW] " << msg;
}

void print_error(std::string_view msg) {
   std::cout << "[EE] " << msg;
}
