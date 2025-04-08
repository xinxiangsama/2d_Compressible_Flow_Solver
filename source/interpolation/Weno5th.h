#pragma once

#include "Interpolation.h"
#include <Eigen/Core>
class Weno5th : public Interpolation
{
public:
    Weno5th() = default;
    virtual ~Weno5th() = default;
    virtual void comLeftPhase(std::list<Cell*>& cells, std::unique_ptr<Phase>& phase) override;
    virtual void comRightPhase(std::list<Cell*>& cells, std::unique_ptr<Phase>& phase) override;
    virtual void comInterpolation(std::array<Cell*,5>& cells, std::unique_ptr<Phase>& phase);
    inline double weno5Interpolation(const std::array<double,5>& v);
};