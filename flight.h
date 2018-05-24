#pragma once
#include <memory>
#include "units.hpp"
#include "dataset.hpp"
#include "balloonproperties.h"
#include "weatherdata.h"
#include "coords.h"
#include "vecs.h"


class BalloonFlight {
    /// Specific gas constant of dry air
    constexpr static const units::Quantity<2, 0, -2, -1> SPECIFIC_GAS_CONST = 287.058;
    /// Gravity
    constexpr static const units::acceleration G = 9.81;

    BalloonProperties balloonProps;

    /// OMSZ légköri adatok
    std::unique_ptr<WeatherData> wdata;

   /// Tényleges vett ballonadatok
    std::vector<std::pair<units::time_point, coords>> balloonData;

    /// Éppen emelkedünk-e
    bool isAscent;
    /// Emelkedési sebesség, repülés kezdetén OMSZ adatokból és saját előzetes adatokból becsült
    units::speed ascentVel;
    /// Várt ballon kidurranási magasság, repülés kezdetén OMSZ adatokból és saját előzetes adatokból becsült
    units::height expectedBurstAlt;
    /// Tényleges ballon kidurranási magasság
    units::height actualBurstAlt;

public:

    BalloonFlight(BalloonProperties props, std::unique_ptr<WeatherData> wdata_);

    /// Új mért adat felvétele
    void recieveBalloonData(units::time_point t, coords loc);

    /// Új predikciós adatpont számítása az előzőből és a rendelkezésre álló adatokból
    coords predictNext(coords lastData, units::time dt, bool isAscent);

    /// Új predikció készítése a legutóbbi tényleges mért adatból
    auto predict(std::pair<units::time_point, coords> startPoint, units::time timeStep);

private:
    /// Emelkedési sebesség elkérése az adott magasságon
    units::speed getAscentVel(units::height h);
    /// Ereszkedési sebesség elkérése az adott magasságon
    units::speed getDescentVel(units::height h);
};
