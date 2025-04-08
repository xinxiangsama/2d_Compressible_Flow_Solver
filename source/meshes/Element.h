#pragma once
#include "Face.h"

class Element
{   
public:
    Element() = default;
    virtual ~Element() = default;
    // Access
    void setCellAssociee(const int& numCell){ m_numCellAssociee = numCell; };
    const Coord& getPosition() const { return m_position; };
    const double& getVolume() const { return m_volume; };
    const int& getNumCellAssociee() const { return m_numCellAssociee; };
    virtual const Coord& getIndex() const = 0;
    virtual const double& getdx() {};
    virtual const double& getdy() {};
    // Modify
    virtual void SetVolume(const double&) = 0;
    virtual void SetPosition(const Coord&) = 0;
    virtual void SetIndex(const Coord&) = 0;
    virtual void Setdx(const double&) {};
    virtual void Setdy(const double&) {};
protected:
    Coord m_position;
    double m_volume;
    int m_numCellAssociee;
};