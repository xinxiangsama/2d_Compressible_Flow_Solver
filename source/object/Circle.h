#pragma once
#include "ImmersedBoundary.h"

class Circle : public ImmersedBoundary
{
public:
    Circle() = default;
    Circle(const double& radius, const Coord& center);
    ~Circle() = default;

    // operator
    void Initialize(const size_t& numpoints) override;
    void DistinguishWallandFluid(Cell* cell) override;
    void SetGhostcell(std::vector<std::vector<Cell>>& cells, const int& num) override;
    void SetImagepoint(Cell* cell, std::vector<std::vector<Cell>>& cells) override;
    virtual void FindNearest4Cell(ImagePoint* imagepoint, std::vector<std::vector<Cell>>& cells);
    void FindNearest4FluidCellsIndexed(ImagePoint* imagepoint, std::vector<std::vector<Cell>>& cells);
    // Modify
    void Setradius(const double& radius) {m_radius = radius;};
    void Setnumpoints(const int& numpoints) {numLagrangianPoints = numpoints;};
    // Access
    double Getradius() const {return m_radius;};
    int Getnumpoints() const {return numLagrangianPoints;};
protected:
    double m_radius;
    Coord m_center;
};