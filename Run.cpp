#include "Run.h"
#include "meshes/CartesianMesh.h"
#include <chrono>
#include <iomanip>
std::shared_ptr<Model> model;
std::shared_ptr<Interpolation> interpolation;
std::shared_ptr<RiemmanSolver> riemmansolver;
std::shared_ptr<ForthOrderCS> gradient;

void Run::initialize()
{   
    /*Mesh Part*/
    m_mesh = std::make_shared<CartesianMesh>();
    int N {2000};
    // m_mesh->setnumberCellsX(N);
    // m_mesh->setnumberCellsY(N);
    m_mesh->setnumberCellsXGlobal(N);
    m_mesh->setnumberCellsYGlobal(N);
    m_mesh->setGlobalLengthX(2.0);
    m_mesh->setGlobalLengthY(2.0);

    /*Parallel Part*/
    m_parallel = std::make_unique<CartesianParallel>();
    m_parallel->setIdAndNumprocs(myid, numprocs);
    m_parallel->setMesh(m_mesh);
    m_parallel->ZoneDcomposition();
    m_parallel->setNeibours();
    // std::cout << m_mesh->getnumberCellsX()<<std::endl;

    m_mesh->allocateCells(m_cellsLv1);
    // std::cout <<"size1 is"<<m_cellsLv1.size()<<std::endl;
    // std::cout <<"size2 is"<<m_cellsLv1[0].size()<<std::endl;
    m_mesh->allocateCellInterfaces(m_cellinterfaces);
    m_mesh->CalCellsize();
    /*Bind CellInterfaces with Cells*/
    m_mesh->BindCellInterfacesWithCells(m_cellsLv1, m_cellinterfaces); // wrong!
    m_mesh->SetElement(m_cellsLv1);
    m_mesh->SetFace(m_cellinterfaces);

    /*Model part*/
    m_model = std::make_shared<EulerMod>();
    m_interpolation = std::make_shared<Weno5th>();
    m_gradient = std::make_shared<ForthOrderCS>();
    // m_initialcondition = std::make_shared<Reimman2D>();
    m_initialcondition = std::make_shared<Cylinder>();
    m_riemmansolver = std::make_shared<RiemmanSolverHLL>();
    m_timeadvance = std::make_shared<TVD2rdRK>();
    m_immersedboundary = std::make_shared<Circle>(0.1, Eigen::Vector2d(0.5, 1.0));
    model = m_model;
    interpolation = m_interpolation;
    gradient = m_gradient;
    riemmansolver = m_riemmansolver;
    m_mesh->setImmersedBoundary(m_immersedboundary, m_cellsLv1);
    
    /*allocate Vars*/
    for(auto& col : m_cellsLv1){
        for(auto& cell : col){
            cell.allocateVar();
        }
    }

    for(auto& cellinterface : m_cellinterfaces){
        cellinterface.allocateVar();
    }

    /*Boundary part*/
    m_initialcondition->setBounbdaryCondition(m_mesh->getBoundarys());
    m_mesh->InsertBoundaryGhostCells(m_cellinterfaces, m_GhostcellsLv1, m_Ghostcellinterfaces);
    for(auto& cell : m_GhostcellsLv1){
        cell->allocateVar();
    }

    /*Guard part*/
    m_parallel->InsertCpuBoundaryGhostCells(m_cellinterfaces, m_GhostcellsLv1ForCpuBoundary, m_Ghostcellinterfaces);
    for(auto& cell : m_GhostcellsLv1ForCpuBoundary){
        cell->allocateVar();
    }
        std::cout << "Process " << myid << ": "
                  << "my ghost cell for physics boundary num " << m_GhostcellsLv1.size()
                  << ", my ghost cell for CPU boundary num " << m_GhostcellsLv1ForCpuBoundary.size()
                  << std::endl;

    for(auto& cellinterface : m_cellinterfaces){
        cellinterface.DistinguishType();
    }


    /*IO part*/
    m_output = std::make_unique<Output>(this);
}


