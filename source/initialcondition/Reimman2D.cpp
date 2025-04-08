#include "Reimman2D.h"



void Reimman2D::fillIn(Cell *cell) const
{
    auto x = cell->getElement()->getPosition()[0];
    auto y = cell->getElement()->getPosition()[1];
    if(x < 1.6 && y < 1.6){
        cell->getPhase()->setDensity(77.0 / 558.0);
        cell->getPhase()->setU(77.0 / 558.0 * 4 / sqrt(11.0));
        cell->getPhase()->setV(77.0 / 558.0 * 4 / sqrt(11.0));
        cell->getPhase()->setPressure(9.0 / 310.0);
    }else if(x < 1.6 && y >= 1.6){
        cell->getPhase()->setDensity(33.0 / 62.0);
        cell->getPhase()->setU(33.0 / 62.0 * 4 / sqrt(11.0));
        cell->getPhase()->setV(0.0);
        cell->getPhase()->setPressure(0.3);
    }else if(x >= 1.6 && y < 1.6){
        cell->getPhase()->setDensity(33.0 / 62.0);
        cell->getPhase()->setU(0.0);
        cell->getPhase()->setV(33.0 / 62.0 * 4 / sqrt(11.0));
        cell->getPhase()->setPressure(0.3);   
    }else{
        cell->getPhase()->setDensity(1.5);
        cell->getPhase()->setU(0.0);
        cell->getPhase()->setV(0.0);
        cell->getPhase()->setPressure(1.5); 
    }
    cell->getPhase()->computeEnergyfromOther();

    // if(cell->isImmersedGhost()){
    //     cell->getPhase()->setDensity(999.0);
    //     cell->getPhase()->setU(999.0);
    //     cell->getPhase()->setV(999.0);
    //     cell->getPhase()->setPressure(999.0);
    //     cell->getPhase()->computeEnergyfromOther();
    // }
}

void Reimman2D::setBounbdaryCondition(std::vector<std::shared_ptr<Boundary>>& boundarys)
{
    // left 
    boundarys.push_back(std::make_shared<NeummanBoundary>(-1, 0));
    // right
    boundarys.push_back(std::make_shared<NeummanBoundary>(1, 0));
    // bottom
    boundarys.push_back(std::make_shared<NeummanBoundary>(0, -1));
    // top
    boundarys.push_back(std::make_shared<NeummanBoundary>(0, 1));
}
