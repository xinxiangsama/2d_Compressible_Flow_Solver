#pragma once
#include "../meshes/Cell.h"
class Explicit
{
public:
    Explicit() = default;
    virtual ~Explicit() = default;

    //perform explicit time advance
    virtual void advance(Cell* cell, const int&) = 0;

    //set the time step size
    virtual void setTimeStep(const double& dt) = 0;

    //get the current time step size
    virtual const double& getTimeStep() const = 0;
protected:
    double m_dt; // time step size
};