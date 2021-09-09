#include "balloonproperties.h"

BalloonProperties::BalloonProperties(double balloonDryMass, double chuteDryMass, double payloadDryMass,
    double neckLift, double parachuteArea,
    double parachuteDrag, double balloonDrag, double burstDiam, double liftingGasMM):
    balloon_dry_mass{balloonDryMass}, parachute_dry_mass{chuteDryMass},
    payload_dry_mass{payloadDryMass}, nozzle_lift{neckLift},
    parachute_area{parachuteArea}, parachute_drag_c{parachuteDrag},
    balloon_drag_c{balloonDrag}, design_burst_diam{burstDiam}, lifting_gas_molar_mass{liftingGasMM} {
#ifdef DEBUG
    //std::cerr << "BPROPS: " << balloonDryMass << chuteDryMass << payloadDryMass << neckLift << parachuteArea
    //<< parachuteDrag << balloonDrag << burstDiam << std::endl;
#endif
}

BalloonProperties::BalloonProperties(std::vector<std::string> values) {
    if (values.size() != 9)
        throw std::invalid_argument("Too few/many balloon properties");

    balloon_dry_mass = std::stod(values[0]);
    parachute_dry_mass = std::stod(values[1]);
    payload_dry_mass = std::stod(values[2]);
    nozzle_lift = std::stod(values[3]);
    parachute_area = std::stod(values[4]);
    parachute_drag_c = std::stod(values[5]);
    balloon_drag_c = std::stod(values[6]);
    design_burst_diam = std::stod(values[7]);
    lifting_gas_molar_mass = std::stod(values[8]);
}

// FIXME: Figure out a way to use std::map instead of this monstrosity
double BalloonProperties::getProp(std::string propName) {
    if (propName == "balloon_dry_mass") {
        return (double) balloon_dry_mass;
    } else if (propName == "parachute_dry_mass") {
        return (double) parachute_dry_mass;
    } else if (propName == "payload_dry_mass") {
        return (double) payload_dry_mass;
    } else if (propName == "nozzle_lift") {
        return (double) nozzle_lift;
    } else if (propName == "parachute_area") {
        return (double) parachute_area;
    } else if (propName == "parachute_drag_c") {
        return (double) parachute_drag_c;
    } else if (propName == "balloon_drag_c") {
        return (double) balloon_drag_c;
    } else if (propName == "design_burst_diam") {
        return (double) design_burst_diam;
    } else if (propName == "lifting_gas_molar_mass") {
        return (double) lifting_gas_molar_mass;
    } else throw std::invalid_argument("No balloon property named " + propName);

}

void BalloonProperties::setProp(std::string propName, double value) {
    if (propName == "balloon_dry_mass") {
        balloon_dry_mass = value;
    } else if (propName == "parachute_dry_mass") {
        parachute_dry_mass = value;
    } else if (propName == "payload_dry_mass") {
        payload_dry_mass = value;
    } else if (propName == "nozzle_lift") {
        nozzle_lift = value;
    } else if (propName == "parachute_area") {
        parachute_area = value;
    } else if (propName == "parachute_drag_c") {
        parachute_drag_c = value;
    } else if (propName == "balloon_drag_c") {
        balloon_drag_c = value;
    } else if (propName == "design_burst_diam") {
        design_burst_diam = value;
    } else if (propName == "lifting_gas_molar_mass") {
        lifting_gas_molar_mass = value;
    } else throw std::invalid_argument("No balloon property named " + propName);
}
