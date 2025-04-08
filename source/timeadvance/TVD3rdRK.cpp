#include "TVD3rdRK.h"

void TVD3rdRK::advance(Cell *cell, const int &step)
{
    auto& phase0 = m_Phase0[cell];
    auto& phase1 = m_Phase1[cell];
    auto& cellphase = cell->getPhase();
    auto rhs {std::move(cell->comRHS())};
    if(!phase0){
        model->allocatePhase(phase0);
    }
    if(!phase1){
        model->allocatePhase(phase1);
    }
    int stage = (step % 3 == 0) ? 2 : (step % 3 == 1) ? 0 : 1;

    if(stage == 0){
        phase0->setDensity(cell->getPhase()->getDensity());
        phase0->setU(cell->getPhase()->getU());
        phase0->setV(cell->getPhase()->getV());
        phase0->setTotalEnergy(cell->getPhase()->getTotalEnergy());
        phase0->computePressurefromOther();

        cellphase->setDensity(coef[stage][1] * phase0->getDensity() + coef[stage][2] * m_dt * rhs->getMass());
        cellphase->setU(coef[stage][1] * phase0->getU() + coef[stage][2] * m_dt * rhs->getMomentum()[0]);
        cellphase->setV(coef[stage][1] * phase0->getV() + coef[stage][2] * m_dt * rhs->getMomentum()[1]);
        cellphase->setTotalEnergy(coef[stage][1] * phase0->getTotalEnergy() + coef[stage][2] * m_dt * rhs->getEnergy());
        cellphase->computePressurefromOther();
    }else if(stage == 1){
        phase1->setDensity(cellphase->getDensity());
        phase1->setU(cellphase->getU());
        phase1->setV(cellphase->getV());
        phase1->setTotalEnergy(cellphase->getTotalEnergy());
        phase1->computePressurefromOther();

        cellphase->setDensity(coef[stage][0] * phase0->getDensity() + coef[stage][1] * phase1->getDensity() + coef[stage][2] * m_dt * rhs->getMass());
        cellphase->setU(coef[stage][0] * phase0->getU() + coef[stage][1] * phase1->getU() + coef[stage][2] * m_dt * rhs->getMomentum()[0]);
        cellphase->setV(coef[stage][0] * phase0->getV() + coef[stage][1] * phase1->getV() + coef[stage][2] * m_dt * rhs->getMomentum()[1]);
        cellphase->setTotalEnergy(coef[stage][0] * phase0->getTotalEnergy() + coef[stage][1] * phase1->getTotalEnergy() + coef[stage][2] * m_dt * rhs->getEnergy());
        cellphase->computePressurefromOther();
    }else{
        cellphase->setDensity(coef[stage][0] * phase0->getDensity() + coef[stage][1] * phase1->getDensity() + coef[stage][2] * m_dt * rhs->getMass());
        cellphase->setU(coef[stage][0] * phase0->getU() + coef[stage][1] * phase1->getU() + coef[stage][2] * m_dt * rhs->getMomentum()[0]);
        cellphase->setV(coef[stage][0] * phase0->getV() + coef[stage][1] * phase1->getV() + coef[stage][2] * m_dt * rhs->getMomentum()[1]);
        cellphase->setTotalEnergy(coef[stage][0] * phase0->getTotalEnergy() + coef[stage][1] * phase1->getTotalEnergy() + coef[stage][2] * m_dt * rhs->getEnergy());
        cellphase->computePressurefromOther();
    }
}