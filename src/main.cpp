#include <iostream>
#include <functional>
#include <regex>
#include "flight.h"
#include "telemetrypacket.h"
#include "util.h"

/// Currently active flights
std::map<std::string, std::unique_ptr<BalloonFlight>> flights;

const auto cmd_result_ok = "[ok] ";
const auto cmd_result_info = "[ii] ";
const auto cmd_result_warn = "[WW] ";
const auto cmd_result_err = "[EE] ";

void cmd_newflight(std::vector<std::string> args) {
    BalloonProperties props = BalloonProperties({args.begin()+2, args.end()});
    auto wdata = std::make_unique<WeatherData>();
    wdata->read(std::cin);

    auto flight = std::make_unique<BalloonFlight>(props, std::move(wdata));
    flights[args.at(1)] = std::move(flight);
    std::cout << cmd_result_ok << "Created flight " << args.at(1);
}

void cmd_recvuprapacket(std::vector<std::string> args) {
    TelemetryPacket packet{args.at(2)};

    flights[args.at(1)]->recieveBalloonData(packet.gpstime, packet.location);
    std::cout << cmd_result_ok;
}

void cmd_balloonprop_get(std::vector<std::string> args) {
    auto val = flights[args.at(1)]->getBalloonProp(args.at(2));

    std::cout << cmd_result_ok << val;
}

void cmd_balloonprop_set(std::vector<std::string> args) {
    flights[args.at(1)]->setBalloonProp(args.at(2), std::stod(args.at(3)));
    std::cout << cmd_result_ok << "Set balloon property " << args.at(2);
}

void cmd_predict(std::vector<std::string> args) {
    units::time timestep = 5; //seconds
    if (args.size() > 2)
        timestep = std::stod(args.at(2));

    auto prediction = flights[args.at(1)]->predict(timestep);

    std::cout << cmd_result_ok << "Flight prediction for flight " + args.at(1) + ":";
    printPredictionCSV(std::cout, prediction);
}

void cmd_endflight(std::vector<std::string> args) {
    flights.erase(args.at(1));
    std::cout << cmd_result_ok << "Concluded flight " + args.at(1);
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
            std::cout << cmd_result_err << "Unknown command.";
        }
    }

    return 0;
}
