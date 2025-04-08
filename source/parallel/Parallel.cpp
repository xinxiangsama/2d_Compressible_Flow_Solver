#include "Parallel.h"

void Parallel::setMesh(std::shared_ptr<Mesh> &mesh)
{
    m_mesh = mesh.get();
    // std::cout <<mesh->getnumberCellsXGlobal() <<std::endl;
}

void Parallel::setIdAndNumprocs(const int &_myid, const int &_numprocs)
{
    myid = _myid;
    numprocs = _numprocs;
}
