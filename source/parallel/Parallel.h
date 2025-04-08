#pragma once
#include <mpi.h>
#include "../meshes/Mesh.h"

class Parallel
{
public:
    Parallel()= default;
    virtual ~Parallel() = default;

    void setMesh(std::shared_ptr<Mesh>& mesh);
    void setIdAndNumprocs(const int& _myid, const int& _numprocs);
    virtual void ZoneDcomposition() = 0; // subclass must have one!
    virtual void setNeibours() = 0;
    virtual void InsertCpuBoundaryGhostCells(std::vector<CellInterface> &cellInterfaces, std::vector<std::shared_ptr<Cell>>& ghostcells, std::vector<std::shared_ptr<CellInterface>>&) = 0;
    virtual void FillSendcellBuffer(std::vector<CellInterface>& cellinterfaces) {};
    virtual void WriteRecvcellBuffer(std::vector<CellInterface>& cellinterfaces) {};
    virtual void exchangedata() {};
    // Access
    virtual const std::vector<int>& getNeibours() const = 0;
    virtual const int& getLeftNeibour() {};
    virtual const int& getRightNeibour() {};
    virtual const int& getTopNeibour() {};
    virtual const int& getBottomNeibour() {};
    const int& getnumberGhostCell() {return m_numberGhostCell;};

    // Modify
    virtual void setnumberGhostCell(const int&) {};
protected:
    Mesh* m_mesh;
    int myid;
    int numprocs;
    int m_numberGhostCell; // ghost cell num changed for interpolation methold, as for weno5th ,it's 3
};