void Run::solver()
{   
    m_timeadvance->setTimeStep(dt);
    using clock = std::chrono::high_resolution_clock;

    /* Initial Condition */
    for(auto& col : m_cellsLv1){
        for(auto& cell : col){
            if(cell.isFluid()){
                m_initialcondition->fillIn(&cell);
            }
        }
    }
    for(auto& col : m_cellsLv1){
        for(auto& cell : col){
            if(cell.isImmersedGhost()){
                cell.comImmersedGhostPhase();
            }
        }
    }

    m_parallel->FillSendcellBuffer(m_cellinterfaces);
    m_parallel->WriteRecvcellBuffer(m_cellinterfaces);

    MPI_Barrier(MPI_COMM_WORLD);

    /* Collect cells only once */
    for(auto& cellinterface : m_cellinterfaces){
        // if((cellinterface.getCellLeft()->isFluid()) || (cellinterface.getCellRight()->isFluid())){
            cellinterface.collectCells();
        // }
    }
    for(auto& col : m_cellsLv1){
        for(auto& cell : col){
            cell.collectStencilCells(Direction::X, 5);
            cell.collectStencilCells(Direction::Y, 5);
        }
    }
    m_output->WriteToHDF5("test.h5");
    m_mesh->ApplyBoundaryCondition(m_cellinterfaces);

    /*for test=============================*/
    // m_parallel->exchangedata();
    // for(auto& cellinterface : m_cellinterfaces){
    //     // if((cellinterface.getCellLeft()->isFluid()) || (cellinterface.getCellRight()->isFluid())){
    //     if(!cellinterface.isWallface()){
    //         cellinterface.comInterpolationPhase();
    //         cellinterface.computeFlux();
    //     }

    //     // }
    // }
    // for(auto& col : m_cellsLv1){
    //     for(auto& cell : col){
    //         if(cell.isFluid()){
    //             cell.comRHS();
    //         }
    //     }
    // }
    /*==============================*/
    for(int i = 1; i < 30000; ++i){

        /*com immersed phase*/
        for(auto& col : m_cellsLv1){
            for(auto& cell : col){
                if(cell.isImmersedGhost()){
                    cell.comImmersedGhostPhase();
                }
            }
        }

        auto t_start = clock::now();
        m_parallel->exchangedata();
        auto t_exchange = clock::now();

        for(auto& cellinterface : m_cellinterfaces){
            // if((cellinterface.getCellLeft()->isFluid()) || (cellinterface.getCellRight()->isFluid())){
            // if(!cellinterface.isWallface())
                cellinterface.comInterpolationPhase();
            // }
        }
        auto t_gradient_start = clock::now();
        for(auto& col : m_cellsLv1){
            for(auto& cell : col){
            // cell.comPrimitiveGradient();
            }
        }
        auto t_gradient_end = clock::now();

        auto t_interpolation = clock::now();

        for(auto& cellinterface : m_cellinterfaces){
            // if(cellinterface.getCellLeft()->isFluid()|| cellinterface.getCellRight()->isFluid()){
            // if(!cellinterface.isWallface())
                cellinterface.computeFlux();
            // cellinterface.comviscousflux();
            // }
        }
        auto t_flux = clock::now();

        for(auto& col : m_cellsLv1){
            for(auto& cell : col){
                if(cell.isFluid()){
                    m_timeadvance->advance(&cell, i);
                    // cell.comRHS();
                }
            }
        }
        auto t_evolution = clock::now();

        m_mesh->ApplyBoundaryCondition(m_cellinterfaces);
        auto t_end = clock::now();

        if(i % 50 == 0){
            m_output->WriteToHDF5("test.h5");
        }

        if(myid == 0){
            std::stringstream ss;
            ss << "========================================\n";
            ss << "Time Step: " << std::setw(3) << i << "\n";
            ss << "----------------------------------------\n";
            ss << "Exchange:      " << std::fixed << std::setprecision(3) 
               << std::setw(6) << std::chrono::duration<double, std::milli>(t_exchange - t_start).count() << " ms\n";
            ss << "Interpolation: " << std::setw(6) 
               << std::chrono::duration<double, std::milli>(t_interpolation - t_exchange).count() << " ms\n";
            ss << "Gradient:      " 
                << std::fixed << std::setprecision(3) 
                << std::setw(6) 
                << std::chrono::duration<double, std::milli>(t_gradient_end - t_gradient_start).count() 
                << " ms\n";
            ss << "Flux:          " << std::setw(6) 
               << std::chrono::duration<double, std::milli>(t_flux - t_interpolation).count() << " ms\n";
            ss << "Evolution:     " << std::setw(6) 
               << std::chrono::duration<double, std::milli>(t_evolution - t_flux).count() << " ms\n";
            ss << "Total:         " << std::setw(6) 
               << std::chrono::duration<double, std::milli>(t_end - t_start).count() << " ms\n";
            ss << "========================================\n";
            std::cout << ss.str();
        }
    }

    m_output->WriteToHDF5("test.h5");
}



// void Run::solver()
// {   
//     /*apply initialcondition*/
//     for(auto& col : m_cellsLv1){
//         for(auto& cell : col){
//             m_initialcondition->fillIn(&cell);

//         }
//     }
//     m_parallel->FillSendcellBuffer(m_cellinterfaces);
//     m_parallel->WriteRecvcellBuffer(m_cellinterfaces);

//     MPI_Barrier(MPI_COMM_WORLD);

//     /*apply boundary*/
//     m_mesh->ApplyBoundaryCondition(m_cellinterfaces);

//     /*share data part*/
//     m_parallel->exchangedata();
//     /*Flux Part*/
//     /*(i) collect cells for interface used for interpolation*/
//     /*（ii) use interpolation methold to interpolating. eg Weno5th Teno5th....*/
//     for(auto& cellinterface : m_cellinterfaces){
//         cellinterface.collectCells();
//         cellinterface.comInterpolationPhase();
//         cellinterface.computeFlux();
//     }

//     /*timeAdvance*/
//     for(auto& col : m_cellsLv1){
//         for(auto& cell : col){
//             auto pre = cell.getPhase()->getDensity() ;
//             cell.Evolution();
//             // cell.getFlux()->SetToZero();
//         }
//     }
//     m_output->WriteToHDF5("test.h5");
//     for(int i = 0; i < 500; ++i){
//         if(myid == 0){
//             std::cout << i <<std::endl;
//         }
//         m_mesh->ApplyBoundaryCondition(m_cellinterfaces);
//         m_parallel->exchangedata();
//         for(auto& cellinterface : m_cellinterfaces){
//             cellinterface.comInterpolationPhase();
//             cellinterface.computeFlux();
//         }
    
//         /*timeAdvance*/
//         for(auto& col : m_cellsLv1){
//             for(auto& cell : col){
//                 auto pre = cell.getPhase()->getDensity() ;
//                 cell.Evolution();
//                 // cell.getFlux()->SetToZero();
//             }
//         }
    
//         if(i % 50 == 0){
//             m_output->WriteToHDF5("test.h5");
//         }
//     }
//     m_output->WriteToHDF5("test.h5");

// }