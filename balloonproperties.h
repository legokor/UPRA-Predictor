#pragma once
#include "units.hpp"
#include "coords.h"

struct BalloonProperties {
    units::mass BALLOON_DRY_MASS;
    units::mass PARACHUTE_DRY_MASS;
    units::mass PAYLOAD_DRY_MASS;
    units::mass NECK_LIFT;
    units::area PARACHUTE_AREA;
    double PARACHUTE_DRAG_C;
    double BALLOON_DRAG_C;
    units::length DESIGN_BURST_DIAM;

    BalloonProperties(double balloonDryMass, double chuteDryMass, double payloadDryMass,
                      double neckLift, double parachuteArea,
                      double parachuteDrag, double balloonDrag, double burstDiam):
                      BALLOON_DRY_MASS{balloonDryMass}, PARACHUTE_DRY_MASS{chuteDryMass},
                      PAYLOAD_DRY_MASS{payloadDryMass}, NECK_LIFT{neckLift},
                      PARACHUTE_AREA{parachuteArea}, PARACHUTE_DRAG_C{parachuteDrag},
                      BALLOON_DRAG_C{balloonDrag}, DESIGN_BURST_DIAM{burstDiam} {}

    BalloonProperties(std::vector<std::string> values);

    void setProp(std::string propName, double value);
    // TODO: Burst altitude calcuation

};
