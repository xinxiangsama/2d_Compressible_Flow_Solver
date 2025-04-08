#pragma once
#include "../flux/Flux.h"
#include "Phase.h"
#include "../param.h"
#include "../gradient/Gradient.h"
class Gradient;
class Model
{
public:
    Model() = default;
    virtual ~Model() = default;

    // allocate
    virtual void allocatePhase(std::unique_ptr<Phase>&) {};
    virtual void allocatePhase(std::shared_ptr<Phase>&) {};
    virtual void allocateFlux(std::unique_ptr<Flux>&) {};
    virtual void allocateFlux(std::shared_ptr<Flux>&) {};
    virtual void allocateGradient(std::unique_ptr<Gradient>&) {};
    virtual void allocateGradient(std::shared_ptr<Gradient>&) {};
    // compute
    virtual std::shared_ptr<Flux> comFlux(const std::unique_ptr<Phase>& phase, Direction dir) {};
    // Access
    virtual const int& getVarnumber() {};
};