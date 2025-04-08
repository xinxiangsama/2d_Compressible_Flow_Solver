#pragma once
#include "model.h"
#include "EulerPhase.h"
#include "../flux/EulerFlux.h"
#include "../gradient/EulerGradient.h"

class EulerMod : public Model
{
public:
    EulerMod() = default;
    virtual ~EulerMod() = default;

    // allocate
    virtual void allocatePhase(std::unique_ptr<Phase>& phase);
    virtual void allocateFlux(std::unique_ptr<Flux>& flux);
    virtual void allocatePhase(std::shared_ptr<Phase>& phase);
    virtual void allocateFlux(std::shared_ptr<Flux>& flux);

    virtual void allocateGradient(std::unique_ptr<Gradient>&);
    virtual void allocateGradient(std::shared_ptr<Gradient>&);
    // compute
    virtual std::shared_ptr<Flux> comFlux(const std::unique_ptr<Phase>& phase, Direction dir);
    // Access
    virtual const int& getVarnumber();

protected:
    const int numberVar{4};
};