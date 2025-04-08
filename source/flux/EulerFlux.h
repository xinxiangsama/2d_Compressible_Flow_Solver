#pragma once
#include "Flux.h"

class EulerFlux : public Flux
{
public:
    EulerFlux() = default;
    EulerFlux(const double& mass, const Coord& momentum, const double& energy): m_mass(mass), m_momentum(momentum), m_energy(energy) {}
    virtual ~EulerFlux() = default;

    // Modify
    virtual void SetToZero() override;
    virtual void setMass(const double& mass);
    virtual void setMomentum(const Coord& momentum);
    virtual void setEnergy(const double& energy);
    // Access
    virtual const double& getMass() const;
    virtual const Coord& getMomentum() const;
    virtual const double& getEnergy() const;
    // Operator
    virtual Flux& operator+(const Flux& other);
    virtual Flux& operator-(const Flux& other);
    virtual Flux& operator*(const double& scalar);
    virtual Flux& operator/(const double& scalar);
    virtual void operator=(const Flux& other);
protected:
    double m_mass;
    Coord m_momentum;
    double m_energy;
};