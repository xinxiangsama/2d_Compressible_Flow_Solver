#pragma once
#include <Eigen/Dense>
#include <memory>
#include <iostream>
#include "../../param.h"

class Phase;

class Phase
{
public:
    using Coord = Eigen::Vector2d;
public:
    Phase() = default;
    virtual ~Phase() = default;

    // Access
    virtual const double& getDensity() const {};
    virtual const double& getPressure() const {};
    virtual const double& getU() const {}; // u * rho
    virtual const double& getV() const {};
    virtual const double& getW() const {};
    virtual const Coord& getVelocity() const {};
    virtual const double& getEnergy() const {};
    virtual const double& getTotalEnergy() const {};
    virtual const double& getTemperature() const {};
    virtual const double getPrimitiveU() const {};
    virtual const double getPrimitiveV() const {};
    virtual const double getPrimitiveW() const {};

    // Modify
    virtual void setDensity(const double& /*density*/) {};
    virtual void setPressure(const double& /*pressure*/) {};
    virtual void setVelocity(const double& /*u*/, const double& /*v*/, const double& /*w*/) {};
    virtual void setVelocity(const Coord& /*vit*/) {};
    virtual void setU(const double& /*u * mass*/) {};
    virtual void setV(const double& /*v * mass*/) {};
    virtual void setW(const double& /*w*/) {};
    virtual void setEnergy(const double& /*energy*/) {};
    virtual void setTotalEnergy(const double& /*totalEnergy*/) {};
    virtual void setTemperature(const double& /*temperature*/) {};

    // Operator
    virtual void cpyPhase(const Phase& otherPhase) {};
    virtual void computeEnergyfromOther() {};
    virtual void computePressurefromOther() {};
    virtual void computeTemperaturefromOther() {};
    virtual void computeViscosityfromOther() {};
    virtual Phase& operator+ (const Phase& otherPhase) {};
    virtual Phase& operator- (const Phase& otherPhase) {};
    virtual Phase& operator* (const double &scalar) {};
    virtual Phase& operator/ (const double &scalar) {};

    //info
    virtual void info() {};
};