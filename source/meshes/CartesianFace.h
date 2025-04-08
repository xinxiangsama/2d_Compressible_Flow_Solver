#pragma once

#include "Face.h"
class CartesianFace : public Face
{
public:
    CartesianFace() = default;
    virtual ~CartesianFace() override = default;

    //Modify
    virtual void setArea(const double& Area) override;
    virtual void setPostion(const double& x, const double& y) override;
    virtual void setTangent(const double& x, const double& y) override;
    virtual void setNormal(const double& x, const double& y) override;
    virtual void setIndex(const int& i, const int& j);
};