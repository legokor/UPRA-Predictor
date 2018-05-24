#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "dataset.hpp"
#include "units.hpp"
#include "vecs.h"

struct coords {
    constexpr static const double WGS84_a = 6378137.0;
    constexpr static const double WGS84_b = 6356752.3142;
    constexpr static const double WGS84_e_sq = (WGS84_a * WGS84_a - WGS84_b * WGS84_b) / (WGS84_a * WGS84_a);

    double lat, lon;
    units::height alt;

    // How many metres is 1 degree of latitude on given latitude
    static double metresToLatRatio(double latitude);
    // How many metres is 1 degree of longitude on given latitude
    static double metresToLonRatio(double latitude);

    coords(): lat{0}, lon{0}, alt{0} {}
    coords(double latitude, double longitude, units::height altitude):
        lat{latitude}, lon{longitude}, alt{altitude} {}

    coords operator+(const vec2& v) const;
	coords operator+(const vec3& v) const;
	coords operator-(const vec2& v) const {
		return *this + (-v);
	}
	coords operator-(const vec3& v) const {
		return *this + (-v);
	}

	// Difference in coords in metres
	vec3 operator-(const coords& rhs) const;
};

std::ostream& operator<<(std::ostream& os, const coords& c);
