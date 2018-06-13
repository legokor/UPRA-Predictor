#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include "dataset.hpp"
#include "units.hpp"
#include "vecs.h"

class WeatherData {
    /// Specific gas constant of dry air
    constexpr static const units::Quantity<2, 0, -2, -1> SPECIFIC_GAS_CONST = 287.058;

    /// Nyomás, a magasság függvényében (OMSZ adatból) Pascal
    Dataset<units::height, units::pressure> pressures;
    /// Hőmérséklet, a magasság függvényében (OMSZ adatból) Kelvin
    Dataset<units::height, units::temperature> temperatures;
    /// Szélirány, szélsebesség, a magasság függvényében (OMSZ adatból) m/s, derékszögű koordinátarendzser
    Dataset<units::height, vec2> windVels;

    /// Emelkedés során mért tényleges széladatok
    Dataset<units::height, vec2> ascentWindVels;
    /// Ereszkedés során mért tényleges széladatok (egyelőre nem használt)
    Dataset<units::height, vec2> descentWindVels;



public:
    WeatherData(std::istream& is);

    units::pressure pressureAt(units::height h) { return pressures.getAt(h); }
    units::temperature temperatureAt(units::height h) { return temperatures.getAt(h); }
    units::density densityAt(units::height h);
    vec2 windVelAt(units::height h);

    void addAscentWindData(units::height h, vec2 wVel) { ascentWindVels.addDataPoint(h, wVel); }
    void addDescentWindData(units::height h, vec2 wVel) { descentWindVels.addDataPoint(h, wVel); }
};
