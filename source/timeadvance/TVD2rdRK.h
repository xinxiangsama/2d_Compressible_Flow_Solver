#pragma once
#include "Explicit.h"
#include "../model/EulerMod.h"
#include <unordered_map>
class TVD2rdRK : public Explicit
{
public:
    TVD2rdRK() = default;
    virtual ~TVD2rdRK() = default;

    virtual void advance(Cell* cell, const int& step) override;
    void setTimeStep(const double& dt) override;
    const double& getTimeStep() const override;
private:
    std::unordered_map<Cell*, std::unique_ptr<Phase>> m_Phase;
    const std::array<std::array<double,3>, 2> coef = {{
        {0.0, 1.0, 1.0},
        {0.5, 0.5, 0.5}
    }};
};
extern std::shared_ptr<Model> model;