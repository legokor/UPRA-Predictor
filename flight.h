#pragma once
#include "units.hpp"
#include "dataset.hpp"
#include "balloonproperties.h"
#include "coords.h"
#include "vecs.h"


class BalloonFlight {
    /// Specific gas constant of dry air
    constexpr static const units::Quantity<2, 0, -2, -1> SPECIFIC_GAS_CONST = 287.058;
    /// Gravity
    constexpr static const units::acceleration G = 9.81;

    BalloonProperties balloonProps;

    /// Nyomás, a magasság függvényében (OMSZ adatból) Pascal
    Dataset<units::height, units::pressure> defPressures;
    /// Hőmérséklet, a magasság függvényében (OMSZ adatból) Kelvin
    Dataset<units::height, units::temperature> defTemperatures;
    /// Szélirány, szélsebesség, a magasság függvényében (OMSZ adatból) m/s, derékszögű koordinátarendzser
    Dataset<units::height, vec2> defWindVels;

    /// Emelkedés során mért tényleges széladatok
    Dataset<units::height, vec2> ascentWindVels;

    /// Ereszkedés során mért tényleges széladatok (egyelőre nem használt)
    Dataset<units::height, vec2> descentWindVels;

    /// Tényleges vett ballonadatok
    std::vector<std::pair<units::time, coords>> balloonData;

    /// Éppen emelkedünk-e
    bool isAscent;
    /// Emelkedési sebesség, repülés kezdetén OMSZ adatokból és saját előzetes adatokból becsült
    units::speed ascentVel;
    /// Várt ballon kidurranási magasság, repülés kezdetén OMSZ adatokból és saját előzetes adatokból becsült
    units::height expectedBurstAlt;
    /// Tényleges ballon kidurranási magasság
    units::height actualBurstAlt;

public:

    BalloonFlight(BalloonProperties props);

    /// Új mért adat felvétele
    void recieveBalloonData(units::time t, coords loc);

    /// Új predikciós adatpont számítása az előzőből és a rendelkezésre álló adatokból
    coords predictNext(coords lastData, units::time dt, bool isAscent);

    /// Új predikció készítése a legutóbbi tényleges mért adatból
    auto predict(std::pair<units::time, coords> startPoint, units::time timeStep);

private:
    /// Szélsebesség elkérése az adott magasságon
    vec2 getWindVel(units::height h);
    /// Emelkedési/süllyesési sebesség elkérése az adott magasságon
    units::speed getVerticalVel(bool ascent, units::height h);
};
