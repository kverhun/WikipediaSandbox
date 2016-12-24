#include "TopologySimple.h"

#include <algorithm>
#include <iterator>

namespace
{
    template<typename K, typename V>
    std::vector<V> _RetrieveMapValues(const std::map<K, V>& i_map)
    {
        std::vector<V> result;
        std::transform(i_map.begin(), i_map.end(), std::back_inserter(result), [&i_map](const std::pair<K, V>& i_el) {return i_el.second; });
        return std::move(result);
    }

    class TopologySimple : public Geometry::ITopology
    {
    public:
        TopologySimple(const TPoints& i_points)
            : m_points(i_points)
            , m_point_list(_RetrieveMapValues(m_points))
        { }

        TopologySimple() {}

        const TPoints& GetPoints() const
        {
            return m_points;
        }

        virtual const TPointList& GetPointList() const
        {
            return m_point_list;
        }

        TPoints GetPointsInRectangle(const Geometry::Rectangle2d& i_rectangle) const override
        {
            TPoints result;

            for (const auto& pt : m_points)
                if (i_rectangle.DoesContainPoint(pt.second))
                    result.insert(pt);

            return result;
        }

    private:
        TPoints m_points;
        TPointList m_point_list;
    };
}

std::unique_ptr<Geometry::ITopology> Geometry::CreateSimpleTopology(const Geometry::ITopology::TPoints& i_points)
{
    return std::make_unique<TopologySimple>(i_points);
}