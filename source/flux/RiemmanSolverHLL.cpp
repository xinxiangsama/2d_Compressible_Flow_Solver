#include "RiemmanSolverHLL.h"
#include <algorithm>
#include <cmath>

std::shared_ptr<Flux> RiemmanSolverHLL::Solver(std::unique_ptr<Phase>& leftPhase, 
                                               std::unique_ptr<Phase>& rightPhase, 
                                               std::shared_ptr<Flux> leftFlux, 
                                               std::shared_ptr<Flux>& rightFlux,
                                               Direction dir)
{
    std::shared_ptr<Flux> res;
    model->allocateFlux(res);

    double rhoL = leftPhase->getDensity();
    auto uL = leftPhase->getVelocity();
    double pL = leftPhase->getPressure();
    double EL = leftPhase->getTotalEnergy();
    double aL = std::sqrt(Gamma * pL / rhoL);

    double rhoR = rightPhase->getDensity();
    auto uR = rightPhase->getVelocity();
    double pR = rightPhase->getPressure();
    double ER = rightPhase->getTotalEnergy();
    double aR = std::sqrt(Gamma * pR / rhoR);

    // 根据方向确定特征速度
    int idx = (dir == Direction::X) ? 0 : 1;

    auto a_ave = 0.5 * (aL + aR);
    auto rho_ave = 0.5 * (rhoL + rhoR);
    auto p_star = 0.5 * (pL + pR) - 0.5 * (uR[idx] / rhoR - uL[idx] / rhoL) * rho_ave * a_ave;

    auto u_star = 0.5 * (uR[idx] / rhoR + uL[idx] / rhoL) - 0.5 * (pR - pL) / (rho_ave * a_ave);

    auto rho_star_L = rhoL + (uL[idx] / rhoL - u_star) * rho_ave / a_ave;
    auto rho_star_R = rhoR - (uR[idx] / rhoR - u_star) * rho_ave / a_ave;

    auto a_star_L = std::sqrt(Gamma * p_star / rho_star_L);
    auto a_star_R = std::sqrt(Gamma * p_star / rho_star_R);

    double SL = std::min(uL[idx] - aL, u_star - a_star_L);
    double SR = std::max(uL[idx] + aL, u_star + a_star_R);

    if (SL >= 0) {
        // *res = *leftFlux;
        res->setMass(leftFlux->getMass());
        res->setMomentum(leftFlux->getMomentum());
        res->setEnergy(leftFlux->getEnergy());
    }else if (SR <= 0) {
        // *res = *rightFlux;
        res->setMass(rightFlux->getMass());
        res->setMomentum(rightFlux->getMomentum());
        res->setEnergy(rightFlux->getEnergy());
    } else{
        double inv_SR_SL = 1.0 / (SR - SL);

        res->setMass((SR * leftFlux->getMass() - SL * rightFlux->getMass() 
                     + SL * SR * (rhoR - rhoL)) * inv_SR_SL);

        auto momentumL = leftFlux->getMomentum();
        auto momentumR = rightFlux->getMomentum();
        Eigen::Vector2d momentumHLL;

        momentumHLL[0] = (SR * momentumL[0] - SL * momentumR[0] 
                         + SL * SR * (uR[0] - uL[0])) * inv_SR_SL;
        momentumHLL[1] = (SR * momentumL[1] - SL * momentumR[1] 
                         + SL * SR * (uR[1] - uL[1])) * inv_SR_SL;

        res->setMomentum(momentumHLL);
        res->setEnergy((SR * leftFlux->getEnergy() - SL * rightFlux->getEnergy() 
                       + SL * SR * (ER - EL)) * inv_SR_SL);
    } 

    return res;
}
