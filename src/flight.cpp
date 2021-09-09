#include "flight.h"

BalloonFlight::BalloonFlight(BalloonProperties props, std::unique_ptr<WeatherData> wdata_):
    balloonProps{props}, wdata{std::move(wdata_)}, isAscent{true} {

    units::density airGndDensity = wdata->airDensityAt(0);

    units::density gndLiftingGasDensity = wdata->gasDensityAt(balloonProps.lifting_gas_molar_mass, 0);

    units::volume balloonGndVolume = (balloonProps.nozzle_lift + balloonProps.balloon_dry_mass) / (airGndDensity * (1.0 - double(balloonProps.lifting_gas_molar_mass / WeatherData::AIR_MOLAR_MASS)));
    units::length balloonGndDiam = 2 * pow(double(balloonGndVolume *3/4/M_PI), 1.0/3.0);

    units::mass liftingGasMass = balloonGndVolume * gndLiftingGasDensity;

    units::volume balloonBurstVolume = pow(double(balloonProps.design_burst_diam / 2), 3) *4/3*M_PI;

    units::density burstGasDensity = liftingGasMass / balloonBurstVolume;

    expectedBurstAlt = WeatherData::ATMOSPHERE_SCALE_HEIGHT * log(double(gndLiftingGasDensity / burstGasDensity));

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

void BalloonFlight::scheduleBalloonPropChangeAtBurst(std::string propName, double value) {
    balloonPropChangesAtBurst.push_back({propName, value});
}

coords BalloonFlight::predictNext(BalloonProperties props, coords lastLoc, units::time dt, bool isAscent) {
    coords newLoc = lastLoc + wdata->windVelAt(lastLoc.alt) * (double) dt;
    newLoc.alt = newLoc.alt + (isAscent ? getAscentVel(props, lastLoc.alt) : getDescentVel(props, lastLoc.alt)) * dt;
    return newLoc;
}

std::vector<std::pair<units::time_point, coords>> BalloonFlight::predict(units::time timeStep) {
    std::vector<std::pair<units::time_point, coords>> prediction;
    auto currentPoint = balloonData.back(); //Start from last recieved location
    bool isSimulatedAscent = isAscent;
    BalloonProperties simulatedBalloonProps = balloonProps;

    while (isSimulatedAscent || currentPoint.second.alt > units::height(0.0)) {
        coords newLoc = predictNext(simulatedBalloonProps, currentPoint.second, timeStep, isSimulatedAscent);
        if (isSimulatedAscent && newLoc.alt >= expectedBurstAlt) {
            isSimulatedAscent = false;
            for (auto [propName, value] : balloonPropChangesAtBurst) {
                simulatedBalloonProps.setProp(propName, value);
            }
        }
        currentPoint = std::make_pair(currentPoint.first + timeStep, newLoc);
        prediction.push_back(currentPoint);
    }
    return prediction;
}

units::speed BalloonFlight::getAscentVel(BalloonProperties props, units::height h) {
    return ascentVel;
}

units::speed BalloonFlight::getDescentVel(BalloonProperties props, units::height h) {
    return -sqrt(double(
        (props.parachute_dry_mass + props.payload_dry_mass) * G
        /
        ( wdata->airDensityAt(h) * props.parachute_drag_c * props.parachute_area ) * 2
    ));
}
