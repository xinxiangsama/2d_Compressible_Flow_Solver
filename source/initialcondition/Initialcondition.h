#pragma once
#include "../model/EulerMod.h"
#include "../meshes/Cell.h"
#include "../boundary/NeummanBoundary.h"
#include "../boundary/DirichletBoundary.h"
class Initialcondition
{
public:
    Initialcondition() = default;
    virtual ~Initialcondition() = default;
    virtual void fillIn(Cell* cell) const {};
    virtual void setBounbdaryCondition(std::vector<std::shared_ptr<Boundary>>&) = 0;
protected:
    std::vector<std::vector<std::unique_ptr<Phase>>> m_phase;
};