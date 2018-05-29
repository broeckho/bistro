#include "Community.h"
#include "GeoGridConfig.h"
#include <util/Exception.h>

using namespace stride::ContactPoolType;

namespace gengeopop {

Community::Community(unsigned int id) : ContactCenter(id) {}

std::string  Community::GetType() const { return "Community"; }
unsigned int Community::GetPoolSize() const { return 2000; }
unsigned int Community::GetMaxPools() const { return 1; }

void Community::AddHouseHold(std::shared_ptr<Household> household)
{
        if (m_pools.empty()) {
                throw stride::util::Exception("Could not add the Household to the Community, no ContactPool available");
        } else {
                if (household->GetPools().empty())
                        return;
                for (stride::Person* person : **household->begin()) {
                        m_pools[0]->AddMember(person);
                }
        }
}
} // namespace gengeopop
