#pragma once
#include <iostream>
#include <cmath>

struct vec2 {
	double x, y;

    static vec2 from_polar(double r, double phi_deg);

	vec2(double x0, double y0) { x = x0; y = y0; }

	vec2 operator*(double a) const { return vec2(x * a, y * a); }
	vec2 operator/(double a) const { return *this * (1/a); }

	vec2 operator+(const vec2& v) const {
		return vec2(x + v.x, y + v.y);
	}
	vec2 operator-(const vec2& v) const {
		return vec2(x - v.x, y - v.y);
	}
	/* vec2 operator*(const vec2& v) const {
		return vec2(x * v.x, y * v.y);
	} */
	vec2& operator+=(const vec2& v) {
		return *this = *this + v;
	}
	vec2& operator-=(const vec2& v) {
		return *this = *this - v;
	}
	/* vec2& operator*=(const vec2& v) {
		return *this = *this * v;
	} */
	vec2 operator-() const {
		return *this * -1;
	}
	double length() const { return sqrt(x*x + y*y); }
	vec2 normalize() const { return *this/this->length(); }
};

struct vec3 {
	double x, y, z;

	vec3(double x0, double y0, double z0 = 0) { x = x0; y = y0; z = z0; }
	vec3(vec2 v, double z0 = 0) { x = v.x; y = v.y; z = z0; }

	vec3 operator*(double a) const { return vec3(x * a, y * a, z * a); }
	vec3 operator/(double a) const { return *this * (1/a); }

	vec3 operator+(const vec3& v) const {
		return vec3(x + v.x, y + v.y, z + v.z);
	}
	vec3 operator-(const vec3& v) const {
		return vec3(x - v.x, y - v.y, z - v.z);
	}
	/* vec3 operator*(const vec3& v) const {
		return vec3(x * v.x, y * v.y, z * v.z);
	} */
	vec3& operator+=(const vec3& v) {
		return *this = *this + v;
	}
	vec3& operator-=(const vec3& v) {
		return *this = *this - v;
	}
	/* vec3& operator*=(const vec3& v) {
		return *this = *this * v;
	} */
	vec3 operator-() const {
		return *this * -1;
	}
	double length() const { return sqrt(x*x + y*y + z*z); }
	vec3 normalize() const { return *this/this->length(); }
};


double dot(const vec2& v1, const vec2& v2);
vec3 cross(const vec2& v1, const vec2& v2);

std::ostream& operator<<(std::ostream& os, vec2 v);

double dot(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);

std::ostream& operator<<(std::ostream& os, vec3 v);
