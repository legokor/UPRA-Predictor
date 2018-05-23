#include "vecs.h"

vec2 vec2::from_polar(double r, double phi_deg) {
    double phi = phi_deg / 180 * M_PI;
    return vec2(r * -sin(phi), r * -cos(phi));
}

double dot(const vec2& v1, const vec2& v2) {
	return (v1.x * v2.x + v1.y * v2.y);
}

double dot(const vec3& v1, const vec3& v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

vec3 cross(const vec2& v1, const vec2& v2) {
	return vec3(0, 0, v1.x * v2.y - v1.y * v2.x);
}

vec3 cross(const vec3& v1, const vec3& v2) {
	return vec3(v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x);
}
