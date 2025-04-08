#pragma once
#include <Eigen/Dense>
#include "../meshes/Cell.h"
class ImagePoint
{
public:
    using Coord = Eigen::Vector2d;
    ImagePoint() = default;
    ~ImagePoint() = default;

    // Modify
    void setPosition(const Coord& position);
    void allocateVar();
    void insertCell(Cell* cell);

    // Access
    const Coord& getPosition() const { return m_positon; }
    std::unique_ptr<Phase>& getPhase() { return m_phase; }

    // Operator
    void InterpolatePhase();
protected:
    Coord m_positon;
    std::unique_ptr<Phase> m_phase;
    std::vector<Cell*> m_cells;
};

extern std::shared_ptr<Model> model;