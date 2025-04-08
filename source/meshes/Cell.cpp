#include "Cell.h"
#include <fstream>

void Cell::addCellInterface(CellInterface* cellinterface)
{
    m_cellInterfaces.push_back(cellinterface);
}

void Cell::deleteCellInterface(CellInterface *cellinterface)
{
    auto it = std::find_if(m_cellInterfaces.begin(), m_cellInterfaces.end(),
                           [cellinterface](const CellInterface* ptr) {
                               return ptr == cellinterface;
                           });
    if (it != m_cellInterfaces.end()) {
        m_cellInterfaces.erase(it);
    }
}

void Cell::allocateVar()
{   
    if (m_phase.get() == nullptr)
    {
        model->allocatePhase(m_phase);
    }
    
    if(m_flux.get() == nullptr){
        model->allocateFlux(m_flux);
    }

    if(m_gradient.get() == nullptr){
        model->allocateGradient(m_gradient);
    }
}

std::unique_ptr<Element> &Cell::getElement()
{
    return m_element;
}

CellInterface *Cell::getCellInterface(const int &b)
{
    return m_cellInterfaces[b];
}

std::unique_ptr<Phase> &Cell::getPhase()
{
    return m_phase;
}

std::unique_ptr<Flux> &Cell::getFlux()
{
    return m_flux;
}

void Cell::setCellInterface(const int &b, CellInterface *cellinterface)
{
    m_cellInterfaces[b] = cellinterface;
}

void Cell::collectStencilCells(Direction dir, int width)
{
    int radius = width / 2;
    std::vector<Cell*>& stencil = (dir == Direction::X) ? m_stencilX : m_stencilY;
    // stencil.clear();
    stencil.reserve(width);

    for (int offset = -radius; offset <= radius; ++offset) {
        Cell* neighbor = (dir == Direction::X)
                         ? this->getNeighborX(offset)
                         : this->getNeighborY(offset);
        if (!neighbor) {
            std::cerr << "Cell stencil collection failed at offset = " << offset << std::endl;
        }
        stencil.push_back(neighbor);
    }
}

std::vector<Cell *> &Cell::getStencil(Direction dir)
{
    return (dir == Direction::X) ? m_stencilX : m_stencilY;
}

void Cell::Evolution()
{   
    double dt{1e-4};
    auto dx {m_element->getdx()};
    auto dy {m_element->getdy()};
    
    auto mass_rhs = -(m_cellInterfaces[1]->getflux()->getMass() - m_cellInterfaces[0]->getflux()->getMass()) / dx
                      -(m_cellInterfaces[3]->getflux()->getMass() - m_cellInterfaces[2]->getflux()->getMass()) / dy;

    auto momentum_rhs = -(m_cellInterfaces[1]->getflux()->getMomentum() - m_cellInterfaces[0]->getflux()->getMomentum()) / dx
                          -(m_cellInterfaces[3]->getflux()->getMomentum() - m_cellInterfaces[2]->getflux()->getMomentum()) / dy;

    auto energy_rhs = -(m_cellInterfaces[1]->getflux()->getEnergy() - m_cellInterfaces[0]->getflux()->getEnergy()) / dx
                        -(m_cellInterfaces[3]->getflux()->getEnergy() - m_cellInterfaces[2]->getflux()->getEnergy()) / dy;

    m_flux->setMass(mass_rhs);
    m_flux->setMomentum(momentum_rhs);
    m_flux->setEnergy(energy_rhs);

    m_phase->setDensity(m_phase->getDensity() + dt * mass_rhs);
    m_phase->setVelocity(m_phase->getVelocity() + dt * momentum_rhs);
    m_phase->setTotalEnergy(m_phase->getTotalEnergy() + dt * energy_rhs);
    m_phase->computePressurefromOther();

}

