#include "CartesianParallel.h"

void CartesianParallel::ZoneDcomposition()
{
    int dims[2] {};
    MPI_Dims_create(numprocs, 2, dims);
    int periods[2] = {0, 0}; // No periodicity
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm_cart);
    if(myid == 0){
        std::cout <<"dims[0] " <<dims[0]
                    <<" dims[1] " <<dims[1]<<std::endl;
    }
    m_mesh->setnumberCpuX(dims[0]);
    m_mesh->setnumberCpuY(dims[1]);

    int coords[2];
    MPI_Cart_coords(comm_cart, myid, 2, coords);

    m_mesh->setCpuCoordX(coords[0]);
    m_mesh->setCpuCoordY(coords[1]);

    std::cout << "Process ID: " << myid 
              << ", CPU Coord X: " << coords[0] 
              << ", CPU Coord Y: " << coords[1] 
              << std::endl;

    int N1 {}, N2 {}, N1_local {}, N2_local {};

    N1 = m_mesh->getnumberCellsXGlobal();
    N2 = m_mesh->getnumberCellsYGlobal();
    auto L1 = m_mesh->getGlobalLengthX();
    auto L2 = m_mesh->getGlobalLengthY();

    N1_local = N1 / dims[0];
    N2_local = N2 / dims[1];
    

    // std::cout << m_mesh->getnumberCellsXGlobal()<<std::endl;
    m_mesh->setnumberCellsX(N1_local);
    m_mesh->setnumberCellsY(N2_local);
    m_mesh->setLocalLengthX(L1 / dims[0]);
    m_mesh->setLocalLengthY(L2 / dims[1]);

    std::cout <<"local length of x : "<< m_mesh->getLocalLengthX()<<std::endl;
    std::cout<<"local lenght of y :"<< m_mesh->getLocalLengthY()<<std::endl;
    // std::cout << m_mesh->getnumberCellsX()<<std::endl;
}

void CartesianParallel::setNeibours()
{
    m_Neibours.resize(4);
    MPI_Cart_shift(comm_cart, 0, 1, &m_Neibours[0], &m_Neibours[1]);
    MPI_Cart_shift(comm_cart, 1, 1, &m_Neibours[3], &m_Neibours[2]);

    std::cout << "Process ID: " << myid << std::endl;
    std::cout << "Left Neighbor ID: " << m_Neibours[0] << std::endl;
    std::cout << "Right Neighbor ID: " << m_Neibours[1] << std::endl;
    std::cout << "Bottom Neighbor ID: " << m_Neibours[3] << std::endl;
    std::cout << "Top Neighbor ID: " << m_Neibours[2] << std::endl;
}

