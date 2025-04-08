#pragma once
#include "Parallel.h"

class CartesianParallel : public Parallel
{
public:
    using CartesianNeibourType = std::vector<int>;
    CartesianParallel() = default;
    virtual ~CartesianParallel() = default;

    virtual void ZoneDcomposition() override;
    virtual void setNeibours() override;
    virtual void InsertCpuBoundaryGhostCells(std::vector<CellInterface> &cellInterfaces, std::vector<std::shared_ptr<Cell>>& ghostcells, std::vector<std::shared_ptr<CellInterface>>& ghostcellinterfaces);
    virtual void FillSendcellBuffer(std::vector<CellInterface>& cellinterfaces);
    virtual void WriteRecvcellBuffer(std::vector<CellInterface> &cellinterfaces);
    virtual void exchangedata();
    // Access
    virtual const std::vector<int>& getNeibours() const override;
    virtual const int& getLeftNeibour() override;
    virtual const int& getRightNeibour() override;
    virtual const int& getTopNeibour() override;
    virtual const int& getBottomNeibour() override;
    // Modify
    virtual void setnumberGhostCell(const int& number);

protected:
    CartesianNeibourType m_Neibours ; // left --- right --- top --- bottom
    MPI_Comm comm_cart;
    std::vector<Cell*> LeftSendcellBuffer;
    std::vector<Cell*> RightSendcellBuffer;
    std::vector<Cell*> BottomSendcellBuffer;
    std::vector<Cell*> TopSendcellBuffer;
    std::vector<Cell*> LeftRecvcellBuffer;
    std::vector<Cell*> RightRecvcellBuffer;
    std::vector<Cell*> BottomRecvcellBuffer;
    std::vector<Cell*> TopRecvcellBuffer;
    std::vector<std::unique_ptr<Phase>> SendPhaseBuffer;
    std::vector<std::unique_ptr<Phase>> RecvPhaseBuffer;

};

struct CartesianParallelExchangeCellData
{   
public:
    CartesianParallelExchangeCellData() = default;
    ~CartesianParallelExchangeCellData() = default;
    double density;
    double Mx;
    double My;
    double Energy;
    double ux;
    double uy;
    double vx;
    double vy;
    double Tx;
    double Ty;
};

extern std::shared_ptr<Model> model;