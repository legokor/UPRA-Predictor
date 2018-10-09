#include <iostream>
#include <functional>
#include <regex>
#include "flight.h"
#include "telemetrypacket.h"
#include "util.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/// Currently active flights
std::map<std::string, std::unique_ptr<BalloonFlight>> flights;

const auto cmd_result_ok = "[ok] ";
const auto cmd_result_info = "[ii] ";
const auto cmd_result_warn = "[WW] ";
const auto cmd_result_err = "[EE] ";

constexpr const bool json_output = true;

void newflight(const std::string& flightname, BalloonProperties bprops, std::unique_ptr<WeatherData> wdata) {
    auto flight = std::make_unique<BalloonFlight>(bprops, std::move(wdata));
    flights[flightname] = std::move(flight);
}

void cmd_newflight(std::vector<std::string> args) {
    BalloonProperties props = BalloonProperties({args.begin()+2, args.end()});

    auto wdata = std::make_unique<WeatherData>();
    wdata->read(std::cin);

    newflight(args.at(1), props, std::move(wdata));

    std::cout << cmd_result_ok << "Created flight " << args.at(1);
}

void cmd_newflight_json(const json& cmd) {
    auto wdata = std::make_unique<WeatherData>();

    for (json dpoint_j : cmd.at("weatherdata")) {
        wdata->addDataPoint(std::make_tuple(
            units::height(dpoint_j.at("hght").get<double>()),
            units::pressure(dpoint_j.at("pres").get<double>() *100),
            units::temperature(dpoint_j.at("temp").get<double>() +273.15),
            dpoint_j.at("wdir").get<double>(),
            units::speed(dpoint_j.at("wspd").get<double>())
        ));
    }

    newflight(
        cmd.at("flightname"),
        BalloonProperties(
            cmd.at("balloonprops").at("balloon_dry_mass").get<double>(),
            cmd.at("balloonprops").at("parachute_dry_mass").get<double>(),
            cmd.at("balloonprops").at("payload_dry_mass").get<double>(),
            cmd.at("balloonprops").at("nozzle_lift").get<double>(),
            cmd.at("balloonprops").at("parachute_area").get<double>(),
            cmd.at("balloonprops").at("parachute_drag_c").get<double>(),
            cmd.at("balloonprops").at("balloon_drag_c").get<double>(),
            cmd.at("balloonprops").at("design_burst_diam").get<double>()
        ),
        std::move(wdata)
    );
    json out = { {"success", true}, {"msg", "Created flight "+cmd.at("flightname").get<std::string>()} };
    std::cout << out;
}

void recvuprapacket(const std::string& flightname, const std::string& packetStr) {
    TelemetryPacket packet{packetStr};
    flights[flightname]->recieveBalloonData(packet.gpstime, packet.location);
}

void cmd_recvuprapacket(const std::vector<std::string>& args) {
    recvuprapacket(args.at(1), args.at(2));
    std::cout << cmd_result_ok;
}
void cmd_recvuprapacket_json(const json& cmd) {
    recvuprapacket(cmd.at("flightname"), cmd["packet"]);
    json out = { {"success", true}, {"msg", "Packet recieved."} };
    std::cout << out;
}

auto balloonprop_get(const std::string& flightname, const std::string& propname) {
    return flights[flightname]->getBalloonProp(propname);
}

void cmd_balloonprop_get(const std::vector<std::string>& args) {
    auto val = balloonprop_get(args.at(1), args.at(2));
    std::cout << cmd_result_ok << val;
}
void cmd_balloonprop_get_json(const json& cmd) {
    auto val = balloonprop_get(cmd.at("flightname"), cmd.at("propname"));
    json out = { {"success", true}, {"flightname", cmd.at("flightname")}, {"bprops", {cmd.at("propname"), val}} };
    std::cout << out;
}

void balloonprop_set(const std::string& flightname, const std::string& propname, double value) {
    flights[flightname]->setBalloonProp(propname, value);
}

