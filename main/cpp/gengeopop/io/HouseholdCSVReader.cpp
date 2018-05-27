#include "HouseholdCSVReader.h"
#include <util/CSV.h>

gengeopop::HouseholdCSVReader::HouseholdCSVReader(std::unique_ptr<std::istream> inputStream) : m_persons()
{
        stride::util::CSV reader(*(inputStream.get()));

        unsigned int id = 1;

        for (const stride::util::CSVRow& row : reader) {
                std::shared_ptr<gengeopop::Household> household = std::make_shared<gengeopop::Household>();

                // Create contactpool of the household
                m_contactPools.emplace_back(id++, stride::ContactPoolType::Id::Household);
                stride::ContactPool* newCP = &m_contactPools.back();

                for (std::size_t i = 0; i < 12; i++) {
                        unsigned int age;
                        try {
                                age = row.GetValue<unsigned int>(i);
                        } catch (const std::bad_cast& e) {
                                // NA
                                break;
                        }

                        if (age < 18 && age >= 6) {
                                m_totalCompulsory++;
                        }

                        if (age >= 18 && age < 26) {
                                m_total1826Years++;
                        }

                        if (age >= 18 && age < 65) {
                                m_total1865Years++;
                        }

                        m_total++;

                        stride::Person p;
                        m_persons.push_back(p);

                        stride::Person* p_ptr = &m_persons.back();
                        p_ptr->SetAge(age);
                        newCP->AddMember(p_ptr);
                }
                household->AddPool(newCP);
                m_households.push_back(household);
        }
}
