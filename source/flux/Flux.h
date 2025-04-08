#pragma once
#include "../model/Phase.h"

class Flux
{
public:
    using Coord = Eigen::Vector2d;
    Flux() = default;
    virtual ~Flux() = default;

    // Modify
    virtual void SetToZero(){}; // subclass must has one !
    virtual void setMass(const double& ) {};
    virtual void setMomentum(const Coord&) {};
    virtual void setEnergy(const double&) {};
    // Access
    virtual const double& getMass() const {};
    virtual const Coord& getMomentum() const {};
    virtual const double& getEnergy() const {};
    // Operator
    virtual Flux& operator+(const Flux& other){};
    virtual Flux& operator-(const Flux& other){};
    virtual Flux& operator*(const double& scalar){};
    virtual Flux& operator/(const double& scalar){};
    virtual void operator=(const Flux& other) {};

protected:

};