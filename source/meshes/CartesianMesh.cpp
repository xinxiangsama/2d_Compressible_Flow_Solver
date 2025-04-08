#include "CartesianMesh.h"

void CartesianMesh::allocateCells(std::vector<std::vector<Cell>>& cells)
{
    cells.resize(m_numberCellsX);
    for(auto& colcell : cells){
        colcell.resize(m_numberCellsY);
    }
}

void CartesianMesh::allocateCellInterfaces(std::vector<CellInterface>& cellInterfaces)
{   
    size_t numberCellInterfaces = (m_numberCellsX + 1) * m_numberCellsY + (m_numberCellsY + 1) * m_numberCellsX;
    cellInterfaces.resize(numberCellInterfaces);
}

void CartesianMesh::BindCellInterfacesWithCells(std::vector<std::vector<Cell>> &cell, std::vector<CellInterface> &cellInterfaces)
{   

    for(auto& col : cell){
        for(auto& cell1 : col){
            cell1.getCellInterfaces().resize(4);
        }
    }

    // horizontol face
    for(int i = 0 ; i < m_numberCellsX + 1; ++i){
        for(int j = 0; j < m_numberCellsY; ++j){
            int cellLeftIndex[2] {i - 1, j};
            int cellRightIndex[2] {i, j};
            int cellInterfaceIndex {i + j * (m_numberCellsX + 1)};
            
            if(i - 1 < 0){
                (cellInterfaces)[cellInterfaceIndex].initialize(nullptr, &(cell)[cellRightIndex[0]][cellRightIndex[1]]);
                (cell)[cellRightIndex[0]][cellRightIndex[1]].setCellInterface(0, &(cellInterfaces)[cellInterfaceIndex]);
            }else if(i > m_numberCellsX - 1){
                (cellInterfaces)[cellInterfaceIndex].initialize(&(cell)[cellLeftIndex[0]][cellLeftIndex[1]], nullptr);  
                (cell)[cellLeftIndex[0]][cellLeftIndex[1]].setCellInterface(1, &(cellInterfaces)[cellInterfaceIndex]); 
            }else{
                (cellInterfaces)[cellInterfaceIndex].initialize (&(cell)[cellLeftIndex[0]][cellLeftIndex[1]], &(cell)[cellRightIndex[0]][cellRightIndex[1]]);
                (cell)[cellLeftIndex[0]][cellLeftIndex[1]].setCellInterface(1, &(cellInterfaces)[cellInterfaceIndex]);
                (cell)[cellRightIndex[0]][cellRightIndex[1]].setCellInterface(0, &(cellInterfaces)[cellInterfaceIndex]);
            }

        }
    }

    size_t numberHorizontalface = (m_numberCellsX + 1) * m_numberCellsY;
    // verticle face
    for(int i = 0; i < m_numberCellsX; ++i){
        for(int j = 0; j < m_numberCellsY + 1; ++j){
            int cellLeftIndex[2] {i, j - 1};
            int cellRightIndex[2] {i, j}; 
            int cellInterfaceIndex {numberHorizontalface + i + j * (m_numberCellsX)};

            if(j - 1 < 0){
                (cellInterfaces)[cellInterfaceIndex].initialize(nullptr, &(cell)[cellRightIndex[0]][cellRightIndex[1]]);
                (cell)[cellRightIndex[0]][cellRightIndex[1]].setCellInterface(2, &(cellInterfaces)[cellInterfaceIndex]);
            } else if (j > m_numberCellsY - 1) {
                    (cellInterfaces)[cellInterfaceIndex].initialize(&(cell)[cellLeftIndex[0]][cellLeftIndex[1]], nullptr);
                    (cell)[cellLeftIndex[0]][cellLeftIndex[1]].setCellInterface(3, &(cellInterfaces)[cellInterfaceIndex]);
            } else {
                    (cellInterfaces)[cellInterfaceIndex].initialize(&(cell)[cellLeftIndex[0]][cellLeftIndex[1]], &(cell)[cellRightIndex[0]][cellRightIndex[1]]);
                    (cell)[cellLeftIndex[0]][cellLeftIndex[1]].setCellInterface(3, &(cellInterfaces)[cellInterfaceIndex]);
                    (cell)[cellRightIndex[0]][cellRightIndex[1]].setCellInterface(2, &(cellInterfaces)[cellInterfaceIndex]);
            }
        }
    }

    /*Set Cell's interfaces*/
    // for(auto& cellinterface : (cellInterfaces)){
    //     if(cellinterface.getCellLeft() != nullptr){
    //         cellinterface.getCellLeft()->addCellInterface(&cellinterface);
    //     }

    //     if(cellinterface.getCellRight() != nullptr){
    //         cellinterface.getCellRight()->addCellInterface(&cellinterface);
    //     }
    // }
}