void CartesianParallel::InsertCpuBoundaryGhostCells(std::vector<CellInterface> &cellInterfaces, std::vector<std::shared_ptr<Cell>>& ghostcells, std::vector<std::shared_ptr<CellInterface>>& ghostcellinterfaces)
{
    m_numberGhostCell = 3;
    for(auto& cellinterface : cellInterfaces){
        if(cellinterface.getProcessBoundary()){
            // std::cout <<"!!!!!!!!"<<std::endl;
            if(cellinterface.getLeftFace()->getIndex()[0] == 0 && cellinterface.getHorizontal()){
            // insert left ghost cell
                auto Iterator = &cellinterface;
                for(int i = 0; i < m_numberGhostCell; ++i){
                    ghostcells.push_back(std::make_shared<GhostCell>());
                    auto celliter = std::prev(ghostcells.end());
                    (*celliter)->setGhost();
                    Iterator->setLeftCell(celliter->get());
                    /*bind ghostcell & ghostcelliinterfaces*/
                    ghostcellinterfaces.push_back(std::make_shared<CellInterface>());
                    auto cellinterfaceiter = std::prev(ghostcellinterfaces.end());
                    (*cellinterfaceiter)->setRightCell(celliter->get());
                    (*celliter)->getCellInterfaces().resize(4);
                    (*celliter)->setCellInterface(0, cellinterfaceiter->get());
                    // Modify ghost cell position
                    (*celliter)->getElement() = std::make_unique<CartesianElement>();
                    (*celliter)->getElement()->SetPosition(Iterator->getCellRight()->getElement()->getPosition() - Coord(m_mesh->getUnidX(), 0));
                    Iterator = cellinterfaceiter->get();
                }
            }else if(cellinterface.getRightFace()->getIndex()[0] == m_mesh->getnumberCellsX()){
            // insert right ghost cell
                auto Iterator = &cellinterface;
                for(int i = 0; i < m_numberGhostCell; ++i){
                    ghostcells.push_back(std::make_shared<GhostCell>());
                    auto celliter = std::prev(ghostcells.end());
                    (*celliter)->setGhost();
                    Iterator->setRightCell(celliter->get());
                    /*bind ghostcell & ghostcelliinterfaces*/
                    ghostcellinterfaces.push_back(std::make_shared<CellInterface>());
                    auto cellinterfaceiter = std::prev(ghostcellinterfaces.end());
                    (*cellinterfaceiter)->setLeftCell(celliter->get());
                    (*celliter)->getCellInterfaces().resize(4);
                    (*celliter)->setCellInterface(1, cellinterfaceiter->get());
                    // Modify ghost cell position
                    (*celliter)->getElement() = std::make_unique<CartesianElement>();
                    (*celliter)->getElement()->SetPosition(Iterator->getCellLeft()->getElement()->getPosition() + Coord(m_mesh->getUnidX(), 0));
                    Iterator = cellinterfaceiter->get();
                }
            }else if(cellinterface.getLeftFace()->getIndex()[1] == 0 && !cellinterface.getHorizontal()){
                // insert bottom ghost cell
                    auto Iterator = &cellinterface;
                    for(int i = 0; i < m_numberGhostCell; ++i){
                        ghostcells.push_back(std::make_shared<GhostCell>());
                        auto celliter = std::prev(ghostcells.end());
                        (*celliter)->setGhost();
                        Iterator->setLeftCell(celliter->get());
                        /*bind ghostcell & ghostcelliinterfaces*/
                        ghostcellinterfaces.push_back(std::make_shared<CellInterface>());
                        auto cellinterfaceiter = std::prev(ghostcellinterfaces.end());
                        (*cellinterfaceiter)->setRightCell(celliter->get());
                        (*celliter)->getCellInterfaces().resize(4);
                        (*celliter)->setCellInterface(2, cellinterfaceiter->get());
                        // Modify ghost cell position
                        (*celliter)->getElement() = std::make_unique<CartesianElement>();
                        (*celliter)->getElement()->SetPosition(Iterator->getCellRight()->getElement()->getPosition() - Coord(0, m_mesh->getUnidY()));
                        Iterator = cellinterfaceiter->get();
                    }
            }else if(cellinterface.getLeftFace()->getIndex()[1] == m_mesh->getnumberCellsY() && !cellinterface.getHorizontal()){
                // insert top ghost cell
                auto Iterator = &cellinterface;
                for(int i = 0; i < m_numberGhostCell; ++i){
                    ghostcells.push_back(std::make_shared<GhostCell>());
                    auto celliter = std::prev(ghostcells.end());
                    (*celliter)->setGhost();
                    Iterator->setRightCell(celliter->get());
                    /*bind ghostcell & ghostcelliinterfaces*/
                    ghostcellinterfaces.push_back(std::make_shared<CellInterface>());
                    auto cellinterfaceiter = std::prev(ghostcellinterfaces.end());
                    (*cellinterfaceiter)->setLeftCell(celliter->get());
                    (*celliter)->getCellInterfaces().resize(4);
                    (*celliter)->setCellInterface(3, cellinterfaceiter->get());
                    // Modify ghost cell position
                    (*celliter)->getElement() = std::make_unique<CartesianElement>();
                    (*celliter)->getElement()->SetPosition(Iterator->getCellLeft()->getElement()->getPosition() + Coord(0, m_mesh->getUnidY()));
                    Iterator = cellinterfaceiter->get();
                }
            }
        }
    }

}

