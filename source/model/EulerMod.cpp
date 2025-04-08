#include "EulerMod.h"

void EulerMod::allocatePhase(std::unique_ptr<Phase> &phase)
{
    phase = std::make_unique<EulerPhase>();
}

void EulerMod::allocateFlux(std::unique_ptr<Flux> &flux)
{
    flux = std::make_unique<EulerFlux>();
}

void EulerMod::allocatePhase(std::shared_ptr<Phase> &phase)
{
    phase = std::make_shared<EulerPhase>();
}

void EulerMod::allocateFlux(std::shared_ptr<Flux> &flux)
{
    flux = std::make_shared<EulerFlux>();
}

void EulerMod::allocateGradient(std::unique_ptr<Gradient> &gradient)
{
    gradient = std::make_unique<EulerGradient>();
}

void EulerMod::allocateGradient(std::shared_ptr<Gradient> &gradient)
{
    gradient = std::make_shared<EulerGradient>();
}

std::shared_ptr<Flux> EulerMod::comFlux(const std::unique_ptr<Phase>& phase, Direction dir)
{   
    phase->computePressurefromOther();
    auto flux = std::make_shared<EulerFlux>();
    const double rho = phase->getDensity();
    const double u = phase->getU();
    const double v = phase->getV();
    const double p = phase->getPressure();
    const double E = phase->getTotalEnergy();

    if (dir == Direction::X) {
        flux->setMass(u);
        flux->setMomentum({u*u/rho + p, u*v/rho});
        flux->setEnergy((E + p) * (u/rho));
    } else { // Direction::Y
        flux->setMass(v);
        flux->setMomentum({u*v/rho, v*v/rho + p});
        flux->setEnergy((E + p) * (v/rho));
    }

    return flux;
}


const int &EulerMod::getVarnumber()
{
    return numberVar;
}
