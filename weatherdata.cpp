#include "weatherdata.h"

WeatherData::WeatherData(std::string filename) {
    std::ifstream datafile(filename);

    for (std::string datapoint; std::getline(datafile, datapoint);) {
        std::istringstream dp_s(datapoint);
        double pres; //Pressure (hPa)
        double hght; //Height (m)
        double temp; //Temperature (°C)
        double drct; //Wind direction (degrees)
        double sknt; //Wind speed (knots)

        dp_s >> pres >> hght >> temp >> drct >> sknt;

        pressures.addDataPoint(hght, pres * 100);
        temperatures.addDataPoint(hght, temp + 273.15);
        windVels.addDataPoint(hght, vec2::from_polar(sknt * 0.51444444444444, drct));
    }
}
