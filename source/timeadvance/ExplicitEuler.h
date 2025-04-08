#pragma once
#include "Explicit.h"

class ExplicitEuler : public Explicit
{
public:
    ExplicitEuler() = default;
    virtual ~ExplicitEuler() = default;

    virtual void advance(Cell* cell, const int& step) override {};
    virtual void setTimeStep(const double& dt) override {};
    virtual const double& getTimeStep() const override {};
};