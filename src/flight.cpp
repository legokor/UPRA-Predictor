#include "flight.h"

BalloonFlight::BalloonFlight(BalloonProperties props, std::unique_ptr<WeatherData> wdata_):
    balloonProps{props}, wdata{std::move(wdata_)}, isAscent{true} {
    // TODO: density at 0m
    units::density airGndDensity = wdata->airDensityAt(0);

    units::density heliumGndDensity = wdata->heliumDensityAt(0);

    units::volume balloonGndVolume = (balloonProps.nozzle_lift + balloonProps.balloon_dry_mass) / (airGndDensity * (1.0 - double(WeatherData::H_MOLAR_MASS / WeatherData::AIR_MOLAR_MASS)));
    units::length balloonGndDiam = 2 * pow(double(balloonGndVolume *3/4/M_PI), 1.0/3.0);

    units::mass heliumMass = balloonGndVolume * heliumGndDensity;

    units::volume balloonBurstVolume = pow(double(balloonProps.design_burst_diam / 2), 3) *4/3*M_PI;

    units::density burstGasDensity = heliumMass / balloonBurstVolume;

    expectedBurstAlt = WeatherData::ATMOSPHERE_SCALE_HEIGHT * log(double(heliumGndDensity / burstGasDensity));

    //units::length balloonGndDiam = 2 * pow(double((balloonProps.nozzle_lift + balloonProps.balloon_dry_mass) / airGndDensity / (4.0/3*M_PI)), 1.0/3);

    ascentVel = sqrt(double(
        (balloonProps.nozzle_lift - balloonProps.parachute_dry_mass - balloonProps.payload_dry_mass) * 2.0 * G
        /
        ( airGndDensity * balloonProps.balloon_drag_c * pow((double)balloonGndDiam,2)*M_PI/4 )
    ));
}


void BalloonFlight::recieveBalloonData(units::time_point t, coords loc) {
    if (!balloonData.empty()) {
        coords prevLoc = balloonData.back().second;
        units::time dt = units::time(t - balloonData.back().first);
        // Elmozdulás a leutóbbi mért helyzethez képest
        vec3 locDelta = loc - prevLoc;
        if (isAscent && loc.alt < prevLoc.alt) {
            actualBurstAlt = (units::height) prevLoc.alt;
            isAscent = false;
        }
        if (isAscent) wdata->addAscentWindData(prevLoc.alt, vec2(locDelta.x, locDelta.y) / (double)dt);
        else wdata->addDescentWindData(prevLoc.alt, vec2(locDelta.x, locDelta.y) / (double)dt);
    }
    balloonData.push_back(std::make_pair(t, loc));
}

coords BalloonFlight::predictNext(coords lastLoc, units::time dt, bool isAscent) {
    coords newLoc = lastLoc + wdata->windVelAt(lastLoc.alt) * (double) dt;
    newLoc.alt = newLoc.alt + (isAscent ? getAscentVel(lastLoc.alt) : getDescentVel(lastLoc.alt)) * dt;
    return newLoc;
}

std::vector<std::pair<units::time_point, coords>> BalloonFlight::predict(units::time timeStep) {
    std::vector<std::pair<units::time_point, coords>> prediction;
    auto currentPoint = balloonData.back(); //Start from last recieved location
    bool isSimulatedAscent = isAscent;

    while (isSimulatedAscent || currentPoint.second.alt > units::height(0.0)) {
        coords newLoc = predictNext(currentPoint.second, timeStep, isSimulatedAscent);
        if (isSimulatedAscent && newLoc.alt >= expectedBurstAlt) {
            isSimulatedAscent = false;
        }
        currentPoint = std::make_pair(currentPoint.first + timeStep, newLoc);
        prediction.push_back(currentPoint);
    }
    return prediction;
}

units::speed BalloonFlight::getAscentVel(units::height h) {
    return ascentVel;
}

units::speed BalloonFlight::getDescentVel(units::height h) {
    return -sqrt(double(
        (balloonProps.parachute_dry_mass + balloonProps.payload_dry_mass) * G
        /
        ( wdata->airDensityAt(h) * balloonProps.parachute_drag_c * balloonProps.parachute_area ) * 2
    ));
}
