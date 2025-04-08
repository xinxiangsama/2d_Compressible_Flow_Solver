#pragma once
#include "../meshes/Cell.h"
class Gradient
{
public:
    Gradient() = default;
    virtual ~Gradient() = default;

    // Modify
    virtual void setUGradient(const Coord&) {};
    virtual void setVGradient(const Coord&) {};
    virtual void setTGradient(const Coord&) {};
    
    virtual void setUGradient(const double& _Ud, const int& dim) {};
    virtual void setVGradient(const double& _Vd, const int& dim) {};
    virtual void setTGradient(const double& _Td, const int& dim) {};
    // Access
    virtual const Coord& getUGradient() const {};
    virtual const Coord& getVGradient() const {};
    virtual const Coord& getTGradient() const {};
};