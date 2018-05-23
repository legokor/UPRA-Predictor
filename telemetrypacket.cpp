#include "telemetrypacket.h"

std::regex TelemetryPacket::nmeaFmt("([+-])([0-9]{2})([0-9]+\\.[0-9]+)", std::regex_constants::grep);
std::regex TelemetryPacket::upraPacketFmt(
    "$$([A-Za-z0-9]+),\
    ([0-9]+),\
    ([0-2][0-9][0-5][0-9][0-5][0-9]),\
    ([+-][0-9]+\\.[0-9]+),\
    ([+-][0-9]+\\.[0-9]+),\
    ([0-9]+),\
    ([0-9]+),\
    ([0-9]+),\
    ([0-9]+),",
    std::regex_constants::grep
);

units::time_point TelemetryPacket::parseTimestamp(std::string timeStr) {
    std::tm tm = {};
    std::istringstream ptime_ss(timeStr);
    ptime_ss >> std::get_time(&tm, "%H%M%S");

    // Set unset fields' values to current time because
    // get_time may have zeroed 'em out
    std::time_t t = std::time(nullptr);
    std::tm *tm_now = std::gmtime(&t);
    tm.tm_mday = tm_now->tm_mday;
    tm.tm_mon = tm_now->tm_mon;
    tm.tm_year = tm_now->tm_year;
    tm.tm_wday = tm_now->tm_wday;
    tm.tm_yday = tm_now->tm_yday;
    tm.tm_isdst = tm_now->tm_isdst;

    return std::chrono::time_point_cast<std::chrono::seconds>(
        std::chrono::system_clock::from_time_t(std::mktime(&tm))
    );
}

double TelemetryPacket::parseNMEA(std::string nmeaStr) {
    std::smatch results;
    std::regex_match(nmeaStr, results, nmeaFmt);

    double ret = std::stod(results[2]);         // Degree
    ret += std::stod(results[3]) * 0.01666667;  // Degree minutes
    if (results[1].str() == "-")
        ret *= -1;
    return ret;
}

TelemetryPacket::TelemetryPacket(std::string upraPacketStr) {
    std::smatch results;
    bool matched = std::regex_match(upraPacketStr, results, upraPacketFmt);

    if (!matched)
        throw std::invalid_argument("Malformed UPRA packet!");

    msgid = std::stoi(results[2]);
    gpstime = parseTimestamp(results[3]);
    location.lat = parseNMEA(results[4]);
    location.lon = parseNMEA(results[5]);
    location.alt = std::stod(results[6]);
    extemp = std::stod(results[7]) / 10.0;
}
