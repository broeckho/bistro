#include "GeoGrid.h"
#include <cmath>
#include <iostream>
#include <queue>
#include <util/Exception.h>
#include <utility>

namespace gengeopop {

GeoGrid::GeoGrid()
    : m_locations(), m_locationsToIdIndex(), m_population(stride::Population::Create()), m_finalized(false), m_tree()
{
}

GeoGrid::GeoGrid(std::shared_ptr<stride::Population> population)
    : m_locations(), m_locationsToIdIndex(), m_population(std::move(population)), m_finalized(false), m_tree()

{
}

GeoGrid::iterator GeoGrid::begin() { return m_locations.begin(); }

GeoGrid::iterator GeoGrid::end() { return m_locations.end(); }

void GeoGrid::AddLocation(std::shared_ptr<Location> location)
{
        if (m_finalized) {
                throw stride::util::Exception("Calling addLocation while GeoGrid is finalized is not supported!");
        }

        m_locations.emplace_back(location);
        m_locationsToIdIndex[location->GetID()] = location;
}

std::shared_ptr<Location> GeoGrid::operator[](size_t index) { return *(begin() + index); }
std::shared_ptr<Location>          GeoGrid::Get(size_t index) { return (*this)[index]; }

std::vector<std::shared_ptr<Location>> GeoGrid::TopK(size_t k) const
{
        auto cmp = [](const std::shared_ptr<Location>& rhs, const std::shared_ptr<Location>& lhs) {
                return rhs->GetPopulation() > lhs->GetPopulation();
        };

        std::priority_queue<std::shared_ptr<Location>, std::vector<std::shared_ptr<Location>>, decltype(cmp)> queue(
            cmp);

        for (auto it = cbegin(); it != cend(); it++) {
                queue.push(*it);
                if (queue.size() > k) {
                        queue.pop();
                }
        }

        std::vector<std::shared_ptr<Location>> topLocations;

        while (!queue.empty()) {
                auto loc = queue.top();
                topLocations.push_back(loc);
                queue.pop();
        }

        return topLocations;
}

GeoGrid::const_iterator GeoGrid::cbegin() const { return m_locations.cbegin(); }

GeoGrid::const_iterator GeoGrid::cend() const { return m_locations.cend(); }

size_t GeoGrid::size() const { return m_locations.size(); }

std::shared_ptr<Location> GeoGrid::GetById(unsigned int id) { return m_locationsToIdIndex.at(id); }

void GeoGrid::remove(const std::shared_ptr<Location>& location)
{
        m_locations.erase(std::remove(m_locations.begin(), m_locations.end(), location), m_locations.end());
        m_locationsToIdIndex.erase(location->GetID());
}

void GeoGrid::Finalize()
{
        std::vector<geogrid_detail::KdTree2DPoint> points;
        for (auto it = begin(); it != end(); ++it) {
                points.emplace_back(geogrid_detail::KdTree2DPoint(*it));
        }

        m_finalized = true;
        m_tree      = KdTree<geogrid_detail::KdTree2DPoint>::Build(points);
}

std::set<std::shared_ptr<Location>> GeoGrid::InBox(double long1, double lat1, double long2, double lat2) const
{
        CheckFinalized(__func__);

        std::set<std::shared_ptr<Location>> result;

        m_tree.Apply(
            [&result](const geogrid_detail::KdTree2DPoint& pt) -> bool {
                    result.insert(pt.GetLocation());
                    return true;
            },
            {{std::min(long1, long2), std::min(lat1, lat2)}, {std::max(long1, long2), std::max(lat1, lat2)}});
        return result;
}

std::vector<std::shared_ptr<Location>> GeoGrid::FindLocationsInRadius(std::shared_ptr<Location> start,
                                                                      double                    radius) const
{
        CheckFinalized(__func__);

        geogrid_detail::KdTree2DPoint startPt(start);

        std::vector<std::shared_ptr<Location>> result;
        auto agg = BuildAggregator<RadiusPolicy>(MakeCollector(result), std::make_tuple(std::move(startPt), radius));
        agg();
        return result;
}

std::shared_ptr<stride::Population> GeoGrid::GetPopulation() { return m_population; }

void GeoGrid::CheckFinalized(const std::string& functionName) const
{
        if (!m_finalized) {
                throw stride::util::Exception("Calling \"" + functionName +
                                              "\" while GeoGrid is not finalized is not supported!");
        }
}

} // namespace gengeopop
