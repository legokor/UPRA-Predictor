#pragma once
#include <map>
#include "units.hpp"
#include "coords.h"

struct BalloonProperties {
    units::mass balloon_dry_mass;
    units::mass parachute_dry_mass;
    units::mass payload_dry_mass;
    units::mass nozzle_lift;
    units::area parachute_area;
    double parachute_drag_c;
    double balloon_drag_c;
    units::length design_burst_diam;

    BalloonProperties(double balloonDryMass, double chuteDryMass, double payloadDryMass,
                      double neckLift, double parachuteArea,
                      double parachuteDrag, double balloonDrag, double burstDiam);

    BalloonProperties(std::vector<std::string> values);

    double getProp(std::string propName);
    void setProp(std::string propName, double value);
    // TODO: Burst altitude calcuation

};
