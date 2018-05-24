#include "balloonproperties.h"

BalloonProperties::BalloonProperties(std::vector<std::string> values) {
    if (values.size() != 8)
        throw std::invalid_argument("Too few/many balloon properties");
    BalloonProperties(
        std::stod(values[0]),
        std::stod(values[1]),
        std::stod(values[2]),
        std::stod(values[3]),
        std::stod(values[4]),
        std::stod(values[5]),
        std::stod(values[6]),
        std::stod(values[7])
    );
}

void BalloonProperties::setProp(std::string propName, double value) {
    if (propName == "BALLOON_DRY_MASS") {
        BALLOON_DRY_MASS = value;
    } else if (propName == "PARACHUTE_DRY_MASS") {
        PARACHUTE_DRY_MASS = value;
    } else if (propName == "PAYLOAD_DRY_MASS") {
        PAYLOAD_DRY_MASS = value;
    } else if (propName == "NECK_LIFT") {
        NECK_LIFT = value;
    } else if (propName == "PARACHUTE_AREA") {
        PARACHUTE_AREA = value;
    } else if (propName == "PARACHUTE_DRAG_C") {
        PARACHUTE_DRAG_C = value;
    } else if (propName == "BALLOON_DRAG_C") {
        BALLOON_DRAG_C = value;
    } else if (propName == "DESIGN_BURST_DIAM") {
        DESIGN_BURST_DIAM = value;
    } else throw std::invalid_argument("No balloon property named " + propName);
}
