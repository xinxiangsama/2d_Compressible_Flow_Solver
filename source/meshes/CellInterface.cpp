#include "CellInterface.h"
#include <stdexcept>
#include <fstream>
void CellInterface::setLeftFace(std::unique_ptr<Face> &&other)
{
    m_Leftface = std::move(other);
}

void CellInterface::setRightFace(std::unique_ptr<Face> &&other)
{
    m_Rightface = std::move(other);
}
void CellInterface::initialize(Cell *cellLeft, Cell *cellRight)
{
    m_cellLeft = cellLeft;
    m_cellRight = cellRight;
}

void CellInterface::setLeftCell(Cell *cellleft)
{
    m_cellLeft = cellleft;
}

void CellInterface::setRightCell(Cell *cellright)
{
    m_cellRight = cellright;
}

void CellInterface::allocateVar()
{
    if(m_phaseLeft.get() == nullptr){
        model->allocatePhase(m_phaseLeft);
    }

    if(m_phaseRight.get() == nullptr){
        model->allocatePhase(m_phaseRight);
    }

    if(m_flux.get() == nullptr){
        model->allocateFlux(m_flux);
    }

    if(m_visflux.get() == nullptr){
        model->allocateFlux(m_visflux);
    }
}

void CellInterface::comInterpolationPhase()
{
    interpolation->comLeftPhase(m_Cells, m_phaseLeft);
    interpolation->comRightPhase(m_Cells, m_phaseRight);

    // if (std::isnan(m_phaseLeft->getDensity()) || std::isnan(m_phaseRight->getDensity())) {
    //     std::cerr << "Warning: NaN detected in phase density during interpolation!" << std::endl;
    // }
    // if (std::isnan(m_phaseLeft->getU()) || std::isnan(m_phaseRight->getU())) {
    //     std::cerr << "Warning: NaN detected in phase velocity during interpolation!" << std::endl;
    // }
    // if (std::isnan(m_phaseLeft->getV()) || std::isnan(m_phaseRight->getV())) {
    //     std::cerr << "Warning: NaN detected in phase velocity during interpolation!" << std::endl;
    // }
    // if (std::isnan(m_phaseLeft->getEnergy()) || std::isnan(m_phaseRight->getEnergy())) {
    //     std::cerr << "Warning: NaN detected in phase energy during interpolation!" << std::endl;
    // }

}

void CellInterface::computeFlux()
{
    Direction direction = IfHorizontal ? Direction::X : Direction::Y;

    auto leftFlux = model->comFlux(m_phaseLeft, direction);
    auto rightFlux = model->comFlux(m_phaseRight, direction);

    // double coef1 = 1.0, coef2 = 1.0;

    // if (direction == Direction::X) {
    //     if (!m_cellLeft->isCellGhost())
    //         coef1 = m_cellLeft->getElement()->getdx();
    //     if (!m_cellRight->isCellGhost())
    //         coef2 = m_cellRight->getElement()->getdx();
    // } else {
    //     if (!m_cellLeft->isCellGhost())
    //         coef1 = m_cellLeft->getElement()->getdy();
    //     if (!m_cellRight->isCellGhost())
    //         coef2 = m_cellRight->getElement()->getdy();
    // }

    m_flux = riemmansolver->Solver(m_phaseLeft, m_phaseRight, leftFlux, rightFlux, direction);

    // if (std::isnan(m_flux->getMass())) {
    //     std::cerr << "Warning: NaN detected in flux density during computation!" << std::endl;
    // }
    // if (std::isnan(m_flux->getMomentum()[0]) || std::isnan(m_flux->getMomentum()[1])) {
    //     std::cerr << "Warning: NaN detected in flux momentum during computation!" << std::endl;
    // }
    // if (std::isnan(m_flux->getEnergy())) {
    //     std::cerr << "Warning: NaN detected in flux energy during computation!" << std::endl;
    // }

    // if(m_type == InterfaceType::immersed){
    //     std::cout << "Left phase density: " << m_phaseLeft->getDensity() << std::endl;
    //     std::cout << "Right phase density: " << m_phaseRight->getDensity() << std::endl;
    //     std::cout << "Left phase velocity: " << m_phaseLeft->getU() << ", " << m_phaseLeft->getV() << std::endl;
    //     std::cout << "Right phase velocity: " << m_phaseRight->getU() << ", " << m_phaseRight->getV() << std::endl;
    //     std::cout << "Left phase energy: " << m_phaseLeft->getTotalEnergy() << std::endl;
    //     std::cout << "Right phase energy: " << m_phaseRight->getTotalEnergy() << std::endl;
    //     std::cout << "Flux mass: " << m_flux->getMass() << std::endl;
    //     std::cout << "Flux momentum: " << m_flux->getMomentum()[0] << ", " << m_flux->getMomentum()[1] << std::endl;
    //     std::cout << "Flux energy: " << m_flux->getEnergy() << std::endl;
    //     std::cout << "----------------------------------------" << std::endl;
    // }
}