void CartesianMesh::SetElement(std::vector<std::vector<Cell>>& cells)
{   
    auto start_col {m_CpuCoordX * m_numberCellsX};
    auto start_row {m_CpuCoordY * m_numberCellsY};
    m_offsetX = start_col;
    m_offsetY = start_row;
    for(int i = 0; i < m_numberCellsX; ++i){
        for(int j = 0; j < m_numberCellsY; ++j){
            cells[i][j].getElement() = std::make_unique<CartesianElement>();
            cells[i][j].getElement()->SetIndex(Coord{i, j});
            cells[i][j].getElement()->SetPosition(Coord{{(i + 0.5 + start_col) * m_UnidX, (j + 0.5 + start_row) * m_UnidY}});
            cells[i][j].getElement()->Setdx(m_UnidX);
            cells[i][j].getElement()->Setdy(m_UnidY);
        }
    }
}

void CartesianMesh::SetFace(std::vector<CellInterface> &cellinterfaces)
{   
    auto start_col {m_CpuCoordX * m_numberCellsX};
    auto start_row {m_CpuCoordY * m_numberCellsY};
    // horizontol face
    for(int i = 0 ; i < m_numberCellsX + 1; ++i){
        for(int j = 0; j < m_numberCellsY; ++j){
            int cellInterfaceIndex {i + j * (m_numberCellsX + 1)};
            cellinterfaces[cellInterfaceIndex].SetHorizontal();
            cellinterfaces[cellInterfaceIndex].setLeftFace(std::make_unique<CartesianFace>());
            cellinterfaces[cellInterfaceIndex].setRightFace(std::make_unique<CartesianFace>());
            cellinterfaces[cellInterfaceIndex].getLeftFace()->setPostion((i + start_col) * m_UnidX, (j + start_row + 0.5) * m_UnidY);
            cellinterfaces[cellInterfaceIndex].getRightFace()->setPostion((i + start_col) * m_UnidX, (j + start_row + 0.5) * m_UnidY);
            cellinterfaces[cellInterfaceIndex].getLeftFace()->setNormal(-1, 0);
            cellinterfaces[cellInterfaceIndex].getRightFace()->setNormal(1, 0);
            cellinterfaces[cellInterfaceIndex].getLeftFace()->setIndex(i, j);
            cellinterfaces[cellInterfaceIndex].getRightFace()->setIndex(i, j);
            /*set physical boundary*/
            if(cellinterfaces[cellInterfaceIndex].getLeftFace()->getPosition()[0] == 0.0 || cellinterfaces[cellInterfaceIndex].getRightFace()->getPosition()[0] == m_gX){
                cellinterfaces[cellInterfaceIndex].SetBoundary();
            }else if(i == 0 || i == m_numberCellsX){
            /*set process boundary*/
                cellinterfaces[cellInterfaceIndex].SetProcessBoundary();
                // std::cout <<" !!!!"<<std::endl;
            }
        }
    }

    size_t numberHorizontalface = (m_numberCellsX + 1) * m_numberCellsY;
    // verticle face
    for(int i = 0; i < m_numberCellsX; ++i){
        for(int j = 0; j < m_numberCellsY + 1; ++j){
            int cellInterfaceIndex {numberHorizontalface + i + j * (m_numberCellsX)};
            cellinterfaces[cellInterfaceIndex].setLeftFace(std::make_unique<CartesianFace>());
            cellinterfaces[cellInterfaceIndex].setRightFace(std::make_unique<CartesianFace>());
            cellinterfaces[cellInterfaceIndex].getLeftFace()->setPostion((i + start_col + 0.5) * m_UnidX, (j + start_row) * m_UnidY);
            cellinterfaces[cellInterfaceIndex].getRightFace()->setPostion((i + start_col + 0.5) * m_UnidX, (j + start_row) * m_UnidY);
            cellinterfaces[cellInterfaceIndex].getLeftFace()->setNormal(0, -1);
            cellinterfaces[cellInterfaceIndex].getRightFace()->setNormal(0, 1);
            cellinterfaces[cellInterfaceIndex].getLeftFace()->setIndex(i, j);
            cellinterfaces[cellInterfaceIndex].getRightFace()->setIndex(i, j);
            if(cellinterfaces[cellInterfaceIndex].getLeftFace()->getPosition()[1] == 0.0 || cellinterfaces[cellInterfaceIndex].getRightFace()->getPosition()[1] == m_gY){
                cellinterfaces[cellInterfaceIndex].SetBoundary();
            }else if(j == 0 || j == m_numberCellsY){
                cellinterfaces[cellInterfaceIndex].SetProcessBoundary();
                // std::cout <<" it truly has processboundary"<<std::endl;
            }
        }
    }
}

