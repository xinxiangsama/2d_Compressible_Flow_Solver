#pragma once
#include "RiemmanSolver.h"

class RiemmanSolverHLL : public RiemmanSolver
{
public:
    RiemmanSolverHLL() = default;
    virtual ~RiemmanSolverHLL() = default;
    virtual std::shared_ptr<Flux> Solver(std::unique_ptr<Phase>& leftPhase, std::unique_ptr<Phase>& rightPhase, std::shared_ptr<Flux> leftFlux, std::shared_ptr<Flux>& rightFlux, Direction dir) override;

    virtual void info() override {std::cout <<"hll riemman solver"<<std::endl;}
};