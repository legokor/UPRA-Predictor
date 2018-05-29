#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>
#ifdef DEBUG
#include <cassert>
#include <cmath>
#endif

namespace units {
    /* quantity in SI unit */
    template <int M, int KG, int S, int K>
    class Quantity {
        double magnitude;
    public:
        constexpr Quantity(double magn = 0): magnitude{magn} {
#ifdef DEBUG
            assert(!std::isnan(magn));
#endif
        }
        constexpr explicit operator double() const { return magnitude; }
    };

    template <>
    class Quantity<0,0,1,0> {
        std::chrono::seconds t;
    public:
        constexpr Quantity<0,0,1,0>(double magnitude = 0): t{(long)magnitude} {}
        constexpr explicit Quantity<0,0,1,0>(std::chrono::seconds t_): t{t_} {}
        constexpr explicit operator double() const { return t.count(); }
        constexpr operator std::chrono::seconds() { return t; }
    };
    using time_point = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;

    /* short names for quantities */
    using length = Quantity<1, 0, 0, 0>;          /* hossz, m */
    using height = length;
    using area = Quantity<2, 0, 0, 0>;            /* terület, m^2 */
    using mass = Quantity<0, 1, 0, 0>;            /* tömeg, kg */
    using density = Quantity< -3, 1, 0, 0>;       /* sűrűség, kg / m^3 */
    using time = Quantity<0, 0, 1, 0>;            /* idő, s */
    using speed = Quantity<1, 0, -1, 0>;          /* sebesség, m/s */
    using acceleration = Quantity<1, 0, -2, 0>;   /* gyorsulás, m/s^2 */
    using force = Quantity<1, 1, -2, 0>;          /* erő, N=m*kg/s^2 */
    using energy = Quantity<2, 1, -2, 0>;         /* energia, J=m^2*kg/s^2 */
    using torque = Quantity<2, 1, -2, 0>;         /* forgatónyomaték, Nm=m^2*kg/s^2 */
    using power = Quantity<2, 1, -3, 0>;          /* teljesítmény, Watt = Joule/s = m^2*kg/s^3 */
    using pressure = Quantity< -1, 1, -2, 0>;     /* nyomás, Pascal = kg/(m*s^2) */
    using temperature = Quantity<0, 0, 0, 1>;     /* hőmérséklet, Kelvin */


    /* operators for quantities */
    template <int M, int KG, int S, int K>
    Quantity<M, KG, S, K> operator+(Quantity<M, KG, S, K> a, Quantity<M, KG, S, K> b) {
        return Quantity<M, KG, S, K>{double(a) + double(b)};
    }

    template <int M, int KG, int S, int K>
    Quantity<M, KG, S, K> operator+(Quantity<M, KG, S, K> a, double b) {
        return Quantity<M, KG, S, K>{double(a) + b};
    }

    template <int M, int KG, int S, int K>
    Quantity<M, KG, S, K> operator-(Quantity<M, KG, S, K> a, Quantity<M, KG, S, K> b) {
        return Quantity<M, KG, S, K>{double(a) - double(b)};
    }

    template <int M, int KG, int S, int K>
    Quantity<M, KG, S, K> operator-(Quantity<M, KG, S, K> a, double b) {
        return Quantity<M, KG, S, K>{double(a) - b};
    }

    template <int M1, int KG1, int S1, int K1, int M2, int KG2, int S2, int K2>
    Quantity<M1+M2, KG1+KG2, S1+S2, K1+K2> operator*(Quantity<M1, KG1, S1, K1> a, Quantity<M2, KG2, S2, K2> b) {
        return Quantity<M1+M2, KG1+KG2, S1+S2, K1+K2>{double(a) * double(b)};
    }

    template <int M, int KG, int S, int K>
    Quantity<M, KG, S, K> operator*(Quantity<M, KG, S, K> a, double b) {
        return Quantity<M, KG, S, K>{double(a) * b};
    }

    template <int M1, int KG1, int S1, int K1, int M2, int KG2, int S2, int K2>
    Quantity<M1-M2, KG1-KG2, S1-S2, K1-K2> operator/(Quantity<M1, KG1, S1, K1> a, Quantity<M2, KG2, S2, K2> b) {
        return Quantity<M1-M2, KG1-KG2, S1-S2, K1-K2>{double(a) / double(b)};
    }

    template <int M, int KG, int S, int K>
    Quantity<M, KG, S, K> operator/(Quantity<M, KG, S, K> a, double b) {
        return Quantity<M, KG, S, K>{double(a) / b};
    }

    template <int M, int KG, int S, int K>
    bool operator<(Quantity<M, KG, S, K> a, Quantity<M, KG, S, K> b) {
        return double(a) < double(b);
    }

    template <int M, int KG, int S, int K>
    bool operator>(Quantity<M, KG, S, K> a, Quantity<M, KG, S, K> b) {
        return double(a) > double(b);
    }

    template <int M, int KG, int S, int K>
    bool operator>=(Quantity<M, KG, S, K> a, Quantity<M, KG, S, K> b) {
        return double(a) >= double(b);
    }

    template <int M, int KG, int S, int K>
    bool operator<=(Quantity<M, KG, S, K> a, Quantity<M, KG, S, K> b) {
        return double(a) <= double(b);
    }

    template <int M, int KG, int S, int K>
    bool operator==(Quantity<M, KG, S, K> a, Quantity<M, KG, S, K> b) {
        return double(a) == double(b);
    }

    inline time_point operator+(time_point t, time dur) {
        t += dur;
        return t;
    }

    inline time_point operator-(time_point t, time dur) {
        t -= dur;
        return t;
    }

    /* generic stream inserter operator for quantities */
    template <int M, int KG, int S, int K>
    std::ostream & operator<<(std::ostream & os, Quantity<M, KG, S, K> m) {
        os << double(m) << ' ';
        bool elso = true;
        if (M != 0) {
            elso = false;
            os << "m^" << M;
        }
        if (KG != 0) {
            if (!elso) os << '*';
            elso = false;
            os << "kg^" << KG;
        }
        if (S != 0) {
            if (!elso) os << '*';
            elso = false;
            os << "s^" << S;
        }
        if (K != 0) {
            if (!elso) os << '*';
            elso = false;
            os << "K^" << K;
        }
        return os;
    }

    /* specialized stream inserter operator for force */
    //template <>
    //std::ostream & operator<<(std::ostream & os, force m) {
    //    os << m.magnitude << " N";
    //    return os;
    //}

    inline void print_time_point(std::ostream& os, time_point t, const char* fmt) {
        std::time_t tt = time_point::clock::to_time_t(t);
        os << std::put_time( std::gmtime(&tt), fmt );
    }


    /* user-defined literals for SI quantities */
    namespace literals {
        constexpr length operator "" _m (long double magnitude) {
            return length(magnitude);
        }

        constexpr length operator "" _km (long double magnitude) {
            return length(magnitude * 1000.0);
        }

        constexpr mass operator "" _kg (long double magnitude) {
            return mass(magnitude);
        }

        constexpr force operator "" _N (long double magnitude) {
            return force(magnitude);
        }
    } /* namespace literals */
} /* namespace units */
