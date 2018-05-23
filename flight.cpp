#include "flight.h"

BalloonFlight::BalloonFlight(BalloonProperties props): balloonProps{props} {
    // TODO: density at 0m
    units::density density = defPressures.getAt(0) / SPECIFIC_GAS_CONST / defTemperatures.getAt(0);

    units::length balloonGndDiam = 2 * pow(double((balloonProps.NECK_LIFT + balloonProps.BALLOON_DRY_MASS) / density / (4.0/3*M_PI)), 1.0/3);

    // TODO: Korrekció hélium tömegével

    ascentVel = sqrt(double(
        (balloonProps.NECK_LIFT - balloonProps.PARACHUTE_DRY_MASS - balloonProps.PAYLOAD_DRY_MASS) * 2.0
        * G / ( density * balloonProps.BALLOON_DRAG_C * pow((double)balloonGndDiam,2)*M_PI/4 ) ));
}


void BalloonFlight::recieveBalloonData(time_point t, coords loc) {
    coords prevLoc = balloonData.back().second;
    units::time dt = t - balloonData.back().first;
    // Elmozdulás a leutóbbi mért helyzethez képest
    vec3 locDelta = loc - prevLoc;
    if (isAscent && loc.alt < prevLoc.alt) {
        actualBurstAlt = (units::height) prevLoc.alt;
        isAscent = false;
    }
    if (isAscent) ascentWindVels.addDataPoint(prevLoc.alt, vec2(locDelta.x, locDelta.y) / (double)dt);
    else descentWindVels.addDataPoint(prevLoc.alt, vec2(locDelta.x, locDelta.y) / (double)dt);
    balloonData.push_back(std::make_pair(t, loc));
}

coords BalloonFlight::predictNext(coords lastLoc, units::time dt, bool isAscent) {
    coords newLoc = lastLoc + getWindVel(lastLoc.alt) * (double) dt;
    newLoc.alt = newLoc.alt + getVerticalVel(isAscent, lastLoc.alt) * dt;
    return newLoc;
}

auto BalloonFlight::predict(std::pair<units::time, coords> startPoint, units::time timeStep) {
    std::vector<std::pair<units::time, coords>> prediction;
    auto currentPoint = startPoint;
    bool isSimulatedAscent = isAscent;

    while (isSimulatedAscent || currentPoint.second.alt > units::height(0.0)) {
        coords newLoc = predictNext(currentPoint.second, timeStep, isSimulatedAscent);
        if (isSimulatedAscent && newLoc.alt >= expectedBurstAlt) {
            isSimulatedAscent = false;
        }
        currentPoint = std::make_pair(startPoint.first + timeStep, newLoc);
        prediction.push_back(currentPoint);
    }
    return prediction;
}

vec2 BalloonFlight::getWindVel(units::height h) {
    if (h < ascentWindVels.getLastKey()) {
        return ascentWindVels.getAt(h);
    } else {
        return defWindVels.getAt(h);
    }
}

units::speed BalloonFlight::getVerticalVel(bool ascent, units::height h) {
    if (ascent) {
        return ascentVel;
    } else {
        // Levegősűrűség számítása a jelenlegi magasságon
        units::density density = defPressures.getAt(h) / SPECIFIC_GAS_CONST / defTemperatures.getAt(h);

        return -sqrt(double( balloonProps.DESCENT_MASS * G / ( density * balloonProps.PARACHUTE_DRAG_C * balloonProps.PARACHUTE_AREA ) * 2 ));
    }
}