void CellInterface::comviscousflux()
{
    Direction direction = IfHorizontal ? Direction::X : Direction::Y;
    gradient->comviscousfluxForcellinterface(this, m_Cells, direction);
}

void CellInterface::addFlux(const Flux& dflux)
{   
    if(!m_cellRight->isCellGhost()){
        m_cellRight->addFlux(dflux);
    }
}

void CellInterface::subtractFlux(const Flux& dflux)
{   
    if(!m_cellLeft->isCellGhost()){
        m_cellLeft->subtractFlux(dflux);
    }
}

void CellInterface::collectCells()
{      
    int faceOrder {};
    if(IfHorizontal){
        faceOrder = 0;
    }else{
        faceOrder = 2;
    }
    m_Cells.clear();
    int Cellnum = interpolation->getNeedCellnum();
    auto forwardIterator = this; // move forward ,eg 1->2->3
    auto backwardIterator = this;//move backward, eg 2->1->0
    for(int i = 0; i < 3; ++i){
        m_Cells.push_front(backwardIterator->getCellLeft());
        m_Cells.push_back(forwardIterator->getCellRight());

        // if (backwardIterator->getCellLeft()->getPhase()->getDensity() == 0) {
        //     std::cout << "Backward iterator cell phase density is 0" << std::endl;
        //     if(backwardIterator->getProcessBoundary()){
        //         std::cout <<"is cpu boundary"<<std::endl;
        //     }else if(backwardIterator->getBoundary()){
        //         std::cout <<"is phy boundary"<<std::endl;
        //     }else{
        //         std::cout <<"not cpu boundary"<<std::endl;
        //     }
        // }
        // if (forwardIterator->getCellRight()->getPhase()->getDensity() == 0) {
        //     std::cout << "Forward iterator cell phase density is 0" << std::endl;
        //     if(forwardIterator->getProcessBoundary()){
        //         std::cout <<"is cpu boundary"<<std::endl;
        //     }else if(forwardIterator->getBoundary()){
        //         std::cout <<"is phy boundary"<<std::endl;
        //     }else{
        //         std::cout <<"not cpu boundary"<<std::endl;
        //     }
        // }
        // move backward
        if(!backwardIterator->getBoundary()){
            backwardIterator = backwardIterator->getCellLeft()->getCellInterface(faceOrder);
        }
        // move forward
        if(!forwardIterator->getBoundary()){
            forwardIterator = forwardIterator->getCellRight()->getCellInterface(faceOrder + 1);
        }
    }

    if(m_Cells.size() != Cellnum){
        std::cout << "collect cell failed !"<<std::endl;
    }

    int immersedghostcellnum {};
    for(auto& cell : m_Cells){
        if(cell->isImmersedGhost()){
            immersedghostcellnum++;
        }
    }

    // if(m_type == InterfaceType::immersed){
    //     std::cout << "Immersed ghost cell number: " << immersedghostcellnum << std::endl;
    // }
}

void CellInterface::DistinguishType()
{
    if(m_cellLeft->isFluid() && m_cellRight->isFluid()){
        m_type = InterfaceType::fluid;
    }else if (m_cellLeft->isImmersedGhost() && m_cellRight->isFluid() 
              || m_cellLeft->isFluid() && m_cellRight->isImmersedGhost()) {
        m_type = InterfaceType::immersed;
    }else{
        m_type = InterfaceType::solid;
    }
}

Face *CellInterface::getLeftFace() const
{
    return m_Leftface.get();
}

Face *CellInterface::getRightFace() const
{
    return m_Rightface.get();
}

Cell *CellInterface::getCellLeft() const
{
    return m_cellLeft;
}

Cell *CellInterface::getCellRight() const
{
    return m_cellRight;
}