void CartesianParallel::FillSendcellBuffer(std::vector<CellInterface>& cellinterfaces)
{
    m_mesh->FillSendcellBuffer(LeftSendcellBuffer, RightSendcellBuffer, BottomSendcellBuffer, TopSendcellBuffer,cellinterfaces);
    // std::cout <<"left send cell buffer size is :"<<LeftSendcellBuffer.size()<<std::endl;
    // std::cout << "Process ID: " << myid << std::endl;
    // std::cout << "LeftSendcellBuffer size: " << LeftSendcellBuffer.size() << std::endl;
    // std::cout << "RightSendcellBuffer size: " << RightSendcellBuffer.size() << std::endl;
    // std::cout << "BottomSendcellBuffer size: " << BottomSendcellBuffer.size() << std::endl;
    // std::cout << "TopSendcellBuffer size: " << TopSendcellBuffer.size() << std::endl;
}

void CartesianParallel::WriteRecvcellBuffer(std::vector<CellInterface> &cellinterfaces)
{
    m_mesh->WriteRecvcellBuffer(LeftRecvcellBuffer, RightRecvcellBuffer, BottomRecvcellBuffer, TopRecvcellBuffer, cellinterfaces);
    // std::cout <<"left recv cell buffer size is :"<<LeftRecvcellBuffer.size()<<std::endl;
    // std::cout << "Process ID: " << myid << std::endl;
    // std::cout << "LeftRecvcellBuffer size: " << LeftRecvcellBuffer.size() << std::endl;
    // std::cout << "RightRecvcellBuffer size: " << RightRecvcellBuffer.size() << std::endl;
    // std::cout << "BottomRecvcellBuffer size: " << BottomRecvcellBuffer.size() << std::endl;
    // std::cout << "TopRecvcellBuffer size: " << TopRecvcellBuffer.size() << std::endl;
}

