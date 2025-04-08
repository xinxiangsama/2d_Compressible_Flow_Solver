#pragma ocne
#include "EulerFlux.h"
#include "../model/model.h"
class Model;

class RiemmanSolver
{
public:
    RiemmanSolver() = default;
    virtual ~RiemmanSolver() = default;

    virtual std::shared_ptr<Flux> Solver(std::unique_ptr<Phase>&, std::unique_ptr<Phase>&, std::shared_ptr<Flux>, std::shared_ptr<Flux>&, Direction ) {};

    virtual void info() {std::cout <<"default riemman solver"<<std::endl;}
};

extern std::shared_ptr<Model> model;