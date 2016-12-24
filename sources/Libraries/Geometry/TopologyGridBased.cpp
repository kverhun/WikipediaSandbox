#include "TopologyGridBased.h"

#include "Utils.h"

#include <iterator>
#include <algorithm>
#include <limits>

using Geometry::Point2d;
using Geometry::Rectangle2d;
using Geometry::ITopology;


namespace
{
    template<typename K, typename V>
    std::vector<V> _RetrieveMapValues(const std::map<K, V>& i_map)
    {
        std::vector<V> result;
        std::transform(i_map.begin(), i_map.end(), std::back_inserter(result), [&i_map](const std::pair<K, V>& i_el) {return i_el.second; });
        return std::move(result);
    }

    class _Grid
    {
    public:
        using TCellID = std::pair<size_t, size_t>;
        using TContentID = ITopology::TPointID;
        using TCellContent = std::vector<TContentID>;

        _Grid(const ITopology::TPoints& i_points)
            : m_points(i_points)
        {
            auto rectangle = [this]() -> Rectangle2d
            {
                double x_min = m_points.begin()->second.GetX();
                double y_min = m_points.begin()->second.GetY();
                double x_max = x_min;
                double y_max = y_min;

                for (const auto& entry : m_points)
                {
                    double x = entry.second.GetX();
                    double y = entry.second.GetY();
                    if (x < x_min)
                        x_min = x;
                    if (x > x_max)
                        x_max = x;
                    if (y < y_min)
                        y_min = y;
                    if (y > y_max)
                        y_max = y;
                }
                // inflate box to make sure content is on the edges
                auto delta = std::max(x_max - x_min, y_max - y_min) * 0.01;
                x_min -= delta;
                y_min -= delta;
                x_max += delta;
                y_max += delta;
                return Rectangle2d(Point2d{ x_min, y_min }, Point2d{x_max, y_max});
            }();

            double x_size = rectangle.GetPointMax().GetX() - rectangle.GetPointMin().GetX();
            double y_size = rectangle.GetPointMax().GetY() - rectangle.GetPointMin().GetY();

            size_t grid_dimention = static_cast<size_t>(std::sqrt(static_cast<double>(m_points.size())));

            double x_cell_size = x_size / grid_dimention;
            double y_cell_size = y_size / grid_dimention;

            double x_min = rectangle.GetPointMin().GetX();
            double y_min = rectangle.GetPointMin().GetY();

            m_x_cells.resize(grid_dimention);
            m_y_cells.resize(grid_dimention);
            for (size_t i = 0; i < grid_dimention; ++i)
            {
                m_x_cells[i] = x_min + i * x_cell_size;
                m_y_cells[i] = y_min + i * y_cell_size;
            }

            for (size_t x = 0; x < m_x_cells.size(); ++x)
                for (size_t y = 0; y < m_y_cells.size(); ++y)
                    m_grid_content.insert({ {x,y},{} });

            for (const auto& pt : m_points)
            {
                auto cell_id = _GetPointCell(pt.second);
                m_grid_content[cell_id].push_back(pt.first);
            }
        }

        ITopology::TPoints GetPointsInRectangle(const Rectangle2d& i_rectangle) const
        {
            ITopology::TPoints result;

            auto min_cell = _GetPointCell(i_rectangle.GetPointMin());
            auto max_cell = _GetPointCell(i_rectangle.GetPointMax());

            for (auto x_id = min_cell.first; x_id <= max_cell.first; ++x_id)
            {
                for (auto y_id = min_cell.second; y_id <= max_cell.second; ++y_id)
                {
                    for (const auto& content : GetCellContent({ x_id, y_id }))
                    {
                        auto pt = m_points.at(content);
                        if (i_rectangle.DoesContainPoint(pt))
                            result.insert({ content, pt});
                    }
                }
            }
            return result;
        }

        void SetCellContent(const TCellID& i_cell_id, const TCellContent& i_cell_content)
        {
            m_grid_content.insert({ i_cell_id, i_cell_content });
        }

        const TCellContent& GetCellContent(const TCellID& i_cell_id) const
        {
            return m_grid_content.at(i_cell_id);
        }

    private:
        TCellID _GetPointCell(const Point2d& i_point) const
        {

            auto get_cell = [](const double& i_value, const std::vector<double>& i_cells) -> size_t
            {
                for (size_t i = 1; i < i_cells.size(); ++i)
                {
                    if (i_cells[i-1] < i_value && i_value < i_cells[i])
                        return i - 1;
                }
                if (i_value < i_cells.front())
                    return 0;
                else
                    return i_cells.size() - 1;
            };

            double pt_x = i_point.GetX();
            double pt_y = i_point.GetY();
            size_t x_cell_id = get_cell(pt_x, m_x_cells);
            size_t y_cell_id = get_cell(pt_y, m_y_cells);
            return std::make_pair(x_cell_id, y_cell_id);
        }

    private:
        std::map<TCellID, TCellContent> m_grid_content;
        const ITopology::TPoints& m_points;

        std::vector<double> m_x_cells;
        std::vector<double> m_y_cells;
    };

    class TopologyGridBased : public ITopology
    {
    public:
        TopologyGridBased(const TPoints& i_points)
            : m_points(i_points)
            , m_point_list(_RetrieveMapValues(m_points))
            , m_grid(m_points)
        {}

        const TPoints& GetPoints() const
        {
            return m_points;
        }

        const TPointList& GetPointList() const
        {
            return m_point_list;
        }

        TPoints GetPointsInRectangle(const Rectangle2d& i_rectangle) const
        {
            return m_grid.GetPointsInRectangle(i_rectangle);
        }

    private:
        TPoints m_points;
        TPointList m_point_list;

        _Grid m_grid;
    };
}

std::unique_ptr<ITopology> Geometry::CreateGridBasedTopology(const ITopology::TPoints& i_points)
{
    return std::make_unique<TopologyGridBased>(i_points);
}