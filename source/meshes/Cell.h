#pragma once
#include "CellInterface.h"
#include "Element.h"
#include <memory>
#include "../model/EulerPhase.h"
#include "../flux/EulerFlux.h"
#include "../model/model.h"
#include "../gradient/Gradient.h"
#include "../gradient/FourthOrderCS.h"
#include <algorithm>
#include "../object/ImagePoint.h"


enum class CellType {
    Fluid,
    Solid,
    ImmersedGhost
};

class CellInterface;
class ForthOrderCS;
class ImagePoint;
class Cell
{
public:
    Cell() = default;
    virtual ~Cell() = default;

    void addCellInterface(CellInterface* cellinterface);
    void deleteCellInterface(CellInterface* cellinterface);
    virtual void allocateVar();
    virtual void CpyPhase(Phase* otherPhase);
    virtual void addFlux(const Flux& dflux);
    virtual void subtractFlux(const Flux& dflux);

    // Access
    virtual std::unique_ptr<Element>& getElement();
    CellInterface* getCellInterface(const int& b);
    std::vector<CellInterface*>& getCellInterfaces() {return m_cellInterfaces;};
    virtual std::unique_ptr<Phase>& getPhase();
    virtual std::unique_ptr<Flux>& getFlux();
    virtual std::unique_ptr<Gradient>& getGradient() {return m_gradient;};
    virtual bool isCellGhost() const { return IfGhost; };
    Cell* getNeighborX(const int& k);
    Cell* getNeighborY(const int& k);
    virtual std::unique_ptr<ImagePoint>& getImagepoint() {return m_imagepoint;};

    // ImmersedBoundary
    void setType(CellType t) { type = t; }
    CellType getType() const { return type; }
    bool isImmersedGhost() const { return type == CellType::ImmersedGhost; }
    bool isSolid() const { return type == CellType::Solid; }
    bool isFluid() const { return type == CellType::Fluid; }

    // Modify
    void setCellInterface(const int& b, CellInterface* cellinterface);
    void setGhost() {IfGhost = true;}
    void collectStencilCells(Direction dir, int width);
    std::vector<Cell*>& getStencil(Direction dir);
    // Operator
    Cell(const Cell& other) = delete;
    Cell& operator=(const Cell& other) = delete;
    Cell(Cell&&) = default;
    virtual void Evolution();
    virtual std::unique_ptr<Flux> comRHS();
    virtual void comPrimitiveGradient();
    virtual void comImmersedGhostPhase();

    // Info
protected:
    std::vector<CellInterface*> m_cellInterfaces; // order is left->right->bottom->top
    std::unique_ptr<Phase> m_phase;
    std::unique_ptr<Element> m_element;
    std::unique_ptr<Flux> m_flux;
    std::unique_ptr<Gradient> m_gradient;
    std::vector<Cell*> m_stencilX; // neighbour cell needed to com gradient;
    std::vector<Cell*> m_stencilY;
    bool IfGhost {false};
    CellType type {CellType::Fluid};

    /*For immersed boundary*/
    std::unique_ptr<ImagePoint> m_imagepoint;
};

extern std::shared_ptr<Model> model;
extern std::shared_ptr<ForthOrderCS> gradient;