#include <iostream>
#include <functional>
#include <regex>
#include "flight.h"
#include "telemetrypacket.h"
#include "util.h"

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

void cmd_newflight(std::vector<std::string> args) {
    BalloonProperties props = BalloonProperties({args.begin()+2, args.end()});
    auto wdata = std::make_unique<WeatherData>();
    wdata->read(std::cin);

    auto flight = std::make_unique<BalloonFlight>(props, std::move(wdata));
    flights[args.at(1)] = std::move(flight);
}

void cmd_recvuprapacket(std::vector<std::string> args) {
    TelemetryPacket packet{args.at(2)};

    flights[args.at(1)]->recieveBalloonData(packet.gpstime, packet.location);
}

void cmd_balloonprop_get(std::vector<std::string> args) {
    flights[args.at(1)]->getBalloonProp(args.at(2));
}

void cmd_balloonprop_set(std::vector<std::string> args) {
    flights[args.at(1)]->setBalloonProp(args.at(2), std::stod(args.at(3)));
}

void cmd_predict(std::vector<std::string> args) {
    units::time timestep = 5; //seconds
    if (args.size() > 2)
        timestep = std::stod(args.at(2));

    auto prediction = flights[args.at(1)]->predict(timestep);

    std::cout << "[ok]" << std::endl;
    printPredictionCSV(std::cout, prediction);
}

void cmd_endflight(std::vector<std::string> args) {
    flights.erase(args.at(1));
}

/// Available commands
const std::map<std::string, std::function<void(std::vector<std::string>)>> commands = {
    {"newflight", cmd_newflight},
    {"senduprapacket", cmd_recvuprapacket},
    {"balloonprop-get", cmd_balloonprop_get},
    {"balloonprop-set", cmd_balloonprop_set},
    {"predict", cmd_predict},
    {"endflight", cmd_endflight}
};

int main() {
    for (std::string cmd; std::getline(std::cin, cmd); ) {
        auto args = split(cmd, " ");

        if (commands.find(args[0]) != commands.end()) {
            commands.at(args[0])(args);
        } else {
            std::cout << "Unknown command.";
        }
    }

    return 0;
}
