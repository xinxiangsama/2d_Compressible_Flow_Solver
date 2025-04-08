#pragma once
#include "../meshes/GhostCell.h"

class Boundary
{
public:
    Boundary() = default;
    Boundary(const double& x, const double& y) {m_Normal = {x, y};};
    virtual ~Boundary() = default;
    virtual void SetGhostcells(CellInterface&, std::vector<std::shared_ptr<Cell>>&, std::vector<std::shared_ptr<CellInterface>>& ) {};
    virtual void EnforceBoundary(CellInterface& cellinterface) {};

    // Modify
    virtual void setNormal(const Coord&) {};
    // Access
    virtual const Coord& getNormal() const {};
protected:
    Coord m_Normal;
};

extern std::shared_ptr<Model> model;
extern std::shared_ptr<Interpolation> interpolation;
extern std::shared_ptr<RiemmanSolver> riemmansolver;