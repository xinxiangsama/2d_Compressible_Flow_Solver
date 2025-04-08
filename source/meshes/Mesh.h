#pragma once
#include <memory>
#include <vector>
#include "Cell.h"
#include "CartesianElement.h"
#include "CartesianFace.h"
#include "../boundary/Boundary.h"
#include "../object/ImmersedBoundary.h"

class Mesh
{
public:
    Mesh() = default;
    virtual ~Mesh() = default;

    virtual void allocateCells(std::vector<std::vector<Cell>>&) {};
    virtual void allocateCellInterfaces(std::vector<CellInterface>& cellInterfaces) {};
    virtual void BindCellInterfacesWithCells(std::vector<std::vector<Cell>>& ,std::vector<CellInterface>&) {};
    virtual void SetElement(std::vector<std::vector<Cell>>&){};
    virtual void SetFace(std::vector<CellInterface>&) {};
    virtual void CalCellsize() {};
    virtual void InsertBoundaryGhostCells(std::vector<CellInterface>&, std::vector<std::shared_ptr<Cell>>&, std::vector<std::shared_ptr<CellInterface>>&) {};
    virtual void SetImmersedBooundary() {};
    virtual void FillSendcellBuffer(std::vector<Cell*>&, std::vector<Cell*>&, std::vector<Cell*>&, std::vector<Cell*>&, std::vector<CellInterface>& cellinterfaces) {};
    virtual void WriteRecvcellBuffer(std::vector<Cell*>& LeftRecvcellBuffer,std::vector<Cell*>& RightRecvcellBuffer ,std::vector<Cell*>& BottomRecvcellBuffer ,std::vector<Cell*>& TopRecvcellBuffer ,std::vector<CellInterface>& cellinterfaces) {};
    virtual void ApplyBoundaryCondition(std::vector<CellInterface>&) {};
    // Modify
    virtual void setnumberCellsX(const int&) {};
    virtual void setnumberCellsY(const int&) {};
    virtual void setnumberCellsXGlobal(const int& N) {};
    virtual void setnumberCellsYGlobal(const int& N) {};
    virtual void setnumberCpuX(const int&) {};
    virtual void setnumberCpuY(const int&) {};
    virtual void setCpuCoordX(const int&) {};
    virtual void setCpuCoordY(const int&) {};
    virtual void setGlobalLengthX(const double&) {};
    virtual void setGlobalLengthY(const double&) {};
    virtual void setLocalLengthX(const double& ) {};
    virtual void setLocalLengthY(const double& ) {};
    virtual void setImmersedBoundary(std::shared_ptr<ImmersedBoundary>&, std::vector<std::vector<Cell>>&) {};

    // Access
    virtual const int& getnumberCellsXGlobal() {};
    virtual const int& getnumberCellsYGlobal() {};
    virtual const int& getnumberCellsX() {};
    virtual const int& getnumberCellsY() {};
    virtual const double& getUnidX() {};
    virtual const double& getUnidY() {};
    virtual const int& getoffsetX() {};
    virtual const int& getoffsetY() {};
    virtual std::vector<std::shared_ptr<Boundary>>& getBoundarys() {};
    virtual const double& getGlobalLengthX() {};
    virtual const double& getGlobalLengthY() {};
    virtual const double& getLocalLengthX() {};
    virtual const double& getLocalLengthY() {};

protected:
    std::vector<std::shared_ptr<Boundary>> m_boundarys;
};