#pragma once
#include "Initialcondition.h"


class Reimman2D : public Initialcondition
{
public:
    Reimman2D() = default;
    // Reimman2D()
    virtual ~Reimman2D() = default;
    virtual void fillIn(Cell* cell) const;
    virtual void setBounbdaryCondition(std::vector<std::shared_ptr<Boundary>>& boundarys);
    protected:
};

extern std::shared_ptr<Model> model;
