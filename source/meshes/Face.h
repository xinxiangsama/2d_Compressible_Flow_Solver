#pragma once

#include <Eigen/Dense>

using Coord = Eigen::Vector2d;
class Face
{
public:
    Face() = default;
    virtual ~Face() = default;

    //Access
    const Coord& getNormal() const {return m_normal;};
    const Coord& getTangent() const {return m_tangent;};
    const double& getArea() const {return m_surfaceArea;};
    const Coord& getPosition() const {return m_position;};
    const Coord& getIndex() const {return m_Index;};

    //Modify
    virtual void setArea(const double& ) = 0;
    virtual void setPostion(const double& , const double&) = 0;
    virtual void setTangent(const double& , const double&) = 0;
    virtual void setNormal(const double& , const double&) = 0;
    virtual void setIndex(const int&, const int&) = 0;

protected:
    Coord m_position;
    double m_surfaceArea;
    Coord m_normal;
    Coord m_tangent;
    Coord m_Index;

};