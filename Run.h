#pragma once
#include "model/EulerMod.h"
#include "interpolation/Weno5th.h"
#include "meshes/Mesh.h"
#include "parallel/CartesianParallel.h"
#include "initialcondition/Reimman2D.h"
#include "initialcondition/Cylinder.h"
#include "flux/RiemmanSolverHLL.h"
#include "io/Output.h"
#include "timeadvance/TVD3rdRK.h"
#include "gradient/FourthOrderCS.h"
#include "object/Circle.h"

class Output;
class Run
{
public:
    Run() = default;
    ~Run() = default;

    void initialize();
    void solver();
    void finalize();
    int myid;
    int numprocs;

    std::shared_ptr<Mesh> m_mesh;
    std::vector<std::vector<Cell>> m_cellsLv1;
    std::vector<std::shared_ptr<Cell>> m_GhostcellsLv1; // Ghost cells for physics boundary used for apply boundary condition
    std::vector<std::shared_ptr<Cell>> m_GhostcellsLv1ForCpuBoundary;// Ghost cells for cpu boundary used for recv needed info from other cpus
    std::vector<CellInterface> m_cellinterfaces;
    std::vector<std::shared_ptr<CellInterface>> m_Ghostcellinterfaces;

    /*Parallel module*/
    std::shared_ptr<Parallel> m_parallel;

    /*Model module*/
    std::shared_ptr<Model> m_model;
    std::shared_ptr<Interpolation> m_interpolation;
    std::shared_ptr<ForthOrderCS> m_gradient;
    std::shared_ptr<Initialcondition> m_initialcondition;
    std::shared_ptr<RiemmanSolver> m_riemmansolver; 
    std::shared_ptr<Explicit> m_timeadvance;
    std::shared_ptr<ImmersedBoundary> m_immersedboundary;
    /*IO module*/
    std::unique_ptr<Output> m_output;
    
};