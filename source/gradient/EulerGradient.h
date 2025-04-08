#pragma once
#include "./Gradient.h"
class EulerGradient : public Gradient
{
public:
    EulerGradient() = default;
    virtual ~EulerGradient() = default;

    // Modify
    virtual void setUGradient(const Coord& _Ud);
    virtual void setVGradient(const Coord& _Vd);
    virtual void setTGradient(const Coord& _Td);

    virtual void setUGradient(const double& _Ud, const int& dim);
    virtual void setVGradient(const double& _Vd, const int& dim);
    virtual void setTGradient(const double& _Td, const int& dim);
    // Access
    virtual const Coord& getUGradient() const;
    virtual const Coord& getVGradient() const;
    virtual const Coord& getTGradient() const;
protected:
    Coord U_d; // partial u / partial x, partial u / partial y;
    Coord V_d;
    Coord T_d;
};