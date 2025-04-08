#pragma once

#include "Face.h"
#include "Cell.h"
#include <memory>
#include "../model/EulerPhase.h"
#include "../flux/EulerFlux.h"
#include "../model/model.h"
#include "../interpolation/Interpolation.h"
#include <iostream>
#include <list>
#include "../flux/RiemmanSolverHLL.h"
#include "../param.h"
// #include "../gradient/EulerGradient.h"
#include "../gradient/FourthOrderCS.h"

enum class InterfaceType
{
    fluid,
    solid,
    immersed
};

class ForthOrderCS;
class Cell;
class Model;
class RiemmanSolver;
class Interpolation;
class Gradient;
class CellInterface
{
public:
    CellInterface() = default;
    virtual ~CellInterface() = default;

    void setLeftFace(std::unique_ptr<Face>&& other);
    void setRightFace(std::unique_ptr<Face>&& other);
    void initialize(Cell* cellLeft, Cell* cellRight);
    void setLeftCell(Cell* cellleft);
    void setRightCell(Cell* cellright);

    virtual void allocateVar();
    virtual void comInterpolationPhase();
    virtual void computeFlux();
    virtual void comviscousflux();
    virtual void addFlux(const Flux& dflux);
    virtual void subtractFlux(const Flux& dflux);
    virtual void collectCells();

    // Modify
    void SetBoundary() {Ifboundary = true;}
    void SetProcessBoundary() {IfProcessBoundary = true;}
    void SetHorizontal() {IfHorizontal = true;}
    void DistinguishType();

    // Access
    Face* getLeftFace () const;
    Face* getRightFace () const;
    Cell* getCellLeft() const;
    Cell* getCellRight() const;
    bool getBoundary(){return Ifboundary;}
    bool getProcessBoundary() {return IfProcessBoundary;}
    bool getHorizontal() {return IfHorizontal;}
    std::unique_ptr<Phase>& getleftphase() {return m_phaseLeft;}
    std::unique_ptr<Phase>& getrightphase() {return m_phaseRight;}
    std::shared_ptr<Flux>& getflux() {return m_flux;}
    std::shared_ptr<Flux>& getvisflux() {return m_visflux;}
    bool isWallface() const {return m_type == InterfaceType::solid;}

    // operator
    CellInterface(const CellInterface& other) = delete;
    CellInterface& operator=(const CellInterface& other) = delete;
    CellInterface(CellInterface&&) = default;
    
protected:
    Cell* m_cellLeft;
    Cell* m_cellRight;
    std::unique_ptr<Face> m_Leftface;
    std::unique_ptr<Face> m_Rightface;
    bool Ifboundary {false};
    bool IfProcessBoundary {false};
    bool IfHorizontal {false};

    std::unique_ptr<Phase> m_phaseLeft; /*Face value needed to be interpolated*/
    std::unique_ptr<Phase> m_phaseRight;
    std::shared_ptr<Flux> m_flux; // inviscid flux
    std::shared_ptr<Flux> m_visflux; // viscous flux
    std::list<Cell*> m_Cells; /*cells needed for interpolation*/

    InterfaceType m_type {InterfaceType::fluid};

    friend class Cell;
};

extern std::shared_ptr<Model> model;
extern std::shared_ptr<Interpolation> interpolation;
extern std::shared_ptr<RiemmanSolver> riemmansolver;
extern std::shared_ptr<ForthOrderCS> gradient;