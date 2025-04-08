#include "ImagePoint.h"
#include <Eigen/Sparse>
#include <Eigen/SVD>

void ImagePoint::setPosition(const Coord &position)
{
    m_positon = position;
}

void ImagePoint::allocateVar()
{
    model->allocatePhase(m_phase);
}

void ImagePoint::insertCell(Cell *cell)
{
    m_cells.push_back(cell);
}

void ImagePoint::InterpolatePhase()
{
    Eigen::Matrix4d CoefMatrix;
    Eigen::Vector4d rho, mu, mv, energy;
    for(int i = 0; i < 4; ++i){
        auto position = m_cells[i]->getElement()->getPosition();
        CoefMatrix(i, 0) = position.x() * position.y();
        CoefMatrix(i, 1) = position.x();
        CoefMatrix(i, 2) = position.y();
        CoefMatrix(i, 3) = 1.0;

        rho(i) = m_cells[i]->getPhase()->getDensity();
        mu(i) = m_cells[i]->getPhase()->getU();
        mv(i) = m_cells[i]->getPhase()->getV();
        energy(i) = m_cells[i]->getPhase()->getTotalEnergy();

    }

    // auto W_rho = CoefMatrix.colPivHouseholderQr().solve(rho);
    // auto W_mu = CoefMatrix.colPivHouseholderQr().solve(mu);
    // auto W_mv = CoefMatrix.colPivHouseholderQr().solve(mv);
    // auto W_energy = CoefMatrix.colPivHouseholderQr().solve(energy);


    auto solver = CoefMatrix.ldlt();  // 或 ldlt()
    if(solver.info() != Eigen::Success) {
        std::cout << "Matrix decomposition failed!" << std::endl;
    }
    auto W_rho = solver.solve(rho);
    auto W_mu = solver.solve(mu);
    auto W_mv = solver.solve(mv);
    auto W_energy = solver.solve(energy);

    // auto W_rho = CoefMatrix.fullPivLu().solve(rho);
    // auto W_mu = CoefMatrix.fullPivLu().solve(mu);
    // auto W_mv = CoefMatrix.fullPivLu().solve(mv);
    // auto W_energy = CoefMatrix.fullPivLu().solve(energy);



    // std::cout <<"w rho: " << W_rho(0) << ", " << W_rho(1) << ", " << W_rho(2) << ", " << W_rho(3) << std::endl;
    // std::cout <<"w mu: " << W_mu(0) << ", " << W_mu(1) << ", " << W_mu(2) << ", " << W_mu(3) << std::endl;
    // std::cout <<"w mv: " << W_mv(0) << ", " << W_mv(1) << ", " << W_mv(2) << ", " << W_mv(3) << std::endl;
    // std::cout <<"w energy: " << W_energy(0) << ", " << W_energy(1) << ", " << W_energy(2) << ", " << W_energy(3) << std::endl;


    m_phase->setDensity(W_rho(0) * m_positon.x() * m_positon.y() + W_rho(1) * m_positon.x() + W_rho(2) * m_positon.y() + W_rho(3));
    m_phase->setU(W_mu(0) * m_positon.x() * m_positon.y() + W_mu(1) * m_positon.x() + W_mu(2) * m_positon.y() + W_mu(3));
    m_phase->setV(W_mv(0) * m_positon.x() * m_positon.y() + W_mv(1) * m_positon.x() + W_mv(2) * m_positon.y() + W_mv(3));
    m_phase->setTotalEnergy(W_energy(0) * m_positon.x() * m_positon.y() + W_energy(1) * m_positon.x() + W_energy(2) * m_positon.y() + W_energy(3));

    // std::cout << "ImagePoint position: " << m_positon.x() << ", " << m_positon.y() << std::endl;
    // std::cout << "ImagePoint density: " << m_phase->getDensity() << std::endl;
    // std::cout << "ImagePoint velocity: " << m_phase->getU() << ", " << m_phase->getV() << std::endl;
    // std::cout << "ImagePoint energy: " << m_phase->getTotalEnergy() << std::endl;
    // std::cout << "----------------------------------------" << std::endl;
    // For test
    // m_phase->setDensity(0.25 * rho(0) + 0.25 * rho(1) + 0.25 * rho(2) + 0.25 * rho(3));
    // m_phase->setU(0.25 * mu(0) + 0.25 * mu(1) + 0.25 * mu(2) + 0.25 * mu(3));
    // m_phase->setV(0.25 * mv(0) + 0.25 * mv(1) + 0.25 * mv(2) + 0.25 * mv(3));
    // m_phase->setEnergy(0.25 * energy(0) + 0.25 * energy(1) + 0.25 * energy(2) + 0.25 * energy(3));
    // m_phase->computePressurefromOther();
}
