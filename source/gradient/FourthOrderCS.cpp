#include "FourthOrderCS.h"


inline auto dot6 = [](const std::array<double, 6>& vec, const std::array<double, 6>& coef) {
    double sum = 0.0;
    for (int i = 0; i < 6; ++i) sum += coef[i] * vec[i];
    return sum;
};

inline auto dot4 = [](const std::array<double, 4>& vec, const std::array<double, 4>& coef) {
    double sum = 0.0;
    for (int i = 0; i < 4; ++i) sum += coef[i] * vec[i];
    return sum;
};

void ForthOrderCS::comviscousfluxForcellinterface(CellInterface* cellinterface, std::list<Cell*>& stencil, Direction dir)
{   
    // Stencil cells: from i-3 to i+2 (total 6 cells)
    auto idx = (dir == Direction::X) ? 0 : 1;
    auto idxcross = (dir == Direction::X) ? 1 : 0;
    std::array<Cell*, cellNum> cells;
    auto it = stencil.begin();
    for (int i = 0; i < cellNum && it != stencil.end(); ++i, ++it) {
        cells[i] = *it;
    }

    // auto dx = cells[static_cast<int>(cellNum/ 2)]->getElement()->getdx();
    // auto dy = cells[static_cast<int>(cellNum/ 2)]->getElement()->getdy();

    // auto dx = (dir == Direction::X) ? cells[2]->getElement()->getdx() : cells[2]->getElement()->getdy();

    auto dx = 0.002;

    // Extract primitive variables at stencil cells
    std::array<double, 6> u, v, T;
    for (int i = 0; i < 6; ++i) {
        const auto& ph = *(cells[i]->getPhase());
        u[i] = ph.getPrimitiveU();
        v[i] = ph.getPrimitiveV();
        T[i] = ph.getTemperature();
    }
    // std::cout <<"max of u: " << *std::max_element(u.begin(), u.end()) << std::endl;
    // std::cout <<"min of u: " << *std::min_element(u.begin(), u.end()) << std::endl;
    // Prepare array for cross-direction derivatives ∂φ/∂x₂
    std::array<double, 4> u_d, v_d, T_d;
    std::array<double, 4> u_4, v_4, viscosity_4;
    for(int i =1; i < 5; ++i){
        const auto& dir = *(cells[i]->getGradient());
        u_d[i-1] = dir.getUGradient()[idxcross];
        v_d[i-1] = dir.getVGradient()[idxcross];
        T_d[i-1] = dir.getTGradient()[idxcross];
        u_4[i-1] = cells[i]->getPhase()->getPrimitiveU();
        v_4[i-1] = cells[i]->getPhase()->getPrimitiveV();
        // mu[i-1] = cells[i]->getPhase()->getViscosity();
    }
    // std::cout <<"max of u_d: " << *std::max_element(u_d.begin(), u_d.end()) << std::endl;
    // std::cout <<"min of u_d: " << *std::min_element(u_d.begin(), u_d.end()) << std::endl;


    // Loop over each of the 3 interface positions

    std::array<double, 3> visMu, visMv, visE;
    for (int r = 0; r < 3; ++r) {
        // Compute derivatives in the main direction using θ^r
        double dud_main = dot6(u, theta_r[r]) / dx;
        double dvd_main = dot6(v, theta_r[r]) / dx;
        double dTd_main = dot6(T, theta_r[r]) / dx;

        // Compute cross-direction derivatives using ζ^r 
        double dud_cross = dot4(u_d, zeta_r[r]);
        double dvd_cross = dot4(v_d, zeta_r[r]);
        double dTd_cross = dot4(T_d, zeta_r[r]);
        double u_construct = dot4(u_4, zeta_r[r]);
        double v_construct = dot4(v_4, zeta_r[r]);
        // double mu_construct = dot4(mu, zeta_r[r]);
        // Compute the viscous fluxes
        if(dir == Direction::X){
            auto sigma_11 = 2 * mu_ref * (2.0 / 3.0) * dud_main;
            auto sigma_12 = mu_ref * (dud_cross + dvd_cross);
            auto q_1 = (-mu_ref / ((Gamma - 1) * pow(Ma_ref, 2))) * dTd_main;

            visMu[r] = sigma_11;
            visMv[r] = sigma_12;
            visE[r] = sigma_11 * u_construct + sigma_12 * v_construct + q_1;

            // use for checking
            // visMu[r] = dud_main;
            // visMv[r] = dvd_main;
            // visE[r] = u[2];
        }else{
            auto sigma_22 = 2 * mu_ref * (2.0 / 3.0) * dvd_main;
            auto sigma_21 = mu_ref * (dud_cross + dvd_cross);
            auto q_2 = (-mu_ref / ((Gamma - 1) * pow(Ma_ref, 2))) * dTd_main;

            visMu[r] = sigma_21;
            visMv[r] = sigma_22;
            visE[r] = sigma_21 * u_construct + sigma_22 * v_construct + q_2;
            // use for checking
            // visMu[r] = dud_main;
            // visMv[r] = dvd_main;
            // visE[r] =  v[2];
        }

    }
    // Compute the viscous fluxes at the cell interface
    auto visMu_contruct = (-1.0 / 24.0) * visMu[0] + (26.0 / 24.0) * visMu[1] + (-1.0 / 24.0) * visMu[2];
    auto visMv_contruct = (-1.0 / 24.0) * visMv[0] + (26.0 / 24.0) * visMv[1] + (-1.0 / 24.0) * visMv[2];
    auto visE_contruct = (-1.0 / 24.0) * visE[0] + (26.0 / 24.0) * visE[1] + (-1.0 / 24.0) * visE[2];

    cellinterface->getvisflux()->setMass(0.0);
    cellinterface->getvisflux()->setMomentum({visMu_contruct, visMv_contruct});
    cellinterface->getvisflux()->setEnergy(visE_contruct);

    // std::cout << "visMu_contruct: " << visMu_contruct << std::endl;
    // std::cout << "visMv_contruct: " << visMv_contruct << std::endl;
    // std::cout << "visE_contruct: " << visE_contruct << std::endl;

}

void ForthOrderCS::comprimitivederivativeForcell(Cell* cell, std::vector<Cell*> &stencil, Direction dir)
{
    std::array<double,5> u {}, v {}, T {};
    int i = 0;
    for(auto it = stencil.begin(); it != stencil.end(); ++it){
        auto& ph = *(*it)->getPhase();
        u[i] = ph.getPrimitiveU();
        v[i] = ph.getPrimitiveV();
        T[i] = ph.getTemperature();
        ++i;
    }
    // Compute derivatives in the main direction using θ^c
    int idx = (dir == Direction::X) ? 0 : 1;
    double dx = (dir == Direction::X) ? cell->getElement()->getdx() : cell->getElement()->getdy(); 
    double du = Fouthcs(u) / dx;
    double dv = Fouthcs(v) / dx;
    double dT = Fouthcs(T) / dx;

    cell->getGradient()->setUGradient(du, idx);
    cell->getGradient()->setVGradient(dv, idx);
    cell->getGradient()->setTGradient(dT, idx);

}

double ForthOrderCS::Fouthcs(const std::array<double, 5> &v)
{
    double result = 0.0;
    for (int i = 0; i < 5; ++i) {
        result += theta_c[i] * v[i];
    }
    return result;
}

const int &ForthOrderCS::getNeedCellnum()
{
    return cellNum;
}