#pragma once
#include "Element.h"

class CartesianElement : public Element
{
public:
    CartesianElement() = default;
    virtual ~CartesianElement() = default;
    // Access
    virtual const Coord& getIndex() const;
    virtual const double& getdx();
    virtual const double& getdy();
    // Modify
    virtual void SetIndex(const Coord& index);
    virtual void SetVolume(const double& volume);
    virtual void SetPosition(const Coord& position);
    virtual void Setdx(const double& dx);
    virtual void Setdy(const double& dy);
protected:
    Coord m_Index;
    double m_dx;
    double m_dy;
};