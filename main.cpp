#include <iostream>
#include <functional>
#include <regex>
#include "flight.h"
#include "telemetrypacket.h"

//Copied from StackOverflow because of course
std::vector<std::string> split(const std::string& input, const std::string& fmt) {
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(fmt);
    std::sregex_token_iterator
        first{input.begin(), input.end(), re, -1},
        last;
    return {first, last};
}

/// Available commands
std::map<std::string, std::function<void(std::vector<std::string>)>> commands;
/// Currently active flights
std::map<std::string, std::unique_ptr<BalloonFlight>> flights;

/// Print recieved prediction in CSV format
void printPredictionCSV(std::ostream& os, const std::vector<std::pair<units::time_point, coords>>& p) {
    for (const auto& dpoint : p) {
        units::print_time_point(os, dpoint.first, "%H%M%S");
        os << ',' << dpoint.second << std::endl;
    }
    os << '.' << std::endl;
}

void cmd_newflight(std::vector<std::string> args) {
    BalloonProperties props = BalloonProperties({args.begin()+2, args.end()});
    auto wdata = std::make_unique<WeatherData>(std::cin);

    auto flight = std::make_unique<BalloonFlight>(props, std::move(wdata));
    flights[args.at(1)] = std::move(flight);
}

void cmd_recvuprapacket(std::vector<std::string> args) {
    TelemetryPacket packet{args.at(2)};

    flights[args.at(1)]->recieveBalloonData(packet.gpstime, packet.location);
}

void cmd_predict(std::vector<std::string> args) {
    units::time timestep = 5; //seconds
    if (args.size() > 2)
        timestep = std::stod(args.at(2));

    auto prediction = flights[args.at(1)]->predict(timestep);

    printPredictionCSV(std::cout, prediction);
}

void cmd_endflight(std::vector<std::string> args) {
    flights.erase(args.at(1));
}

int main() {

    commands["newflight"] = cmd_newflight;
    commands["senduprapacket"] = cmd_recvuprapacket;
    commands["predict"] = cmd_predict;
    commands["endflight"] = cmd_endflight;

    for (std::string cmd; std::getline(std::cin, cmd); ) {
        auto args = split(cmd, " ");

        if (commands.find(args[0]) != commands.end()) {
            commands[args[0]](args);
        } else {
            std::cout << "Unknown command.";
        }
    }

    return 0;
}
