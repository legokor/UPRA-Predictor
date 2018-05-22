#pragma once
#include "units.hpp"
#include "coords.h"

struct BalloonProperties {
    const units::mass BALLOON_DRY_MASS;
    const units::mass PARACHUTE_DRY_MASS;
    const units::mass PAYLOAD_DRY_MASS;
    const units::mass NECK_LIFT;
    const units::area PARACHUTE_AREA;
    const double PARACHUTE_DRAG_C;
    double BALLOON_DRAG_C;
    const units::length DESIGN_BURST_DIAM;
    units::mass DESCENT_MASS;

    BalloonProperties(double balloonDryMass, double chuteDryMass, double payloadDryMass,
                      double neckLift, double parachuteArea,
                      double parachuteDrag, double balloonDrag, double burstDiam):
                      BALLOON_DRY_MASS{balloonDryMass}, PARACHUTE_DRY_MASS{chuteDryMass},
                      PAYLOAD_DRY_MASS{payloadDryMass}, NECK_LIFT{neckLift},
                      PARACHUTE_AREA{parachuteArea}, PARACHUTE_DRAG_C{parachuteDrag},
                      BALLOON_DRAG_C{balloonDrag}, DESIGN_BURST_DIAM{burstDiam},
                      DESCENT_MASS{PARACHUTE_DRY_MASS + PAYLOAD_DRY_MASS} {}

    // TODO: Burst altitude calcuation

};
