#pragma once

#include "pop/Person.h"
#include <vector>

namespace gengeopop {
class ContactPool
{
public:
        ContactPool(unsigned int id);
        using iterator = std::vector<std::shared_ptr<stride::Person>>::iterator;

        ContactPool();

        unsigned int  getID() const;
        unsigned long getCapacity() const;
        unsigned long getUsedCapacity() const;
        void          addMember(std::shared_ptr<stride::Person> p);

        iterator begin();
        iterator end();

private:
        std::vector<std::shared_ptr<stride::Person>> m_people;
        unsigned long                                m_capacity;
        unsigned int                                 m_id;
};
} // namespace gengeopop
