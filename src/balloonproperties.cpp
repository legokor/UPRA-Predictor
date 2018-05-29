#include "balloonproperties.h"

BalloonProperties::BalloonProperties(double balloonDryMass, double chuteDryMass, double payloadDryMass,
    double neckLift, double parachuteArea,
    double parachuteDrag, double balloonDrag, double burstDiam):
    BALLOON_DRY_MASS{balloonDryMass}, PARACHUTE_DRY_MASS{chuteDryMass},
    PAYLOAD_DRY_MASS{payloadDryMass}, NECK_LIFT{neckLift},
    PARACHUTE_AREA{parachuteArea}, PARACHUTE_DRAG_C{parachuteDrag},
    BALLOON_DRAG_C{balloonDrag}, DESIGN_BURST_DIAM{burstDiam} {
#ifdef DEBUG
    std::cerr << "BPROPS: " << balloonDryMass << chuteDryMass << payloadDryMass << neckLift << parachuteArea
    << parachuteDrag << balloonDrag << burstDiam << std::endl;
#endif
}

BalloonProperties::BalloonProperties(std::vector<std::string> values) {
    if (values.size() != 8)
        throw std::invalid_argument("Too few/many balloon properties");

    BALLOON_DRY_MASS = std::stod(values[0]);
    PARACHUTE_DRY_MASS = std::stod(values[1]);
    PAYLOAD_DRY_MASS = std::stod(values[2]);
    NECK_LIFT = std::stod(values[3]);
    PARACHUTE_AREA = std::stod(values[4]);
    PARACHUTE_DRAG_C = std::stod(values[5]);
    BALLOON_DRAG_C = std::stod(values[6]);
    DESIGN_BURST_DIAM = std::stod(values[7]);

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
