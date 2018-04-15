#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

#include "KdTree.h"
#include "Location.h"

namespace gengeopop {

inline double degreeToRadian(double degree) { return (degree * M_PI) / 180.0; }

inline double radianToDegree(double radian) { return (180 * radian) / M_PI; }

class GeoGrid
{
public:
        using iterator       = std::vector<std::shared_ptr<Location>>::iterator;
        using const_iterator = std::vector<std::shared_ptr<Location>>::const_iterator;

        GeoGrid();

        void addLocation(std::shared_ptr<Location> location);

        /**
         * Disables the addLocation method and builds the kdtree.
         */
        void finalize();

        /**
         * Search for locations in \p radius arodun \ start
         */
        std::set<std::shared_ptr<Location>> findLocationsInRadius(std::shared_ptr<Location> start, double radius) const;

        /**
         * @param k
         * @return the K biggest Location of this GeoGrid
         */
        std::vector<std::shared_ptr<Location>> topK(size_t k) const;

        /**
         * Get the Locations location in a rectangle determined by the two coordinates (long1, lat1) and (long2, lat2).
         *
         * The coordinates must be position on the diagonal, i.e:
         *
         *  p1 -----+     +-------p1
         *  |       |     |       |
         *  |       |  or |       |
         *  |       |     |       |
         *  +-------p2    p2------+
         */
        std::set<std::shared_ptr<Location>> inBox(double long1, double lat1, double long2, double lat2) const;

        std::set<std::shared_ptr<Location>> inBox(const std::shared_ptr<Location>& loc1,
                                                  std::shared_ptr<Location>&       loc2) const
        {
                return inBox(loc1->getCoordinate().longitude, loc1->getCoordinate().latitude,
                             loc2->getCoordinate().longitude, loc2->getCoordinate().latitude);
        }

        iterator begin();
        iterator end();

        const_iterator cbegin() const;
        const_iterator cend() const;

        size_t size() const;

        void remove(const std::shared_ptr<Location>& location);

        std::shared_ptr<Location> operator[](size_t index);

        std::shared_ptr<Location> get(size_t index);

        std::shared_ptr<Location> GetById(unsigned int id);

private:
        std::vector<std::shared_ptr<Location>>                      m_locations;
        std::unordered_map<unsigned int, std::shared_ptr<Location>> m_locationsToIdIndex;

        bool m_finalized;

        class KdTree2DPoint
        {
        public:
                explicit KdTree2DPoint(const std::shared_ptr<Location>& location)
                    : m_location(location), m_longitude(location->getCoordinate().longitude),
                      m_latitude(location->getCoordinate().latitude)
                {
                }

                KdTree2DPoint() : m_location(nullptr), m_longitude(), m_latitude(){};

                KdTree2DPoint(double longt, double lat) : m_location(nullptr), m_longitude(longt), m_latitude(lat) {}

                constexpr static std::size_t dim = 2;

                bool operator==(const KdTree2DPoint& o) const
                {
                        return o.m_longitude == m_longitude && o.m_latitude == m_latitude;
                }
                bool operator<(const KdTree2DPoint& o) const
                {
                        return std::make_pair(m_longitude, m_latitude) < std::make_pair(o.m_longitude, o.m_latitude);
                }

                template <std::size_t D>
                double get() const
                {
                        static_assert(0 <= D && D <= 1, "Dimension should be in range");
                        if (D == 0) {
                                return m_longitude;
                        } else {
                                return m_latitude;
                        }
                }

                bool InBox(const AABB<KdTree2DPoint>& box) const
                {
                        return box.lower.m_longitude <= m_longitude && m_longitude <= box.upper.m_longitude &&
                               box.lower.m_latitude <= m_latitude && m_latitude <= box.upper.m_latitude;
                }

                bool InRadius(const KdTree2DPoint& start, double radius) const { return distance(start) <= radius; }

                std::shared_ptr<Location> getLocation() const { return m_location; }

                template <std::size_t D>
                struct dimension_type
                {
                        using type = double;
                };

        private:
                std::shared_ptr<Location> m_location;
                double                    m_longitude;
                double                    m_latitude;

                double distance(const KdTree2DPoint& other) const
                {
                        double lat1 = degreeToRadian(m_latitude);
                        double lon1 = degreeToRadian(m_longitude);
                        double lat2 = degreeToRadian(other.m_latitude);
                        double lon2 = degreeToRadian(other.m_longitude);

                        return 6371.0 * std::acos(std::sin(lat1) * std::sin(lat2) +
                                                  std::cos(lat1) * std::cos(lat2) * std::cos(lon1 - lon2));
                }
        };

        KdTree<KdTree2DPoint> m_tree;
};

} // namespace gengeopop
