#pragma once
#include "TVD2rdRK.h"

class TVD3rdRK : public TVD2rdRK
{
public:
    TVD3rdRK() = default;
    virtual ~TVD3rdRK() = default;

    virtual void advance(Cell* cell, const int& step) override;
private:
    std::unordered_map<Cell*, std::unique_ptr<Phase>> m_Phase0;
    std::unordered_map<Cell*, std::unique_ptr<Phase>> m_Phase1;
    const std::array<std::array<double, 3>, 3> coef = {{
        {0.0, 1.0, 1.0},
        {0.75, 0.25, 0.25},
        {1.0 / 3.0, 2.0 / 3.0, 2.0 / 3.0}
    }};
};