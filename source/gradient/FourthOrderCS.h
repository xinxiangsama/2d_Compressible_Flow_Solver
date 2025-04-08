#pragma once
#include <array>
#include <list>
#include "../meshes/Cell.h"
#include "../param.h"
#include "../meshes/CellInterface.h"

class CellInterface;
class Cell;
class ForthOrderCS
{
public:
    ForthOrderCS() = default;
    virtual ~ForthOrderCS() = default;
    virtual void comviscousfluxForcellinterface(CellInterface* cellinterface, std::list<Cell*>& stencil, Direction dir);
    virtual void comprimitivederivativeForcell(Cell* cell, std::vector<Cell*>& stencil, Direction dir);
    virtual void comviscousflux() {};
    virtual void constructflux() {};
    double Fouthcs(const std::array<double,5>& v);

    // Access
    const int& getNeedCellnum ();
protected:
    static constexpr int cellNum{6};
};

// ϑ^r_l for three interface points (r = i-3/2, i-1/2, i+1/2), l in [-3,2]
static const std::array<std::array<double,6>, 3> theta_r = {{
    {71.0/1920, -141.0/128, 69.0/64, 1.0/192, -3.0/128, 3.0/640},   // i-3/2
    {-3.0/640, 25.0/384, -75.0/64, 75.0/64, -25.0/384, 3.0/640},    // i-1/2
    {-3.0/640, 3.0/128, -1.0/192, -69.0/64, 141.0/128, -71.0/1920}  // i+1/2
}};

static const std::array<double, 5> theta_c = {
    1.0/12, -8.0/12, 0.0, 8.0/12, -1.0/12
};

static const std::array<std::array<double,4>, 3> zeta_r = {{
    {5.0/16, 15.0/16, -15.0/16, 1.0/16},   // i-3/2
    {-1.0/16, 9.0/16, 9.0/16, -1.0/16},    // i-1/2
    {1.0/16, -5.0/16, 15.0/16, 5.0/16}     // i+1/2
}};