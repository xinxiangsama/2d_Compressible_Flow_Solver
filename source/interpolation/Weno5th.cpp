// Weno5th.cpp
#include "Weno5th.h"

inline double Weno5th::weno5Interpolation(const std::array<double,5>& v){
    constexpr double eps = 1e-2; 

    double beta0 = (13.0/12.0)*( (v[0]-2*v[1]+v[2])*(v[0]-2*v[1]+v[2]) ) 
                    + 0.25*( (v[0]-4*v[1]+3*v[2])*(v[0]-4*v[1]+3*v[2]) );
    double beta1 = (13.0/12.0)*( (v[1]-2*v[2]+v[3])*(v[1]-2*v[2]+v[3]) ) 
                    + 0.25*( (v[1]-v[3])*(v[1]-v[3]) );
    double beta2 = (13.0/12.0)*( (v[2]-2*v[3]+v[4])*(v[2]-2*v[3]+v[4]) ) 
                    + 0.25*( (3*v[2]-4*v[3]+v[4])*(3*v[2]-4*v[3]+v[4]) );

    double alpha0 = 0.1 / ((eps + beta0)*(eps + beta0));
    double alpha1 = 0.6 / ((eps + beta1)*(eps + beta1));
    double alpha2 = 0.3 / ((eps + beta2)*(eps + beta2));

    double sum_alpha = alpha0 + alpha1 + alpha2;

    double omega0 = alpha0 / sum_alpha;
    double omega1 = alpha1 / sum_alpha;
    double omega2 = alpha2 / sum_alpha;

    return omega0*(v[0]/3.0 - 7.0*v[1]/6.0 + 11.0*v[2]/6.0)
         + omega1*(-v[1]/6.0 + 5.0*v[2]/6.0 + v[3]/3.0)
         + omega2*(v[2]/3.0 + 5.0*v[3]/6.0 - v[4]/6.0);
    // return v[2];
}

void Weno5th::comInterpolation(std::array<Cell*,5>& cells, std::unique_ptr<Phase>& phase)
{
    std::array<double,5> rho {}, mu {}, mv {}, energy {};

    for(int i=0; i<5; ++i){
        auto& ph = *(cells[i]->getPhase());
        rho[i]    = ph.getDensity();
        mu[i]     = ph.getU();
        mv[i]     = ph.getV();
        energy[i] = ph.getTotalEnergy();
    }

    phase->setDensity(weno5Interpolation(rho));
    phase->setU(weno5Interpolation(mu));
    phase->setV(weno5Interpolation(mv));
    phase->setTotalEnergy(weno5Interpolation(energy));

    // // For test
    // phase->setDensity(0.25 * rho[0] + 0.25 * rho[1] + 0.25 * rho[2] + 0.25 * rho[3]);
    // phase->setU(0.25 * mu[0] + 0.25 * mu[1] + 0.25 * mu[2] + 0.25 * mu[3]);
    // phase->setV(0.25 * mv[0] + 0.25 * mv[1] + 0.25 * mv[2] + 0.25 * mv[3]);
    // phase->setTotalEnergy(0.25 * energy[0] + 0.25 * energy[1] + 0.25 * energy[2] + 0.25 * energy[3]);

    //get pressure from directly compute
    phase->computePressurefromOther();
}

void Weno5th::comLeftPhase(std::list<Cell*>& cells, std::unique_ptr<Phase>& phase)
{
    std::array<Cell*,5> fivecells;
    auto it = cells.begin();
    // for(int i=0; i<5 && it!=cells.end(); ++i,++it){
    //     fivecells[i] = *it;
    // }
    for(int i=0; i<5; ++i,++it){
        fivecells[i] = *it;
    }
    comInterpolation(fivecells,phase);
}

void Weno5th::comRightPhase(std::list<Cell*>& cells, std::unique_ptr<Phase>& phase)
{
    std::array<Cell*,5> fivecells;
    auto it = std::prev(cells.end());
    // for(int i=0; i<5; ++i,--it){
    //     fivecells[4-i] = *it; //right phase order is reverse
    // }
    for(int i=0; i<5; --it,++i){
        fivecells[i] = *it;
    }
    comInterpolation(fivecells,phase);
}
