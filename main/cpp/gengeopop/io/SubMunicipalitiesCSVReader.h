#pragma once

#include "CitiesReader.h"

namespace gengeopop {
class SubMunicipalitiesCSVReader : public CitiesReader
{
public:
        SubMunicipalitiesCSVReader(std::unique_ptr<std::istream> inputStream);

        void FillGeoGrid(std::shared_ptr<GeoGrid> geoGrid) const override;
};
} // namespace gengeopop
