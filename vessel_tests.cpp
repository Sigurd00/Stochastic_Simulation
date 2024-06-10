//
// Created by jskad on 10-06-2024.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "vessel.h"
using namespace stochastic;

TEST_CASE("Vessel")
{
    // Create a new vessel to hold reactants and reactions
    auto vessel = stochastic::vessel<std::string, double>{"Test"};

    // Add initial reactants
    const auto a =  vessel.add("a", 1);
    const auto b = vessel.add("b", 0);
    const auto c = vessel.add("c", 2);


    SUBCASE("Initial values for reactants")
    {
        CHECK((vessel.get("a") == 1));
        CHECK((vessel.get("b") == 0));
        CHECK((vessel.get("c") == 2));
    }

    SUBCASE("Getting nonexistent reactant throws error (Case a)")
    {
        CHECK_THROWS_MESSAGE(vessel.get("d"), "Reactant not found");
    }

    SUBCASE("Adding existing reactant throws error (Case b)")
    {
        CHECK_THROWS_MESSAGE(vessel.add("a", 0), "Reactant already exists");
    }

}