void CartesianParallel::exchangedata()
{
    // define a mpi datatype
    auto Varnum = model->getVarnumber();
    MPI_Datatype MPI_CELL_DATA;
    MPI_Type_contiguous(Varnum + 6, MPI_DOUBLE, &MPI_CELL_DATA);
    MPI_Type_commit(&MPI_CELL_DATA);
    
    // write in double type to send and recv
    std::vector<CartesianParallelExchangeCellData> leftsenddata(LeftSendcellBuffer.size(), CartesianParallelExchangeCellData());
    std::vector<CartesianParallelExchangeCellData> rightsenddata(RightSendcellBuffer.size(), CartesianParallelExchangeCellData());
    std::vector<CartesianParallelExchangeCellData> bottomsenddata(BottomSendcellBuffer.size(), CartesianParallelExchangeCellData());
    std::vector<CartesianParallelExchangeCellData> topsenddata(TopSendcellBuffer.size(), CartesianParallelExchangeCellData());
    for(int i = 0; i <LeftSendcellBuffer.size(); ++i){
        leftsenddata[i].density = LeftSendcellBuffer[i]->getPhase()->getDensity();
        leftsenddata[i].Mx = LeftSendcellBuffer[i]->getPhase()->getU();
        leftsenddata[i].My = LeftSendcellBuffer[i]->getPhase()->getV();
        leftsenddata[i].Energy = LeftSendcellBuffer[i]->getPhase()->getTotalEnergy();
        leftsenddata[i].ux = LeftSendcellBuffer[i]->getGradient()->getUGradient()[0];
        leftsenddata[i].uy = LeftSendcellBuffer[i]->getGradient()->getUGradient()[1];
        leftsenddata[i].vx = LeftSendcellBuffer[i]->getGradient()->getVGradient()[0];
        leftsenddata[i].vy = LeftSendcellBuffer[i]->getGradient()->getVGradient()[1];
        leftsenddata[i].Tx = LeftSendcellBuffer[i]->getGradient()->getTGradient()[0];
        leftsenddata[i].Ty = LeftSendcellBuffer[i]->getGradient()->getTGradient()[1];

        // if(myid == 2){
        //     std::cout <<LeftSendcellBuffer[i]->getPhase()->getU()<<std::endl;
        // }
    }
    for(int i = 0; i < RightSendcellBuffer.size(); ++i){
        rightsenddata[i].density = RightSendcellBuffer[i]->getPhase()->getDensity();
        rightsenddata[i].Mx = RightSendcellBuffer[i]->getPhase()->getU();
        rightsenddata[i].My = RightSendcellBuffer[i]->getPhase()->getV();
        rightsenddata[i].Energy = RightSendcellBuffer[i]->getPhase()->getTotalEnergy();
        rightsenddata[i].ux = RightSendcellBuffer[i]->getGradient()->getUGradient()[0];
        rightsenddata[i].uy = RightSendcellBuffer[i]->getGradient()->getUGradient()[1];
        rightsenddata[i].vx = RightSendcellBuffer[i]->getGradient()->getVGradient()[0];
        rightsenddata[i].vy = RightSendcellBuffer[i]->getGradient()->getVGradient()[1];
        rightsenddata[i].Tx = RightSendcellBuffer[i]->getGradient()->getTGradient()[0];
        rightsenddata[i].Ty = RightSendcellBuffer[i]->getGradient()->getTGradient()[1];

        // if(myid == 1){
        //     std::cout <<RightSendcellBuffer[i]->getPhase()->getTotalEnergy()<<std::endl;
        // }
    }
    for(int i = 0; i < BottomSendcellBuffer.size(); ++i){
        bottomsenddata[i].density = BottomSendcellBuffer[i]->getPhase()->getDensity();
        bottomsenddata[i].Mx = BottomSendcellBuffer[i]->getPhase()->getU();
        bottomsenddata[i].My = BottomSendcellBuffer[i]->getPhase()->getV();
        bottomsenddata[i].Energy = BottomSendcellBuffer[i]->getPhase()->getTotalEnergy();
        bottomsenddata[i].ux = BottomSendcellBuffer[i]->getGradient()->getUGradient()[0];
        bottomsenddata[i].uy = BottomSendcellBuffer[i]->getGradient()->getUGradient()[1];
        bottomsenddata[i].vx = BottomSendcellBuffer[i]->getGradient()->getVGradient()[0];
        bottomsenddata[i].vy = BottomSendcellBuffer[i]->getGradient()->getVGradient()[1];
        bottomsenddata[i].Tx = BottomSendcellBuffer[i]->getGradient()->getTGradient()[0];
        bottomsenddata[i].Ty = BottomSendcellBuffer[i]->getGradient()->getTGradient()[1];
        // if(myid == 2){
        //     std::cout <<BottomSendcellBuffer[i]->getPhase()->getU()<<std::endl;
        // }
    }
    for(int i = 0; i < TopSendcellBuffer.size(); ++i){
        topsenddata[i].density = TopSendcellBuffer[i]->getPhase()->getDensity();
        topsenddata[i].Mx = TopSendcellBuffer[i]->getPhase()->getU();
        topsenddata[i].My = TopSendcellBuffer[i]->getPhase()->getV();
        topsenddata[i].Energy = TopSendcellBuffer[i]->getPhase()->getTotalEnergy();
        topsenddata[i].ux = TopSendcellBuffer[i]->getGradient()->getUGradient()[0];
        topsenddata[i].uy = TopSendcellBuffer[i]->getGradient()->getUGradient()[1];
        topsenddata[i].vx = TopSendcellBuffer[i]->getGradient()->getVGradient()[0];
        topsenddata[i].vy = TopSendcellBuffer[i]->getGradient()->getVGradient()[1];
        topsenddata[i].Tx = TopSendcellBuffer[i]->getGradient()->getTGradient()[0];
        topsenddata[i].Ty = TopSendcellBuffer[i]->getGradient()->getTGradient()[1];
    }

        // Check if any send density is zero
        // for (int i = 0; i < leftsenddata.size(); ++i) {
        //     if (leftsenddata[i].density == 0) {
        //         std::cerr << "Warning: leftsenddata[" << i << "] has density = 0" << std::endl;
        //     }
        // }
        // for (int i = 0; i < rightsenddata.size(); ++i) {
        //     if (rightsenddata[i].density == 0) {
        //         std::cerr << "Warning: rightsenddata[" << i << "] has density = 0" << std::endl;
        //     }
        // }
        // for (int i = 0; i < bottomsenddata.size(); ++i) {
        //     if (bottomsenddata[i].density == 0) {
        //         std::cerr << "Warning: bottomsenddata[" << i << "] has density = 0" << std::endl;
        //     }
        // }
        // for (int i = 0; i < topsenddata.size(); ++i) {
        //     if (topsenddata[i].density == 0) {
        //         std::cerr << "Warning: topsenddata[" << i << "] has density = 0" << std::endl;
        //     }
        // }

        // if (myid == 2) {
        //     for (int i = 0; i < LeftRecvcellBuffer.size(); ++i) {
        //         double density = topsenddata[i].Mx;
        //         // if (density != 0.53226 && density != 0.13799) {
        //             std::cerr << "Warning: RightRecvcellBuffer[" << i << "] on process 0 has unexpected density = " << density << std::endl;
        //         // }
        //     }
        // }


        std::vector<CartesianParallelExchangeCellData> leftrecvdata(LeftRecvcellBuffer.size(), CartesianParallelExchangeCellData());
        std::vector<CartesianParallelExchangeCellData> rightrecvdata(RightRecvcellBuffer.size(), CartesianParallelExchangeCellData());
        std::vector<CartesianParallelExchangeCellData> bottomrecvdata(BottomRecvcellBuffer.size(), CartesianParallelExchangeCellData());
        std::vector<CartesianParallelExchangeCellData> toprecvdata(TopRecvcellBuffer.size(), CartesianParallelExchangeCellData());
        // Exchange data with neighbors
        MPI_Request requests[8];
        MPI_Status statuses[8];

        MPI_Irecv(leftrecvdata.data(), LeftRecvcellBuffer.size(), MPI_CELL_DATA, m_Neibours[0], 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(rightrecvdata.data(), RightRecvcellBuffer.size(), MPI_CELL_DATA, m_Neibours[1], 1, MPI_COMM_WORLD, &requests[1]);
        MPI_Irecv(bottomrecvdata.data(), BottomRecvcellBuffer.size(), MPI_CELL_DATA, m_Neibours[3], 2, MPI_COMM_WORLD, &requests[2]);
        MPI_Irecv(toprecvdata.data(), TopRecvcellBuffer.size(), MPI_CELL_DATA, m_Neibours[2], 3, MPI_COMM_WORLD, &requests[3]);

        MPI_Isend(leftsenddata.data(), LeftSendcellBuffer.size(), MPI_CELL_DATA, m_Neibours[0], 1, MPI_COMM_WORLD, &requests[4]);
        MPI_Isend(rightsenddata.data(), RightSendcellBuffer.size(), MPI_CELL_DATA, m_Neibours[1], 0, MPI_COMM_WORLD, &requests[5]);
        MPI_Isend(bottomsenddata.data(), BottomSendcellBuffer.size(), MPI_CELL_DATA, m_Neibours[3], 3, MPI_COMM_WORLD, &requests[6]);
        MPI_Isend(topsenddata.data(), TopSendcellBuffer.size(), MPI_CELL_DATA, m_Neibours[2], 2, MPI_COMM_WORLD, &requests[7]);

        MPI_Waitall(8, requests, statuses);

        MPI_Type_free(&MPI_CELL_DATA);

        // // Output received data for debugging
        // for (int i = 0; i < LeftRecvcellBuffer.size(); ++i) {
        //     // if (leftrecvdata[i].density != 0 || leftrecvdata[i].Mx != 0 || leftrecvdata[i].My != 0 || leftrecvdata[i].Energy != 0) {
        //         std::cout << "LeftRecvcellBuffer[" << i << "] - Density: " << leftrecvdata[i].density
        //                   << ", Mx: " << leftrecvdata[i].Mx
        //                   << ", My: " << leftrecvdata[i].My
        //                   << ", Energy: " << leftrecvdata[i].Energy << std::endl;
        //     // }
        // }
        // for (int i = 0; i < RightRecvcellBuffer.size(); ++i) {
        //     // if (rightrecvdata[i].density != 0 || rightrecvdata[i].Mx != 0 || rightrecvdata[i].My != 0 || rightrecvdata[i].Energy != 0) {
        //         std::cout << "RightRecvcellBuffer[" << i << "] - Density: " << rightrecvdata[i].density
        //                   << ", Mx: " << rightrecvdata[i].Mx
        //                   << ", My: " << rightrecvdata[i].My
        //                   << ", Energy: " << rightrecvdata[i].Energy << std::endl;
        //     // }
        // }
        // for (int i = 0; i < BottomRecvcellBuffer.size(); ++i) {
        //     // if (bottomrecvdata[i].density != 0 || bottomrecvdata[i].Mx != 0 || bottomrecvdata[i].My != 0 || bottomrecvdata[i].Energy != 0) {
        //         std::cout << "BottomRecvcellBuffer[" << i << "] - Density: " << bottomrecvdata[i].density
        //                   << ", Mx: " << bottomrecvdata[i].Mx
        //                   << ", My: " << bottomrecvdata[i].My
        //                   << ", Energy: " << bottomrecvdata[i].Energy << std::endl;
        //     // }
        // }
        // for (int i = 0; i < TopRecvcellBuffer.size(); ++i) {
        //     // if (toprecvdata[i].density != 0 || toprecvdata[i].Mx != 0 || toprecvdata[i].My != 0 || toprecvdata[i].Energy != 0) {
        //         std::cout << "TopRecvcellBuffer[" << i << "] - Density: " << toprecvdata[i].density
        //                   << ", Mx: " << toprecvdata[i].Mx
        //                   << ", My: " << toprecvdata[i].My
        //                   << ", Energy: " << toprecvdata[i].Energy << std::endl;
        //     // }
        // }

        // if (myid == 3) {
        //     for (int i = 0; i < LeftRecvcellBuffer.size(); ++i) {
        //         double density = leftrecvdata[i].Energy;
        //         // if (density != 0.53226 && density != 0.13799) {
        //             std::cerr << "Warning: RightRecvcellBuffer[" << i << "] on process 0 has unexpected density = " << density << std::endl;
        //         // }
        //     }
        // }

        // Check if any received density is zero
        // for (int i = 0; i < LeftRecvcellBuffer.size(); ++i) {
        //     if (leftrecvdata[i].density == 0) {
        //         std::cerr << "Warning: LeftRecvcellBuffer[" << i << "] has density = 0" << std::endl;
        //     }
        // }
        // for (int i = 0; i < RightRecvcellBuffer.size(); ++i) {
        //     if (rightrecvdata[i].density == 0) {
        //         std::cerr << "Warning: RightRecvcellBuffer[" << i << "] has density = 0" << std::endl;
        //     }
        // }
        // for (int i = 0; i < BottomRecvcellBuffer.size(); ++i) {
        //     if (bottomrecvdata[i].density == 0) {
        //         std::cerr << "Warning: BottomRecvcellBuffer[" << i << "] has density = 0" << std::endl;
        //     }
        // }
        // for (int i = 0; i < TopRecvcellBuffer.size(); ++i) {
        //     if (toprecvdata[i].density == 0) {
        //         std::cerr << "Warning: TopRecvcellBuffer[" << i << "] has density = 0" << std::endl;
        //     }
        // }

        // write data to recvcell
        for(int i = 0; i < LeftRecvcellBuffer.size(); ++i){
            LeftRecvcellBuffer[i]->getPhase()->setDensity(leftrecvdata[i].density);
            LeftRecvcellBuffer[i]->getPhase()->setU(leftrecvdata[i].Mx);
            LeftRecvcellBuffer[i]->getPhase()->setV(leftrecvdata[i].My);
            LeftRecvcellBuffer[i]->getPhase()->setTotalEnergy(leftrecvdata[i].Energy);
            LeftRecvcellBuffer[i]->getPhase()->computePressurefromOther();
            LeftRecvcellBuffer[i]->getGradient()->setUGradient({leftrecvdata[i].ux, leftrecvdata[i].uy});
            LeftRecvcellBuffer[i]->getGradient()->setVGradient({leftrecvdata[i].vx, leftrecvdata[i].vy});
            LeftRecvcellBuffer[i]->getGradient()->setTGradient({leftrecvdata[i].Tx, leftrecvdata[i].Ty});
        }
        for(int i = 0; i < RightRecvcellBuffer.size(); ++i){
            RightRecvcellBuffer[i]->getPhase()->setDensity(rightrecvdata[i].density);
            RightRecvcellBuffer[i]->getPhase()->setU(rightrecvdata[i].Mx);
            RightRecvcellBuffer[i]->getPhase()->setV(rightrecvdata[i].My);
            RightRecvcellBuffer[i]->getPhase()->setTotalEnergy(rightrecvdata[i].Energy);
            RightRecvcellBuffer[i]->getPhase()->computePressurefromOther();
            RightRecvcellBuffer[i]->getGradient()->setUGradient({rightrecvdata[i].ux, rightrecvdata[i].uy});
            RightRecvcellBuffer[i]->getGradient()->setVGradient({rightrecvdata[i].vx, rightrecvdata[i].vy});
            RightRecvcellBuffer[i]->getGradient()->setTGradient({rightrecvdata[i].Tx, rightrecvdata[i].Ty});
        }
        for(int i = 0; i < BottomRecvcellBuffer.size(); ++i){
            BottomRecvcellBuffer[i]->getPhase()->setDensity(bottomrecvdata[i].density);
            BottomRecvcellBuffer[i]->getPhase()->setU(bottomrecvdata[i].Mx);
            BottomRecvcellBuffer[i]->getPhase()->setV(bottomrecvdata[i].My);
            BottomRecvcellBuffer[i]->getPhase()->setTotalEnergy(bottomrecvdata[i].Energy);
            BottomRecvcellBuffer[i]->getPhase()->computePressurefromOther();
            BottomRecvcellBuffer[i]->getGradient()->setUGradient({bottomrecvdata[i].ux, bottomrecvdata[i].uy});
            BottomRecvcellBuffer[i]->getGradient()->setVGradient({bottomrecvdata[i].vx, bottomrecvdata[i].vy});
            BottomRecvcellBuffer[i]->getGradient()->setTGradient({bottomrecvdata[i].Tx, bottomrecvdata[i].Ty});
        }
        for(int i = 0; i < TopRecvcellBuffer.size(); ++i){
            TopRecvcellBuffer[i]->getPhase()->setDensity(toprecvdata[i].density);
            TopRecvcellBuffer[i]->getPhase()->setU(toprecvdata[i].Mx);
            TopRecvcellBuffer[i]->getPhase()->setV(toprecvdata[i].My);
            TopRecvcellBuffer[i]->getPhase()->setTotalEnergy(toprecvdata[i].Energy);
            TopRecvcellBuffer[i]->getPhase()->computePressurefromOther();
            TopRecvcellBuffer[i]->getGradient()->setUGradient({toprecvdata[i].ux, toprecvdata[i].uy});
            TopRecvcellBuffer[i]->getGradient()->setVGradient({toprecvdata[i].vx, toprecvdata[i].vy});
            TopRecvcellBuffer[i]->getGradient()->setTGradient({toprecvdata[i].Tx, toprecvdata[i].Ty});
        }

        // Check if any data in recv buffers is zero after writing
        // for (int i = 0; i < LeftRecvcellBuffer.size(); ++i) {
        //     if (LeftRecvcellBuffer[i]->getPhase()->getDensity() == 0) {
        //         std::cerr << "Warning: LeftRecvcellBuffer[" << i << "] has density = 0 after writing" << std::endl;
        //     }
        // }
        // for (int i = 0; i < RightRecvcellBuffer.size(); ++i) {
        //     if (RightRecvcellBuffer[i]->getPhase()->getDensity() == 0) {
        //         std::cerr << "Warning: RightRecvcellBuffer[" << i << "] has density = 0 after writing" << std::endl;
        //     }
        // }
        // for (int i = 0; i < BottomRecvcellBuffer.size(); ++i) {
        //     if (BottomRecvcellBuffer[i]->getPhase()->getDensity() == 0) {
        //         std::cerr << "Warning: BottomRecvcellBuffer[" << i << "] has density = 0 after writing" << std::endl;
        //     }
        // }
        // for (int i = 0; i < TopRecvcellBuffer.size(); ++i) {
        //     if (TopRecvcellBuffer[i]->getPhase()->getDensity() == 0) {
        //         std::cerr << "Warning: TopRecvcellBuffer[" << i << "] has density = 0 after writing" << std::endl;
        //     }
        // }
}

const std::vector<int> &CartesianParallel::getNeibours() const
{
    return m_Neibours;
}

const int &CartesianParallel::getLeftNeibour()
{
    return m_Neibours[0];
}

const int &CartesianParallel::getRightNeibour()
{
    return m_Neibours[1];
}

const int &CartesianParallel::getTopNeibour()
{
    return m_Neibours[2];
}

const int &CartesianParallel::getBottomNeibour()
{
    return m_Neibours[3];
}

void CartesianParallel::setnumberGhostCell(const int &number)
{
    m_numberGhostCell = number;
}
