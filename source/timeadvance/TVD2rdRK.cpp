#include "TVD2rdRK.h"

void TVD2rdRK::advance(Cell *cell, const int &step)
{   
    auto& phase = m_Phase[cell];
    auto& cellphase = cell->getPhase();
    auto rhs {std::move(cell->comRHS())};
    if(!phase){
        model->allocatePhase(phase);
    }
    int stage = (step % 2 == 0) ? 1 : 0;
    if(stage == 0){
        phase->setDensity(cell->getPhase()->getDensity());
        phase->setU(cell->getPhase()->getU());
        phase->setV(cell->getPhase()->getV());
        phase->setTotalEnergy(cell->getPhase()->getTotalEnergy());
        phase->computePressurefromOther();

        cellphase->setDensity(coef[stage][1] * phase->getDensity() + coef[stage][2] * m_dt * rhs->getMass());
        cellphase->setU(coef[stage][1] * phase->getU() + coef[stage][2] * m_dt * rhs->getMomentum()[0]);
        cellphase->setV(coef[stage][1] * phase->getV() + coef[stage][2] * m_dt * rhs->getMomentum()[1]);
        cellphase->setTotalEnergy(coef[stage][1] * phase->getTotalEnergy() + coef[stage][2] * m_dt * rhs->getEnergy());
        cellphase->computePressurefromOther();
    }else{
        cellphase->setDensity(coef[stage][0] * phase->getDensity() + coef[stage][1] * cellphase->getDensity() + coef[stage][2] * m_dt * rhs->getMass());
        cellphase->setU(coef[stage][0] * phase->getU() + coef[stage][1] * cellphase->getU() + coef[stage][2] * m_dt * rhs->getMomentum()[0]);
        cellphase->setV(coef[stage][0] * phase->getV() + coef[stage][1] * cellphase->getV() + coef[stage][2] * m_dt * rhs->getMomentum()[1]);
        cellphase->setTotalEnergy(coef[stage][0] * phase->getTotalEnergy() + coef[stage][1] * cellphase->getTotalEnergy() + coef[stage][2] * m_dt * rhs->getEnergy());
        cellphase->computePressurefromOther();
    }
}

void TVD2rdRK::setTimeStep(const double &dt)
{
    m_dt = dt;
}

const double &TVD2rdRK::getTimeStep() const
{
    return m_dt;
}
