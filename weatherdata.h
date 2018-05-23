#include <iostream>
#include <fstream>
#include <sstream>
#include "dataset.hpp"
#include "units.hpp"

class WeatherData {
    /// Nyomás, a magasság függvényében (OMSZ adatból) Pascal
    Dataset<units::height, units::pressure> pressures;
    /// Hőmérséklet, a magasság függvényében (OMSZ adatból) Kelvin
    Dataset<units::height, units::temperature> temperatures;
    /// Szélirány, szélsebesség, a magasság függvényében (OMSZ adatból) m/s, derékszögű koordinátarendzser
    Dataset<units::height, vec2> windVels;

public:
    WeatherData(std::string filename);

    units::pressure pressureAt(units::height h) { return pressures.getAt(h); }
    units::temperature temperatureAt(units::height h) { return temperatures.getAt(h); }
    vec2 windVelAt(units::height h) { return windVels.getAt(h); }

};
