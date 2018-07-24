#include "weatherdata.h"

void WeatherData::addDataPoint(datapoint dpoint) {
    auto [hght, pres, temp, wdir, wspd] = dpoint;

    pressures.addDataPoint(hght, pres);
    temperatures.addDataPoint(hght, temp);
    windVels.addDataPoint(hght, vec2::from_polar(double(wspd), wdir));
}

WeatherData::WeatherData(std::vector<datapoint> data_in) {
    for (auto dpoint : data_in) {
        addDataPoint(dpoint);
    }
}

void WeatherData::read(std::istream& is) {
    std::string datapoint;
    while (std::getline(is, datapoint) && datapoint != ".") {
        std::istringstream dp_s(datapoint);
        double hght; //Height (m)
        double pres; //Pressure (hPa)
        double temp; //Temperature (°C)
        double drct; //Wind direction (degrees true)
        double wspd; //Wind speed (m/s)

        dp_s >> hght >> pres >> temp >> drct >> wspd;

        addDataPoint({hght, pres * 100, temp + 273.15, drct, wspd});
    }
}

units::density WeatherData::densityAt(units::height h) {
    return pressureAt(h) / SPECIFIC_GAS_CONST / temperatureAt(h);
}

vec2 WeatherData::windVelAt(units::height h) {
    if (!ascentWindVels.empty() && h < ascentWindVels.getLastKey()) {
        return ascentWindVels.getAt(h);
    } else {
        return windVels.getAt(h);
    }
}
