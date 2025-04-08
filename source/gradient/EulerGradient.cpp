#include "EulerGradient.h"

void EulerGradient::setUGradient(const Coord &_Ud)
{
    U_d = _Ud;
}

void EulerGradient::setVGradient(const Coord &_Vd)
{
    V_d = _Vd;
}

void EulerGradient::setTGradient(const Coord &_Td)
{
    T_d = _Td;
}

void EulerGradient::setUGradient(const double &_Ud, const int &dim)
{
    U_d[dim] = _Ud;
}

void EulerGradient::setVGradient(const double &_Vd, const int &dim)
{
    V_d[dim] = _Vd;
}

void EulerGradient::setTGradient(const double &_Td, const int &dim)
{
    T_d[dim] = _Td;
}

const Coord &EulerGradient::getUGradient() const
{
    return U_d;
}

const Coord &EulerGradient::getVGradient() const
{
    return V_d;
}

const Coord &EulerGradient::getTGradient() const
{
    return T_d;
}