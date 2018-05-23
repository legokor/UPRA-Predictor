#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <string>
#include <regex>
#include "units.hpp"
#include "coords.h"


/*
UPRA packet format:
$$CCCCCCC,iii,hhmmss,(+/-)xxxx.xxx,(+/-)xxxxx.xxx,aaaaa,eeee,ooo,rrr,

$$ 	START bytes
CCCCCC 	callsign
iii 	Message ID
hhmmss 	GPS time (UTC) (hh-hours, mm-minutes, ss-seconds)
(+/-)xxxx.xxxx 	latitude (NMEA format)
(+/-)xxxxx.xxxx 	longitude (NMEA format)
aaaaa 	altitude (m)
eeee 	external temperature (*10 °C -> eee.e°C)
ooo 	OBC module temperature (*10 °C -> ooo°C)
rrr 	COM module temperature (*10 °C -> rrr°C)
*/

struct TelemetryPacket {
    static std::regex nmeaFmt;
    static std::regex upraPacketFmt;

    int msgid;
    units::time_point gpstime;
    coords location;
    units::temperature extemp;

    static units::time_point parseTimestamp(std::string timeStr);
    static double parseNMEA(std::string nmeaStr);

    TelemetryPacket(std::string upraPacketStr);
};
