#pragma once
#include "../meshes/Cell.h"
#include "../model/Phase.h"
#include <list>

class Cell;

class Interpolation
{
public:
    Interpolation() = default;
    virtual ~Interpolation() = default;

    virtual void comLeftPhase(std::list<Cell*>&, std::unique_ptr<Phase>&) = 0;
    virtual void comRightPhase(std::list<Cell*>&, std::unique_ptr<Phase>&) = 0;

    // Access
    const int& getNeedCellnum ();

protected:
    int Cellnum {6};
};
