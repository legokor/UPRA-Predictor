#include "coords.h"

double coords::metresToLatRatio(double latitude) {
    double phi = latitude / 180 * M_PI;
    return (M_PI * WGS84_a * (1 - WGS84_e_sq)) / (180 * pow(1 - WGS84_e_sq * pow(sin(phi),2), 1.5));
}

double coords::metresToLonRatio(double latitude) {
    double phi = latitude / 180 * M_PI;
    return (M_PI * WGS84_a * cos(phi)) / (180 * sqrt(1 - WGS84_e_sq * pow(sin(phi),2)));
}

coords coords::operator+(const vec2& v) const {
	return coords(lat + v.y / metresToLatRatio(lat), lon + v.x / metresToLonRatio(lat), alt);
}
coords coords::operator+(const vec3& v) const {
	return coords(lat + v.y / metresToLatRatio(lat), lon + v.x / metresToLonRatio(lat), alt + (units::height)v.z);
}

// Difference between coords in metres
vec3 coords::operator-(const coords& rhs) const {
    return vec3((rhs.lat - lat) * metresToLatRatio(lat),
                (rhs.lon - lon) * metresToLonRatio(lat),
                (double)(rhs.alt - alt));
}
