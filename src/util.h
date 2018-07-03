#pragma once
#include <iostream>
#include <string_view>
#include <vector>
#include <regex>

std::vector<std::string> split(const std::string& input, const std::string& fmt);
void print_info(std::string_view msg);
void print_ok(std::string_view msg);
void print_warning(std::string_view msg);
void print_error(std::string_view msg);
