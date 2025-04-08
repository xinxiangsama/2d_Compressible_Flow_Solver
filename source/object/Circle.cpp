#include "Circle.h"

Circle::Circle(const double &radius, const Coord &center)
{
    m_radius = radius;
    m_center = center;
    numLagrangianPoints = 0;
    m_points.clear();
    m_segments.clear();
}

void Circle::Initialize(const size_t &numpoints)
{
    numLagrangianPoints = numpoints;
    m_points.resize(numLagrangianPoints);
    m_segments.resize(numLagrangianPoints);
    // Set the position of each Lagrangian point
    for (size_t i = 0; i < numLagrangianPoints; ++i)
    {
        double angle = 2.0 * M_PI * i / numLagrangianPoints;
        m_points[i].setPosition(m_center + m_radius * Eigen::Vector2d(cos(angle), sin(angle)));
    }

    // Set the segments between Lagrangian points
    for (size_t i = 0; i < numLagrangianPoints; ++i)
    {
        m_segments[i].setleftpoint(&m_points[i]);
        m_segments[i].setrightpoint(&m_points[(i + 1) % numLagrangianPoints]);
        m_points[i].setLeftSegment(&m_segments[i]);
        m_points[i].setRightSegment(&m_segments[(i + 1) % numLagrangianPoints]);
    }

    // Set the information of each segment
    for(auto& segment : m_segments)
    {
        auto vector = segment.getrightpoint()->getPosition() - segment.getleftpoint()->getPosition();
        segment.setlength(vector.norm());
        segment.setsloop(vector.normalized());
        segment.setnormal(Eigen::Vector2d(-vector.y(), vector.x()).normalized());
    }
}

void Circle::DistinguishWallandFluid(Cell *cell)
{
    auto position = cell->getElement()->getPosition();
    auto distance = (position - m_center).norm();
    if(distance <= m_radius)
    {
        cell->setType(CellType::Solid);
    }
}

void Circle::SetGhostcell(std::vector<std::vector<Cell>> &cells, const int& num)
{
    const int nx = static_cast<int>(cells.size());
    const int ny = static_cast<int>(cells[0].size());

    for (int i = 0; i < nx; ++i)
    {
        for (int j = 0; j < ny; ++j)
        {
            Cell &cell = cells[i][j];
            if (!cell.isFluid())
                continue;

            // 检查四个方向
            const std::vector<std::pair<int, int>> directions = {
                {1, 0},  // right
                {-1, 0}, // left
                {0, 1},  // top
                {0, -1}  // bottom
            };

            for (const auto &[di, dj] : directions)
            {
                int ni = i + di;
                int nj = j + dj;

                // 判断第一层是否为 Wall
                if (ni >= 0 && ni < nx && nj >= 0 && nj < ny &&
                    (cells[ni][nj].getType() == CellType::Solid || 
                     cells[ni][nj].getType() == CellType::ImmersedGhost))
                {
                    // 设置三层 Ghost cell
                    for (int k = 1; k <= num; ++k)
                    {
                        int gi = i + k * di;
                        int gj = j + k * dj;
                        if (gi >= 0 && gi < nx && gj >= 0 && gj < ny &&
                            (cells[gi][gj].isSolid() || cells[gi][gj].isImmersedGhost()))
                        {
                            cells[gi][gj].setType(CellType::ImmersedGhost);
                        }
                        else
                        {
                            break; // 遇到边界或非Wall，不再继续设置
                        }
                    }
                }
            }
        }
    }
}