void cmd_balloonprop_set(const std::vector<std::string>& args) {
    balloonprop_set(args.at(1), args.at(2), std::stod(args.at(3)));
    std::cout << cmd_result_ok << "Set balloon property " << args.at(2);
}
void cmd_balloonprop_set_json(const json& cmd) {
    json bprops = cmd.at("bprops");
    for (json::iterator bprop = bprops.begin(); bprop != bprops.end(); ++bprop) {
        balloonprop_set(cmd.at("flightname").get<std::string>(), bprop.key(), bprop.value());
    }
    json out = { {"success", true} };
    std::cout << out;
}

auto predict(const std::string& flightname, units::time timestep = 5) {
    return flights[flightname]->predict(timestep);
}

void cmd_predict(const std::vector<std::string>& args) {
    units::time timestep = 0;
    if (args.size() > 2)
        timestep = std::stod(args.at(2));

    decltype(predict("")) prediction;
    if (double(timestep) != 0) {
        prediction = predict(args.at(1), timestep);
    } else {
        prediction = predict(args.at(1));
    }

    std::cout << cmd_result_ok << "Flight prediction for flight " + args.at(1) + ":";
    printPredictionCSV(std::cout, prediction);
}

void cmd_predict_json(const json& cmd) {
    // Figure out timestep
    units::time timestep = 0;
    try {
        timestep = cmd.at("timestep").get<double>();
    } catch (...) { }

    // Get prediction...
    decltype(predict("")) prediction;
    if (double(timestep) != 0) {
        prediction = predict(cmd.at("flightname").get<std::string>(), timestep);
    } else {
        prediction = predict(cmd.at("flightname").get<std::string>());
    }

    // Print result
    json j;
    json prediction_j;

    for (auto& dpoint : prediction) {
        json dpoint_j;
        dpoint_j["tstamp"] = units::time_point_to_str(dpoint.first, "%FT%TZ");
        dpoint_j["lat"] = dpoint.second.lat;
        dpoint_j["lon"] = dpoint.second.lon;
        dpoint_j["alt"] = double(dpoint.second.alt);

        prediction_j.push_back(dpoint_j);
    }

    j["success"] = true;
    j["flightname"] = cmd.at("flightname");
    j["prediction"] = std::move(prediction_j);

    std::cout << j;
}

void endflight(const std::string& flightname) {
    flights.erase(flightname);
}

void cmd_endflight(const std::vector<std::string>& args) {
    endflight(args.at(1));
    std::cout << cmd_result_ok << "Concluded flight " + args.at(1);
}
void cmd_endflight_json(const json& cmd) {
    endflight(cmd.at("flightname"));
    json out = { {"success", true}, {"msg", "Concluded flight "+cmd.at("flightname").get<std::string>()} };
    std::cout << out;
}

/// Available commands
const std::map<std::string, std::function<void(const std::vector<std::string>&)>> commands = {
    {"newflight", cmd_newflight},
    {"senduprapacket", cmd_recvuprapacket},
    {"balloonprop-get", cmd_balloonprop_get},
    {"balloonprop-set", cmd_balloonprop_set},
    {"predict", cmd_predict},
    {"endflight", cmd_endflight}
};
/// Available commands (json input-output)
const std::map<std::string, std::function<void(const json&)>> commands_json = {
    {"newflight", cmd_newflight_json},
    {"senduprapacket", cmd_recvuprapacket_json},
    {"balloonprop-get", cmd_balloonprop_get_json},
    {"balloonprop-set", cmd_balloonprop_set_json},
    {"predict", cmd_predict_json},
    {"endflight", cmd_endflight_json}
};

int main() {
    std::ios_base::sync_with_stdio(false);

    if constexpr (json_output) {
        for (json cmd; std::cin >> cmd; ) {
            auto cmdname = cmd.at("cmd").get<std::string>();

            if (cmdname == "quit") break;

            if (commands_json.find(cmdname) != commands_json.end()) {
                commands_json.at(cmdname)(cmd);
            } else {
                json out = { {"success", false}, {"msg", "Unknown command."} };
                std::cout << out;
            }
        }
    } else {
        for (std::string cmd; std::getline(std::cin, cmd); ) {
            auto args = split(cmd, " ");

            if (args[0] == "quit") break;

            if (commands.find(args[0]) != commands.end()) {
                commands.at(args[0])(args);
            } else {
                std::cout << cmd_result_err << "Unknown command.";
            }
        }
    }

    return 0;
}
