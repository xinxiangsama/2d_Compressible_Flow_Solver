#include "Cylinder.h"

void Cylinder::fillIn(Cell *cell) const
{
    auto& phase = cell->getPhase();

    const double rho = 1.0;
    const double u = 1.0;
    const double v = 0.0;
    const double p = 1.0;

    phase->setDensity(rho);
    phase->setU(rho * u);
    phase->setV(rho * v);
    phase->setPressure(p);
    phase->computeEnergyfromOther();
}

void Cylinder::setBounbdaryCondition(std::vector<std::shared_ptr<Boundary>>& boundaries)
{
    // 左边：Dirichlet - 入口，设定恒定来流
    boundaries.push_back(std::make_shared<DirichletBoundary>(-1, 0, [](Phase& phase){
        const double rho = 1.0;
        const double u = 1.0;
        const double v = 0.0;
        const double p = 1.0;
        phase.setDensity(rho);
        phase.setU(rho * u);
        phase.setV(rho * v);
        phase.setPressure(p);
        phase.computeEnergyfromOther();
    }));

    // 右边：Neumann - 出口，允许流出
    boundaries.push_back(std::make_shared<NeummanBoundary>(1, 0));

    // 上边和下边：Neumann 边界或滑移边界（避免反射）
    boundaries.push_back(std::make_shared<NeummanBoundary>(0, -1)); // bottom
    boundaries.push_back(std::make_shared<NeummanBoundary>(0, 1));  // top
}