#pragma once
#include "../meshes/Cell.h"
#include "./Largrangian.h"

class ImmersedBoundary
{
public:
    ImmersedBoundary() = default;
    virtual ~ImmersedBoundary() = default;
    // Methold
    virtual void Initialize(const size_t& numpoints) {};
    virtual void DistinguishWallandFluid(Cell* cell) {};
    virtual void SetGhostcell(std::vector<std::vector<Cell>>& cells, const int& num) {};
    virtual void SetImagepoint(Cell* cell, std::vector<std::vector<Cell>>& cells) {};
    // Access
    void SetnumLagrangianPoints(const size_t& numpoints) {numLagrangianPoints = numpoints;};
    // Modify

protected:
    size_t numLagrangianPoints;
    std::vector<LargrangianPoint> m_points;
    std::vector<Segment> m_segments;
protected:
    
};