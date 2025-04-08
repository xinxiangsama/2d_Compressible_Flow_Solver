#pragma once
#include "Cell.h"

class GhostCell : public Cell
{
public:
    GhostCell() = default;
    virtual ~GhostCell() = default;
    virtual bool isCellGhost() const { return true; };

protected:
    int m_rankOfNeighborCPU; /*!< Rank of the neighbor CPU corresponding to this ghost cell */
};