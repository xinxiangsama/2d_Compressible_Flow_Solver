#pragma once
#include "Mesh.h"

class CartesianMesh : public Mesh
{
public:
    CartesianMesh()  = default;
    virtual ~CartesianMesh() = default;

    virtual void allocateCells(std::vector<std::vector<Cell>>& cells);
    virtual void allocateCellInterfaces(std::vector<CellInterface>& cellInterfaces);
    virtual void BindCellInterfacesWithCells(std::vector<std::vector<Cell>>& cell,std::vector<CellInterface>& cellInterfaces) override;
    virtual void SetElement(std::vector<std::vector<Cell>>& cells) override;
    virtual void SetFace(std::vector<CellInterface>& cellinterfaces) override;
    virtual void CalCellsize();
    virtual void InsertBoundaryGhostCells(std::vector<CellInterface>& cellInterfaces, std::vector<std::shared_ptr<Cell>>& ghostcells, std::vector<std::shared_ptr<CellInterface>>& ghostcellinterface);
    virtual void FillSendcellBuffer(std::vector<Cell*>& LeftSendcellBuffer,std::vector<Cell*>& RightSendcellBuffer ,std::vector<Cell*>& BottomSendcellBuffer ,std::vector<Cell*>& TopSendcellBuffer ,std::vector<CellInterface>& cellinterfaces);
    virtual void WriteRecvcellBuffer(std::vector<Cell*>& LeftRecvcellBuffer,std::vector<Cell*>& RightRecvcellBuffer ,std::vector<Cell*>& BottomRecvcellBuffer ,std::vector<Cell*>& TopRecvcellBuffer ,std::vector<CellInterface>& cellinterfaces);
    virtual void ApplyBoundaryCondition(std::vector<CellInterface>& cellinterfaces) override;
    // Modify
    virtual void setnumberCellsX(const int& N);
    virtual void setnumberCellsY(const int& N);
    virtual void setnumberCellsXGlobal(const int& N);
    virtual void setnumberCellsYGlobal(const int& N);
    virtual void setnumberCpuX(const int& N);
    virtual void setnumberCpuY(const int& N);
    virtual void setCpuCoordX(const int& N);
    virtual void setCpuCoordY(const int& N);
    virtual void setGlobalLengthX(const double& x);
    virtual void setGlobalLengthY(const double& y);
    virtual void setLocalLengthX(const double& x);
    virtual void setLocalLengthY(const double& y);
    virtual void setImmersedBoundary(std::shared_ptr<ImmersedBoundary>& object, std::vector<std::vector<Cell>>& cells);

    // Access
    virtual const int& getnumberCellsXGlobal();
    virtual const int& getnumberCellsYGlobal();
    virtual const int& getnumberCellsX();
    virtual const int& getnumberCellsY();
    virtual const double& getUnidX();
    virtual const double& getUnidY();
    virtual const int& getoffsetX();
    virtual const int& getoffsetY();
    virtual std::vector<std::shared_ptr<Boundary>>& getBoundarys();
    virtual const double& getGlobalLengthX();
    virtual const double& getGlobalLengthY();
    virtual const double& getLocalLengthX();
    virtual const double& getLocalLengthY();

protected:
    double m_lX {};
    double m_lY {};// local length of one process compute domain
    double m_gX {};
    double m_gY{}; // global length of compute domain
    int m_numberCellsX;
    int m_numberCellsY;
    int m_numberCellsXGlobal;
    int m_numberCellsYGlobal;
    std::vector<double> m_dXi;/*!< Array of the lengths of the cells in the x-direction */
    std::vector<double> m_dYj;/*!< Array of the lengths of the cells in the y-direction */
    std::vector<double> m_posXi;/*!< Array of the positions of the cells in the x-direction */
    std::vector<double> m_posYj;/*!< Array of the positions of the cells in the y-direction */
    double m_UnidX;
    double m_UnidY;

    /*parallel*/
    int m_numberCpuX;/*!< Optimal number of processors in the x-direction */
    int m_numberCpuY;/*!< Optimal number of processors in the y-direction */
    int m_CpuCoordX;/*!< X-coordinate of the current CPU */
    int m_CpuCoordY;/*!< Y-coordinate of the current CPU */
    int m_offsetX;/*!< Offset in the x-direction of the current CPU for the array of cell lenghts and cell positions */
    int m_offsetY;/*!< Offset in the y-direction of the current CPU for the array of cell lenghts and cell positions */

    /*immersed boundary*/
    std::shared_ptr<ImmersedBoundary> m_immersedboundary;
    // std::vector<std::shared_ptr<ImmersedBoundary>> m_immersedboundaries;

};
extern std::shared_ptr<Interpolation> interpolation;