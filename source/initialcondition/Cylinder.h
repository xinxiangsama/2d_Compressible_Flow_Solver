#pragma once
#include "./Initialcondition.h"

class Cylinder : public Initialcondition
{
public:
    Cylinder() = default;
    ~Cylinder() = default;
    void fillIn(Cell* cell) const override;
    void setBounbdaryCondition(std::vector<std::shared_ptr<Boundary>>& boundary) override;

};

extern std::shared_ptr<Model> model;