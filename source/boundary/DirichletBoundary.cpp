#include "DirichletBoundary.h"

void DirichletBoundary::SetGhostcells(CellInterface& cellinterface,
                                      std::vector<std::shared_ptr<Cell>>& ghostcells,
                                      std::vector<std::shared_ptr<CellInterface>>& ghostcellinterface)
{
    auto direction = m_Normal[0] + m_Normal[1];  // only one of them non-zero
    bool isHorizontal = cellinterface.getHorizontal(); // true: x-interface

    auto ghost = std::make_shared<GhostCell>();
    ghost->setGhost();
    ghost->getCellInterfaces().resize(4);

    auto newInterface = std::make_shared<CellInterface>();
    ghostcellinterface.push_back(newInterface);

    if (direction < 0.0) {
        // ghost 在 left / bottom
        cellinterface.setLeftCell(ghost.get());
        newInterface->setRightCell(ghost.get());
        ghost->setCellInterface(isHorizontal ? 0 : 2, newInterface.get());
    } else {
        // ghost 在 right / top
        cellinterface.setRightCell(ghost.get());
        newInterface->setLeftCell(ghost.get());
        ghost->setCellInterface(isHorizontal ? 1 : 3, newInterface.get());
    }

    ghostcells.push_back(ghost);
}

void DirichletBoundary::EnforceBoundary(CellInterface& cellinterface)
{
    auto direction = m_Normal[0] + m_Normal[1];
    Cell* ghostCell = nullptr;

    if (direction < 0.0) {
        ghostCell = cellinterface.getCellLeft();
    } else {
        ghostCell = cellinterface.getCellRight();
    }

    // 设置 ghost cell 的状态为指定的边界值
    m_SetPhaseFunc(*ghostCell->getPhase());
    ghostCell->getPhase()->computeEnergyfromOther(); // 确保能量同步
}
