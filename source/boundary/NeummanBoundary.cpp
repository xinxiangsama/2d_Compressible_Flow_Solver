#include "NeummanBoundary.h"

void NeummanBoundary::SetGhostcells(CellInterface& cellinterface, std::vector<std::shared_ptr<Cell>>& ghostcells, std::vector<std::shared_ptr<CellInterface>>& ghostcellinterface)
{
    auto direction = m_Normal[0] + m_Normal[1];  // only one of them non-zero
    bool isHorizontal = cellinterface.getHorizontal(); // horizontal: x-direction

    auto ghost = std::make_shared<GhostCell>();
    ghost->setGhost();
    ghost->getCellInterfaces().resize(4);

    // Allocate new interface between ghost and fluid cell
    auto newInterface = std::make_shared<CellInterface>();
    ghostcellinterface.push_back(newInterface);
    if (direction < 0.0) {
        // ghost is on the 'left' or 'bottom' side
        cellinterface.setLeftCell(ghost.get());
        newInterface->setRightCell(ghost.get());
        ghost->setCellInterface(isHorizontal ? 0 : 2, newInterface.get()); 
    } else {
        // ghost is on the 'right' or 'top' side
        cellinterface.setRightCell(ghost.get());
        newInterface->setLeftCell(ghost.get());
        ghost->setCellInterface(isHorizontal ? 1 : 3, newInterface.get()); 
    }

    ghostcells.push_back(ghost);
}


void NeummanBoundary::EnforceBoundary(CellInterface& cellinterface)
{
    auto direction {m_Normal[0] + m_Normal[1]};
    if(direction < 0.0){
        cellinterface.getCellLeft()->CpyPhase(cellinterface.getCellRight()->getPhase().get());
        if (cellinterface.getCellLeft()->getPhase()->getDensity() == 0) {
            // std::cerr << "Warning: Phase value is 0 during boundary enforcement." << std::endl;
        }
    }else{
        cellinterface.getCellRight()->CpyPhase(cellinterface.getCellLeft()->getPhase().get());
        if (cellinterface.getCellRight()->getPhase()->getDensity() == 0) {
            std::cerr << "Warning: Phase value is 0 during boundary enforcement." << std::endl;
        }
    }
}

void NeummanBoundary::setNormal(const Coord &_coord)
{
    m_Normal = _coord;
}

const Coord &NeummanBoundary::getNormal() const
{
    return m_Normal;
}