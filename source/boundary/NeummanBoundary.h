#pragma once
#include "Boundary.h"

class NeummanBoundary : public Boundary
{
public:
    NeummanBoundary() = default;
    NeummanBoundary(const double& x, const double& y) {m_Normal = {x, y};}
    virtual ~NeummanBoundary() = default;
    virtual void SetGhostcells(CellInterface& cellinterface, std::vector<std::shared_ptr<Cell>>& ghostcells, std::vector<std::shared_ptr<CellInterface>>& ghostcellinterface) override;
    virtual void EnforceBoundary(CellInterface& cellinterface);

    // Modify
    virtual void setNormal(const Coord& _coord);
    // Access
    virtual const Coord& getNormal() const;
protected:

};