#include "CartesianFace.h"

void CartesianFace::setArea(const double& Area)
{
    m_surfaceArea = Area;
}

void CartesianFace::setPostion(const double& x, const double& y)
{
    m_position(0) = x;
    m_position(1) = y;
}

void CartesianFace::setTangent(const double &x, const double &y)
{
    m_tangent(0) = x;
    m_tangent(1) = y;
}

void CartesianFace::setNormal(const double &x, const double &y)
{
    m_normal(0) = x;
    m_normal(1) = y;
}

void CartesianFace::setIndex(const int &i, const int &j)
{
    m_Index[0] = i;
    m_Index[1] = j;
}
