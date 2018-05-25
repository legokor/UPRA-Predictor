#include "flight.h"

BalloonFlight::BalloonFlight(BalloonProperties props, std::unique_ptr<WeatherData> wdata_):
    balloonProps{props}, wdata{std::move(wdata_)} {
    // TODO: density at 0m
    units::density density = wdata->pressureAt(0) / SPECIFIC_GAS_CONST / wdata->temperatureAt(0);

    units::length balloonGndDiam = 2 * pow(double((balloonProps.NECK_LIFT + balloonProps.BALLOON_DRY_MASS) / density / (4.0/3*M_PI)), 1.0/3);

    // TODO: Korrekció hélium tömegével

    ascentVel = sqrt(double(
        (balloonProps.NECK_LIFT - balloonProps.PARACHUTE_DRY_MASS - balloonProps.PAYLOAD_DRY_MASS)
        * 2.0 * G / ( density * balloonProps.BALLOON_DRAG_C * pow((double)balloonGndDiam,2)*M_PI/4 )
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
    // Levegősűrűség számítása a jelenlegi magasságon
    units::density density = wdata->pressureAt(h) / SPECIFIC_GAS_CONST / wdata->temperatureAt(h);

    return -sqrt(double( (balloonProps.PARACHUTE_DRY_MASS + balloonProps.PAYLOAD_DRY_MASS) * G / ( density * balloonProps.PARACHUTE_DRAG_C * balloonProps.PARACHUTE_AREA ) * 2 ));
}