void CartesianMesh::CalCellsize()
{
    m_UnidX = m_lX / m_numberCellsX;
    m_UnidY = m_lY / m_numberCellsY;
}

void CartesianMesh::InsertBoundaryGhostCells(std::vector<CellInterface> &cellInterfaces, std::vector<std::shared_ptr<Cell>>& ghostcells, std::vector<std::shared_ptr<CellInterface>>& ghostcellinterface)
{   
    int countBoundary0 = 0;
    int countBoundary1 = 0;
    int countBoundary2 = 0;
    int countBoundary3 = 0;

    for(auto& cellinterface : cellInterfaces){
        if(cellinterface.getBoundary()){
            if((cellinterface.getLeftFace()->getPosition()[0] == 0) && cellinterface.getHorizontal()){
                m_boundarys[0]->SetGhostcells(cellinterface, ghostcells, ghostcellinterface);
                countBoundary0++;
            }else if(cellinterface.getRightFace()->getPosition()[0] == m_gX){
                m_boundarys[1]->SetGhostcells(cellinterface, ghostcells, ghostcellinterface);
                countBoundary1++;
            }else if((cellinterface.getLeftFace()->getPosition()[1] == 0) && (!cellinterface.getHorizontal())){
                m_boundarys[2]->SetGhostcells(cellinterface, ghostcells, ghostcellinterface);
                countBoundary2++;
            }else if((cellinterface.getRightFace()->getPosition()[1] == m_gY) && (!cellinterface.getHorizontal())){
                m_boundarys[3]->SetGhostcells(cellinterface, ghostcells, ghostcellinterface);
                countBoundary3++;
            }
        }
    }

    // std::cout << "Boundary 0 interfaces: " << countBoundary0 << std::endl;
    // std::cout << "Boundary 1 interfaces: " << countBoundary1 << std::endl;
    // std::cout << "Boundary 2 interfaces: " << countBoundary2 << std::endl;
    // std::cout << "Boundary 3 interfaces: " << countBoundary3 << std::endl;
}

void CartesianMesh::FillSendcellBuffer(std::vector<Cell*>& LeftSendcellBuffer,std::vector<Cell*>& RightSendcellBuffer ,std::vector<Cell*>& BottomSendcellBuffer ,std::vector<Cell*>& TopSendcellBuffer ,std::vector<CellInterface>& cellinterfaces)
{
    // the order is from outer to inner 
    /*eg "left cpu boundary"->"1"->"2"     "-2"<-"-1"<-"right cpu boundary" */
    int Numcell = interpolation->getNeedCellnum() / 2;
    for(auto& cellinterface : cellinterfaces){
        if(cellinterface.getProcessBoundary()){
            if((cellinterface.getHorizontal()) && (cellinterface.getLeftFace()->getIndex()[0] == 0)){
                auto Iterator = &cellinterface;
                for(int i = 0; i < Numcell; ++i){
                    LeftSendcellBuffer.push_back(Iterator->getCellRight());
                    Iterator = Iterator->getCellRight()->getCellInterface(1);
                }
            }else if((cellinterface.getHorizontal()) && (cellinterface.getRightFace()->getIndex()[0] == m_numberCellsX)){
                auto Iterator = &cellinterface;
                for(int i = 0; i < Numcell; ++i){
                    RightSendcellBuffer.push_back(Iterator->getCellLeft());
                    Iterator = Iterator->getCellLeft()->getCellInterface(0);
                }
            }else if((!cellinterface.getHorizontal()) && (cellinterface.getLeftFace()->getIndex()[1] == 0)){
                auto Iterator = &cellinterface;
                for(int i = 0; i < Numcell; ++i){
                    BottomSendcellBuffer.push_back(Iterator->getCellRight());
                    Iterator = Iterator->getCellRight()->getCellInterface(3);
                }
            }else{
                auto Iterator = &cellinterface;
                for(int i = 0; i < Numcell; ++i){
                    TopSendcellBuffer.push_back(Iterator->getCellLeft());
                    Iterator = Iterator->getCellLeft()->getCellInterface(2);
                }
            }
        }
    }
}

