#include "EulerPhase.h"
using Coord = Eigen::Vector2d;

const double &EulerPhase::getDensity() const
{
    return m_density;
}

const double &EulerPhase::getPressure() const
{
    return m_pressure;
}

const double &EulerPhase::getU() const
{
    return m_velocity(0);
}

const double &EulerPhase::getV() const
{
    return m_velocity(1);
}

const double &EulerPhase::getW() const
{
    //2d no z dim
}

const Coord &EulerPhase::getVelocity() const
{
    return m_velocity;
}

const double &EulerPhase::getEnergy() const
{
    return m_energy;
}

const double &EulerPhase::getTotalEnergy() const
{
    return m_totalEnergy;
}

const double &EulerPhase::getTemperature() const
{
    return m_temperature;
}

const double EulerPhase::getPrimitiveU() const
{
    return m_velocity[0] / m_density;
}

const double EulerPhase::getPrimitiveV() const
{
    return m_velocity[1] / m_density;
}

const double EulerPhase::getPrimitiveW() const
{
    return 0.0;
}

void EulerPhase::setDensity(const double& density)
{
    m_density = density;
}

void EulerPhase::setPressure(const double & pressure)
{
    m_pressure = pressure;
}

void EulerPhase::setVelocity(const double & u, const double & v, const double & w)
{
    m_velocity(0) = u;
    m_velocity(1) = v;
}

void EulerPhase::setVelocity(const Coord & velocity)
{
    m_velocity = velocity;
}

void EulerPhase::setU(const double & U)
{
    m_velocity(0) = U;
}

void EulerPhase::setV(const double & V)
{
    m_velocity(1) = V;
}

void EulerPhase::setW(const double &)
{
}

void EulerPhase::setEnergy(const double& energy)
{
    m_energy = energy;
}

void EulerPhase::setTotalEnergy(const double& totalenergy)
{  
    m_totalEnergy = totalenergy;
}

void EulerPhase::setTemperature(const double& temperature)
{
    m_temperature = temperature;
}

void EulerPhase::cpyPhase(const Phase &otherPhase)
{
    m_density = otherPhase.getDensity();
    m_pressure = otherPhase.getPressure();
    m_temperature = otherPhase.getTemperature();
    m_velocity = otherPhase.getVelocity();
    m_energy = otherPhase.getEnergy();
    m_totalEnergy = otherPhase.getTotalEnergy();
}

void EulerPhase::computeEnergyfromOther()
{   
    m_energy = m_pressure / (Gamma - 1);
    m_totalEnergy = m_energy + 0.5 * (m_velocity(0) * m_velocity(0) + m_velocity(1) * m_velocity(1)) / m_density;
}

void EulerPhase::computePressurefromOther()
{
    m_pressure = (Gamma - 1) * (m_totalEnergy -  0.5 * (m_velocity(0) * m_velocity(0) + m_velocity(1) * m_velocity(1)) / m_density);
}

void EulerPhase::computeTemperaturefromOther()
{
    m_temperature = Gamma * pow(Ma_ref, 2) * m_pressure / m_density;
}

void EulerPhase::computeViscosityfromOther()
{
    m_viscosity = (1 + C / Tref) * pow(m_temperature, 1.5) / (m_temperature + C / Tref);

}
Phase &EulerPhase::operator+(const Phase &otherPhase)
{
    m_density += otherPhase.getDensity();
    m_pressure += otherPhase.getPressure();
    m_temperature += otherPhase.getTemperature();         
    m_velocity += otherPhase.getVelocity();
    m_energy += otherPhase.getEnergy();
    m_totalEnergy += otherPhase.getTotalEnergy();
    return *this;
}

Phase &EulerPhase::operator-(const Phase &otherPhase)
{
    m_density -= otherPhase.getDensity();
    m_pressure -= otherPhase.getPressure();
    m_temperature -= otherPhase.getTemperature();
    m_velocity -= otherPhase.getVelocity();
    m_energy -= otherPhase.getEnergy();
    m_totalEnergy -= otherPhase.getTotalEnergy();
    return *this;
}

Phase &EulerPhase::operator*(const double &scalar)
{
    m_density *= scalar;
    m_pressure *= scalar;
    m_temperature *= scalar;
    m_velocity *= scalar;
    m_energy *= scalar;
    m_totalEnergy *= scalar;
    return *this;
}

Phase &EulerPhase::operator/(const double &scalar)
{
    m_density /= scalar;
    m_pressure /= scalar;
    m_temperature /= scalar;
    m_velocity /= scalar;
    m_energy /= scalar;
    m_totalEnergy /= scalar;
    return *this;
}