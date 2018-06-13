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
Example:
$$HAxUPRA,022,082633,+4728.413,+01903.649,00131,0327,002,001,

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
    static std::regex nmeaLatFmt;
    static std::regex nmeaLonFmt;
    static std::regex upraPacketFmt;

    int msgid;
    units::time_point gpstime;
    coords location;
    units::temperature extemp;

    static units::time_point parseTimestamp(std::string timeStr);
    static double parseNMEA(std::string nmeaStr, const std::regex& nmeaFmt);

    TelemetryPacket(std::string upraPacketStr);
};