void Circle::SetImagepoint(Cell* cell, std::vector<std::vector<Cell>>& cells)
{
    /*step 1 : find the closest sgement of ghost cell*/
    auto position = cell->getElement()->getPosition();
    double minDistance = std::numeric_limits<double>::max();
    Segment* closestSegment = nullptr;
    Coord interceptPoint;
    for(auto & segment : m_segments){
        auto startPoint = segment.getleftpoint()->getPosition();
        auto Projection = segment.getsloop().dot(position - startPoint) * segment.getsloop() + startPoint;
        auto distance = (Projection - position).norm();
        if(distance < minDistance){
            minDistance = distance;
            closestSegment = &segment;
            interceptPoint = Projection;
        }
    }
    // auto normal = -closestSegment->getnormal();
    auto normal = (interceptPoint - position).normalized();
    auto imagePoint = position + 2 * (minDistance * normal);
    cell->getImagepoint() = std::make_unique<ImagePoint>();
    cell->getImagepoint()->setPosition(imagePoint);
    if((imagePoint.x() - m_center.x()) * (imagePoint.x() - m_center.x()) + (imagePoint.y() - m_center.y()) * (imagePoint.y() - m_center.y()) <= m_radius * m_radius){
        std::cerr << "Warning: Image point is inside the circle." << std::endl;
        std::cerr << "Image point position: " << imagePoint.x() << ", " << imagePoint.y() << std::endl;
        std::cerr << "Original cell position: " << position.x() << ", " << position.y() << std::endl;
    }

    cell->getImagepoint()->allocateVar();
    // FindNearest4Cell(cell->getImagepoint().get(), cells);
    FindNearest4FluidCellsIndexed(cell->getImagepoint().get(), cells);


}

void Circle::FindNearest4Cell(ImagePoint* imagepoint, std::vector<std::vector<Cell>> &cells)
{
    auto position = imagepoint->getPosition();
    std::vector<std::pair<double, Cell*>> candidateCells;
    for(auto& col : cells){
        for(auto& cell : col){
            if(cell.isFluid()){
                // auto distance = (cell.getElement()->getPosition() - position).norm();
                // candidateCells.emplace_back(distance, &cell);
            }
        }
    }
    // std::sort(candidateCells.begin(), candidateCells.end(), [](const auto& a, const auto& b) {
    //     return a.first < b.first;
    // });
    // for(int i = 0; i < 4; ++i){
    //     imagepoint->insertCell(candidateCells[i].second);
    // }
}

void Circle::FindNearest4FluidCellsIndexed(ImagePoint* imagepoint, std::vector<std::vector<Cell>> &cells)
{    if (cells.empty() || cells[0].empty()) return;

    auto imagePos = imagepoint->getPosition();
    const int Nx = cells.size();
    const int Ny = cells[0].size();
    auto dx = cells[0][0].getElement()->getdx();
    auto dy = cells[0][0].getElement()->getdy();
    auto Origin = cells[0][0].getElement()->getPosition();

    // 估算 imagePos 所在的 cell 的索引
    int i_center = static_cast<int>((imagePos.x() - Origin.x()) / dx);
    int j_center = static_cast<int>((imagePos.y() - Origin.y()) / dy);

    // std::cout <<"Origin: " << Origin.x() << ", " << Origin.y() << std::endl;
    // std::cout << "imagePos: " << imagePos.x() << ", " << imagePos.y() << std::endl;
    // std::cout << "dx: " << dx << ", dy: " << dy << std::endl;
    // std::cout << "i_center: " << i_center << ", j_center: " << j_center << std::endl;

    using CellDistPair = std::pair<double, Cell*>;
    std::vector<CellDistPair> candidates;

    const int window = 5; // 3×3，或改成更大如 5

    for (int i = i_center - window; i <= i_center + window; ++i)
    {
        for (int j = j_center - window; j <= j_center + window; ++j)
        {
            if (i >= 0 && i < Nx && j >= 0 && j < Ny)
            {
                Cell& cell = cells[i][j];
                if (cell.getType() == CellType::Fluid)
                {
                    double dist = (cell.getElement()->getPosition() - imagePos).squaredNorm();
                    candidates.emplace_back(dist, const_cast<Cell*>(&cell));
                }
            }
        }
    }

    // 排序并选出前 4 个
    std::sort(candidates.begin(), candidates.end(),
              [](const CellDistPair& a, const CellDistPair& b) {
                  return a.first < b.first;
              });
    if(candidates.size() < 4){
        std::cerr << "Warning: Not enough fluid cells found for interpolation." << std::endl;
        std::cerr << "Found: " << candidates.size() << ", Expected: 4" << std::endl;
        return;
    }

    for (size_t i = 0; i < std::min(candidates.size(), size_t(4)); ++i)
    {
        imagepoint->insertCell(candidates[i].second);
    }
}
