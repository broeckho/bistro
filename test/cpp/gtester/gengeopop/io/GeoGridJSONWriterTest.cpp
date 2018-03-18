#include <gengeopop/Community.h>
#include <gengeopop/GeoGridConfig.h>
#include <gengeopop/HighSchool.h>
#include <gengeopop/School.h>
#include <gengeopop/Workplace.h>
#include <gengeopop/generators/GeoGridGenerator.h>
#include <gengeopop/io/GeoGridJSONWriter.h>
#include <gtest/gtest.h>
#include <util/FileSys.h>

using namespace gengeopop;

namespace {

std::shared_ptr<GeoGrid> getGeoGrid()
{
        GeoGridConfig config;
        config.populationSize            = 10000;
        config.fraction_compulsoryPupils = 0.20;

        GeoGridGenerator geoGridGenerator(config);
        return geoGridGenerator.getGeoGrid();
}

bool compareGeoGrid(std::shared_ptr<GeoGrid> geoGrid, std::string testname)
{
        GeoGridJSONWriter writer;
        std::stringstream ss;
        writer.write(geoGrid, ss);

        boost::property_tree::ptree result;
        boost::property_tree::read_json(ss, result);
        boost::property_tree::ptree expected;
        boost::property_tree::read_json(
            stride::util::FileSys::GetTestsDir().string() + "/testdata/GeoGridJSON/" + testname, expected);
        return result == expected;
}

TEST(GeoGridJSONWriterTest, locationTest)
{
        auto geoGrid = getGeoGrid();
        geoGrid->addLocation(std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove"));
        geoGrid->addLocation(std::make_shared<Location>(2, 3, 5000, Coordinate(0, 0, 0, 0), "Gent"));
        geoGrid->addLocation(std::make_shared<Location>(3, 2, 2500, Coordinate(0, 0, 0, 0), "Mons"));

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test0.json"));
}
TEST(GeoGridJSONWriterTest, contactCentersTest)
{
        auto geoGrid  = getGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");
        location->addContactCenter(std::make_shared<School>());
        location->addContactCenter(std::make_shared<Community>());
        location->addContactCenter(std::make_shared<HighSchool>());
        location->addContactCenter(std::make_shared<Household>());
        location->addContactCenter(std::make_shared<Workplace>());
        geoGrid->addLocation(location);

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test1.json"));
}
TEST(GeoGridJSONWriterTest, peopleTest)
{
        auto geoGrid  = getGeoGrid();
        auto location = std::make_shared<Location>(1, 4, 2500, Coordinate(0, 0, 0, 0), "Bavikhove");

        auto school = std::make_shared<School>();
        location->addContactCenter(school);
        auto schoolPool = std::make_shared<ContactPool>(2);
        school->addPool(schoolPool);

        auto community = std::make_shared<Community>();
        location->addContactCenter(community);
        auto communityPool = std::make_shared<ContactPool>(3);
        community->addPool(communityPool);

        auto secondaryCommunity = std::make_shared<Community>();
        location->addContactCenter(secondaryCommunity);
        auto secondaryCommunityPool = std::make_shared<ContactPool>(7);
        secondaryCommunity->addPool(secondaryCommunityPool);

        auto highschool = std::make_shared<HighSchool>();
        location->addContactCenter(highschool);
        auto highschoolPool = std::make_shared<ContactPool>(4);
        highschool->addPool(highschoolPool);

        auto household = std::make_shared<Household>();
        location->addContactCenter(household);
        auto householdPool = std::make_shared<ContactPool>(5);
        household->addPool(householdPool);

        auto workplace = std::make_shared<Workplace>();
        location->addContactCenter(workplace);
        auto workplacePool = std::make_shared<ContactPool>(6);
        workplace->addPool(workplacePool);

        geoGrid->addLocation(location);
        auto person = std::make_shared<stride::Person>(1, 18, 4, 2, 6, 3, 7, 0, 0, 0, 0, 0);
        communityPool->addMember(person);
        schoolPool->addMember(person);
        secondaryCommunityPool->addMember(person);
        highschoolPool->addMember(person);
        householdPool->addMember(person);
        workplacePool->addMember(person);

        EXPECT_TRUE(compareGeoGrid(geoGrid, "test2.json"));
}
} // namespace