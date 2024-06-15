//
// Created by jskad on 10-06-2024.
//
#include "doctest.h"
#include "reaction.h"

using namespace stochastic;
// Requirement 9
TEST_CASE("Reaction")
{
    std::string A = "a";
    std::string C = "c";
    std::string B = "b";
    SUBCASE("Reactants")
    {
        auto r1 = A + B;
        CHECK((r1[0] == "a"));
        CHECK((r1[1] == "b"));

        auto r2 = A + B + C;
        CHECK((r2[0] == "a"));
        CHECK((r2[1] == "b"));
        CHECK((r2[2] == "c"));

    }

    SUBCASE("Reactions")
    {
        Reaction r3 = ((A + C) >> 20.0 >>= B + C);
        CHECK((r3.inputs[0] == "a"));
        CHECK((r3.inputs[1] == "c"));
        CHECK((r3.rate == 20.0));
        CHECK((r3.products[0] == "b"));
        CHECK((r3.products[1] == "c"));

        Reaction r4 = ((A) >> 10.0 >>= B);
        CHECK((r4.inputs[0] == "a"));
        CHECK((r4.rate == 10.0));
        CHECK((r4.products[0] == "b"));
    }
}
