#include "CartesianElement.h"

const Coord &CartesianElement::getIndex() const
{
    return m_Index;
}

const double &CartesianElement::getdx()
{
    return m_dx;
}

const double &CartesianElement::getdy()
{
    return m_dy;
}

void CartesianElement::SetIndex(const Coord &index)
{
    m_Index = index;
}

void CartesianElement::SetVolume(const double &volume)
{
    m_volume = volume;
}

void CartesianElement::SetPosition(const Coord &position)
{
    m_position = position;
}

void CartesianElement::Setdx(const double &dx)
{
    m_dx = dx;
}
void CartesianElement::Setdy(const double &dy)
{
    m_dy = dy;
}
