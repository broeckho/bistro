#include "Location.h"

namespace gengeopop {
    Location::Location(unsigned int id, unsigned int province, unsigned int population, Coordinate coordinate, std::string name) : m_id(id), m_province(province), m_coordinate(coordinate), m_name(name), m_population(population) {

    }

    std::string Location::getName(){
        return m_name;
    }

    unsigned int Location::getProvince(){
        return m_province;
    }

    unsigned int Location::getID(){
        return m_id;
    }

    unsigned int Location::getPopulation() {
        return m_population;
    }

    void Location::addContactCenter(std::shared_ptr<ContactCenter> contactCenter) {
        m_contactCenters.push_back(contactCenter);
    }

    const std::vector<std::shared_ptr<ContactCenter> >& Location::getContactCenters() const {
            return m_contactCenters;
    }

    Coordinate &Location::getCoordinate() {
        return m_coordinate;
    }

    LocationIterator Location::begin() {
        return m_contactCenters.begin();
    }

    LocationIterator Location::end() {
        return m_contactCenters.end();
    }

}