std::unique_ptr<Flux> Cell::comRHS()
{
    std::unique_ptr<Flux> rhs;
    model->allocateFlux(rhs);
    auto dx {m_element->getdx()};
    auto dy {m_element->getdy()};
    
    // invicid fluxes
    auto mass_rhs = -(m_cellInterfaces[1]->getflux()->getMass() - m_cellInterfaces[0]->getflux()->getMass()) / dx
                      -(m_cellInterfaces[3]->getflux()->getMass() - m_cellInterfaces[2]->getflux()->getMass()) / dy;

    auto momentum_rhs = -(m_cellInterfaces[1]->getflux()->getMomentum() - m_cellInterfaces[0]->getflux()->getMomentum()) / dx
                          -(m_cellInterfaces[3]->getflux()->getMomentum() - m_cellInterfaces[2]->getflux()->getMomentum()) / dy;

    auto energy_rhs = -(m_cellInterfaces[1]->getflux()->getEnergy() - m_cellInterfaces[0]->getflux()->getEnergy()) / dx
                        -(m_cellInterfaces[3]->getflux()->getEnergy() - m_cellInterfaces[2]->getflux()->getEnergy()) / dy;

    // viscous fluxes
    auto mass_rhs_vis = -(m_cellInterfaces[1]->getvisflux()->getMass() - m_cellInterfaces[0]->getvisflux()->getMass()) / dx
                      -(m_cellInterfaces[3]->getvisflux()->getMass() - m_cellInterfaces[2]->getvisflux()->getMass()) / dy;
    auto momentum_rhs_vis = -(m_cellInterfaces[1]->getvisflux()->getMomentum() - m_cellInterfaces[0]->getvisflux()->getMomentum()) / dx
                          -(m_cellInterfaces[3]->getvisflux()->getMomentum() - m_cellInterfaces[2]->getvisflux()->getMomentum()) / dy;
    auto energy_rhs_vis = -(m_cellInterfaces[1]->getvisflux()->getEnergy() - m_cellInterfaces[0]->getvisflux()->getEnergy()) / dx
                        -(m_cellInterfaces[3]->getvisflux()->getEnergy() - m_cellInterfaces[2]->getvisflux()->getEnergy()) / dy;

    m_flux->setMass(mass_rhs);
    m_flux->setMomentum(momentum_rhs);
    m_flux->setEnergy(energy_rhs);

    // std::ofstream outFile("output.txt", std::ios::app);
    // if (outFile.is_open()) {
    //     outFile << "mass_rhs is " << mass_rhs << std::endl;
    //     outFile.close();
    // } else {
    //     std::cerr << "Failed to open output.txt for writing" << std::endl;
    // }

    // if(std::isnan(mass_rhs)){
    //     std::cerr << "mass_rhs is nan" << std::endl;
    // }
    // if(std::isnan(momentum_rhs[0]) || std::isnan(momentum_rhs[1])){
    //     std::cerr << "momentum_rhs is nan" << std::endl;
    // }
    // if(std::isnan(energy_rhs)){
    //     std::cerr << "energy_rhs is nan" << std::endl;
    // }

                    
    rhs->setMass(mass_rhs);
    rhs->setMomentum(momentum_rhs);
    rhs->setEnergy(energy_rhs);
    return rhs;
}

void Cell::comPrimitiveGradient()
{
    gradient->comprimitivederivativeForcell(this, m_stencilX, Direction::X);
    gradient->comprimitivederivativeForcell(this, m_stencilY, Direction::Y);
    // m_gradient->comviscousflux();
    // m_gradient->constructflux();
}

void Cell::comImmersedGhostPhase()
{
    m_imagepoint->InterpolatePhase();

    m_phase->setDensity(m_imagepoint->getPhase()->getDensity());
    m_phase->setU( - m_imagepoint->getPhase()->getU());
    m_phase->setV( - m_imagepoint->getPhase()->getV());
    m_phase->setTotalEnergy(m_imagepoint->getPhase()->getTotalEnergy());
    m_phase->computePressurefromOther();
}

void Cell::CpyPhase(Phase *otherPhase)
{
    m_phase->cpyPhase(*otherPhase);
}

void Cell::addFlux(const Flux &dflux)
{
    *m_flux = *m_flux + dflux;
}

void Cell::subtractFlux(const Flux &dflux)
{
    *m_flux = *m_flux - dflux;
}


Cell* Cell::getNeighborX(const int& k)
{
    if (k == 0) return this;
    Cell* curr = this;

    int step = (k > 0) ? 1 : -1;
    for (int i = 0; i < std::abs(k); ++i) {
        CellInterface* interface = (step > 0) ? curr->m_cellInterfaces[1] : curr->m_cellInterfaces[0];
        if (!interface) return nullptr;

        Cell* left = interface->getCellLeft();
        Cell* right = interface->getCellRight();

        if (curr == left && right) curr = right;
        else if (curr == right && left) curr = left;
        else return curr; 
    }

    return curr;
}

Cell* Cell::getNeighborY(const int& k)
{
    if (k == 0) return this;
    Cell* curr = this;

    int step = (k > 0) ? 1 : -1;
    for (int i = 0; i < std::abs(k); ++i) {
        CellInterface* interface = (step > 0) ? curr->m_cellInterfaces[3] : curr->m_cellInterfaces[2];
        if (!interface) return nullptr;

        Cell* bottom = interface->getCellLeft();
        Cell* top = interface->getCellRight();

        if (curr == bottom && top) curr = top;
        else if (curr == top && bottom) curr = bottom;
        else return curr;
    }

    return curr;
}
