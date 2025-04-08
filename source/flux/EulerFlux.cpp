#include "EulerFlux.h"

void EulerFlux::SetToZero()
{
    m_mass = 0.0;
    m_energy = 0.0;
    m_momentum.setZero();
}

void EulerFlux::setMass(const double &mass)
{
    m_mass = mass;
}

void EulerFlux::setMomentum(const Coord &momentum)
{
    m_momentum = momentum;
}

void EulerFlux::setEnergy(const double &energy)
{
    m_energy = energy;
}

const double &EulerFlux::getMass() const
{
    return m_mass;
}

const Flux::Coord &EulerFlux::getMomentum() const
{
    return m_momentum; 
}

const double &EulerFlux::getEnergy() const
{
    return m_energy;
}

Flux &EulerFlux::operator+(const Flux &other)
{
    m_mass += other.getMass();
    m_momentum += other.getMomentum();
    m_energy += other.getEnergy();
    return *this;
}

Flux &EulerFlux::operator-(const Flux &other)
{
    m_mass -= other.getMass();
    m_momentum -= other.getMomentum();
    m_energy -= other.getEnergy();
    return *this;
}

Flux &EulerFlux::operator*(const double &scalar)
{
    m_mass *= scalar;
    m_momentum *= scalar;
    m_energy *= scalar;
    return *this;
}

Flux &EulerFlux::operator/(const double &scalar)
{
    m_mass /= scalar;
    m_momentum /= scalar;
    m_energy /= scalar;
    return *this;
}

void EulerFlux::operator=(const Flux &other)
{
    m_mass = other.getMass();
    m_momentum = other.getMomentum();
    m_energy = other.getEnergy();
}
