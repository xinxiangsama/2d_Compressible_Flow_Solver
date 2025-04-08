#pragma once
#include "Boundary.h"
#include <functional>

class DirichletBoundary : public Boundary
{
public:
    using DirichletFunc = std::function<void(Phase&)>;

    DirichletBoundary() = default;
    DirichletBoundary(const double& x, const double& y, DirichletFunc func)
        : m_SetPhaseFunc(func) { m_Normal = {x, y}; }

    virtual ~DirichletBoundary() = default;

    virtual void SetGhostcells(CellInterface& cellinterface,
                               std::vector<std::shared_ptr<Cell>>& ghostcells,
                               std::vector<std::shared_ptr<CellInterface>>& ghostcellinterface) override;

    virtual void EnforceBoundary(CellInterface& cellinterface) override;

    void setNormal(const Coord& _coord) { m_Normal = _coord; }
    const Coord& getNormal() const { return m_Normal; }

private:
    DirichletFunc m_SetPhaseFunc;
};