void CartesianMesh::WriteRecvcellBuffer(std::vector<Cell *> &LeftRecvcellBuffer, std::vector<Cell *> &RightRecvcellBuffer, std::vector<Cell *> &BottomRecvcellBuffer, std::vector<Cell *> &TopRecvcellBuffer, std::vector<CellInterface> &cellinterfaces)
{   
    // the order need to be reverse
    /*eg  "-2"<-"-1"<-"left cpu boundary"     "right cpu boundary"->"1"->"2" */
    int Numcell = interpolation->getNeedCellnum() / 2;
    for(auto& cellinterface : cellinterfaces){
        if(cellinterface.getProcessBoundary()){
            if((cellinterface.getHorizontal()) && (cellinterface.getLeftFace()->getIndex()[0] == 0)){
                auto Iterator = &cellinterface;
                // for(int i = 0; i < Numcell; ++i){
                //     Iterator = Iterator->getCellLeft()->getCellInterface(0);
                // }
                for(int i = 0; i < Numcell; ++i){
                    // LeftRecvcellBuffer.push_back(Iterator->getCellRight());
                    // Iterator = Iterator->getCellRight()->getCellInterface(0);
                    LeftRecvcellBuffer.push_back(Iterator->getCellLeft());
                    Iterator = Iterator->getCellLeft()->getCellInterface(0);
                }
            }else if((cellinterface.getHorizontal()) && (cellinterface.getRightFace()->getIndex()[0] == m_numberCellsX)){
                auto Iterator = &cellinterface;
                // for(int i = 0; i < Numcell; ++i){
                //     Iterator = Iterator->getCellRight()->getCellInterface(1);
                // }
                for(int i = 0; i < Numcell; ++i){
                    // RightRecvcellBuffer.push_back(Iterator->getCellLeft());
                    // Iterator = Iterator->getCellLeft()->getCellInterface(1);
                    RightRecvcellBuffer.push_back(Iterator->getCellRight());
                    Iterator = Iterator->getCellRight()->getCellInterface(1);
                }

            }else if((!cellinterface.getHorizontal()) && (cellinterface.getLeftFace()->getIndex()[1] == 0)){
                auto Iterator = &cellinterface;
                // for(int i = 0; i < Numcell; ++i){
                //     Iterator = Iterator->getCellLeft()->getCellInterface(2);
                // }
                for(int i = 0; i < Numcell; ++i){
                    // BottomRecvcellBuffer.push_back(Iterator->getCellRight());
                    // Iterator = Iterator->getCellRight()->getCellInterface(2);
                    BottomRecvcellBuffer.push_back(Iterator->getCellLeft());
                    Iterator = Iterator->getCellLeft()->getCellInterface(2);
                }
            }else{
                auto Iterator = &cellinterface;
                // for(int i = 0; i < Numcell; ++i){
                //     Iterator = Iterator->getCellRight()->getCellInterface(3);
                // }
                for(int i = 0 ; i < Numcell; ++i){
                    // TopRecvcellBuffer.push_back(Iterator->getCellLeft());
                    // Iterator = Iterator->getCellLeft()->getCellInterface(3);
                    TopRecvcellBuffer.push_back(Iterator->getCellRight());
                    Iterator = Iterator->getCellRight()->getCellInterface(3);
                }
            }
        }
    }  
}

