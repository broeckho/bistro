#pragma once

#include "IPartialGenerator.h"
#include "GeoGridGenerator.h"

namespace gengeopop {
   class GeoGridGenerator {
   public:
       GeoGridGenerator(GeoGridConfig geoGridConfig);
       void addPartialGenerator(std::shared_ptr<IPartialGenerator> gen);

       /**
        * @param filename Filename with the config of the wanted geogrid.
        * @return The GeoGrid that was generated by applying all partial generators onto an empty GeoGrid object.
        */
       void generateGeoGrid();

        std::shared_ptr<GeoGrid> getGeoGrid();

   private:
       std::vector<std::shared_ptr<IPartialGenerator> > m_partialGenerators;
       std::shared_ptr<GeoGrid> m_geoGrid; // FIXME unique pointer?
       GeoGridConfig m_geoGridConfig;
    };
}