void CartesianMesh::ApplyBoundaryCondition(std::vector<CellInterface> &cellinterfaces)
{
    // enforce boundary phase to unique value
    int countBoundary0 = 0;
    int countBoundary1 = 0;
    int countBoundary2 = 0;
    int countBoundary3 = 0;

    for(auto& cellinterface : cellinterfaces){
        if(cellinterface.getBoundary()){
            if((cellinterface.getLeftFace()->getPosition()[0] == 0) && cellinterface.getHorizontal()){
                m_boundarys[0]->EnforceBoundary(cellinterface);
                countBoundary0++;
            }else if(cellinterface.getRightFace()->getPosition()[0] == m_gX){
                m_boundarys[1]->EnforceBoundary(cellinterface);
                countBoundary1++;
            }else if((cellinterface.getLeftFace()->getPosition()[1] == 0) && (!cellinterface.getHorizontal())){
                m_boundarys[2]->EnforceBoundary(cellinterface);
                countBoundary2++;
            }else if((cellinterface.getRightFace()->getPosition()[1] == m_gY) && (!cellinterface.getHorizontal())){
                m_boundarys[3]->EnforceBoundary(cellinterface);
                countBoundary3++;
            }
        }
    }
}

void CartesianMesh::setnumberCellsX(const int &N)
{
    m_numberCellsX = N;
}

void CartesianMesh::setnumberCellsY(const int &N)
{
    m_numberCellsY = N;
}

void CartesianMesh::setnumberCpuX(const int &N)
{
    m_numberCellsX = N;
}

void CartesianMesh::setnumberCpuY(const int &N)
{
    m_numberCellsY = N;
}

void CartesianMesh::setCpuCoordX(const int &N)
{
    m_CpuCoordX = N;
}

void CartesianMesh::setCpuCoordY(const int &N)
{
    m_CpuCoordY = N;
}

void CartesianMesh::setGlobalLengthX(const double &x)
{
    m_gX = x;
}

void CartesianMesh::setGlobalLengthY(const double &y)
{
    m_gY = y;
}

void CartesianMesh::setLocalLengthX(const double &x)
{
    m_lX = x;
}

void CartesianMesh::setLocalLengthY(const double &y)
{
    m_lY = y;
}

void CartesianMesh::setImmersedBoundary(std::shared_ptr<ImmersedBoundary> &object, std::vector<std::vector<Cell>> &cells)
{
    m_immersedboundary = object;
    m_immersedboundary->Initialize(400);
    for(auto& col : cells){
        for(auto& cell : col){
            m_immersedboundary->DistinguishWallandFluid(&cell);
        }
    }
    m_immersedboundary->SetGhostcell(cells, 3);
    for(auto& col : cells){
        for(auto& cell : col){
            if(cell.isImmersedGhost()){
                m_immersedboundary->SetImagepoint(&cell, cells);
            }
        }
    }
}


const int &CartesianMesh::getnumberCellsXGlobal()
{
    return m_numberCellsXGlobal;
}

const int &CartesianMesh::getnumberCellsYGlobal()
{
    return m_numberCellsYGlobal;
}

const int &CartesianMesh::getnumberCellsX()
{
    return m_numberCellsX;
}

const int &CartesianMesh::getnumberCellsY()
{
    return m_numberCellsY;
}

const double &CartesianMesh::getUnidX()
{
    return m_UnidX;
}

const double &CartesianMesh::getUnidY()
{
    return m_UnidY;
}

std::vector<std::shared_ptr<Boundary>> &CartesianMesh::getBoundarys()
{
    return m_boundarys;
}

const double &CartesianMesh::getGlobalLengthX()
{
    return m_gX;
}

const double &CartesianMesh::getGlobalLengthY()
{
    return m_gY;
}

const double &CartesianMesh::getLocalLengthX()
{
    return m_lX;
}

const double &CartesianMesh::getLocalLengthY()
{
    return m_lY;
}

void CartesianMesh::setnumberCellsXGlobal(const int &N)
{
    m_numberCellsXGlobal = N;
}
void CartesianMesh::setnumberCellsYGlobal(const int &N)
{
    m_numberCellsYGlobal = N;
}
const int &CartesianMesh::getoffsetX()
{
    return m_offsetX;
}

const int &CartesianMesh::getoffsetY()
{
    return m_offsetY;
}