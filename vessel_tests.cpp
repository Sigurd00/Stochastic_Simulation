//
// Created by jskad on 10-06-2024.
//
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "vessel.h"
#include "pretty_printer.h"
#include <fstream>

using namespace stochastic;

TEST_CASE("Vessel")
{
    // Create a new vessel to hold reactants and reactions
    auto vessel = stochastic::vessel{"Test"};

    // Add initial reactants
    const auto a =  vessel.add("a", 1);
    const auto b = vessel.add("b", 0);
    const auto c = vessel.add("c", 2);

    //Create pretty printer
    auto pp = pretty_printer(vessel);


    SUBCASE("Initial values for reactants")
    {
        CHECK((vessel.get("a") == 1));
        CHECK((vessel.get("b") == 0));
        CHECK((vessel.get("c") == 2));
    }

    SUBCASE("Getting nonexistent reactant throws error (Case a)")
    {
        CHECK_THROWS(vessel.get("d"));
    }

    SUBCASE("Adding existing reactant throws error (Case b)")
    {
        CHECK_THROWS(vessel.add("a", 0));
    }

    SUBCASE("Adding reactions to vessel")
    {
        vessel.add(a >> 10.0 >>= b);
        auto r1 = vessel.reactions[0];
        CHECK((r1.inputs[0] == "a"));
        CHECK((r1.rate == 10.0));
        CHECK((r1.products[0] == "b"));
    }

    SUBCASE("Adding reaction with unknown reactant throws error")
    {
        const std::string d = "d";
        CHECK_THROWS(vessel.add(d >> 10 >>= a));
        CHECK_THROWS(vessel.add(a >> 10 >>= d));
    }

    SUBCASE("Printing a single reaction in human readable format")
    {
        vessel.add(a >> 10.0 >>= b);
        auto oss = std::ostringstream();
        auto r1 = vessel.reactions[0];
        oss << r1;
        CHECK(oss.str() == "a -[10]-> b");
    }


    SUBCASE("Printing reaction network in human readable format")
    {
        vessel.add(a >> 10.0 >>= b);
        vessel.add(a + b >> 0.5 >>= b + c);

        auto oss = std::ostringstream();
        std::streambuf* old_cout_buf = std::cout.rdbuf(oss.rdbuf());

        pp.print_reactions();

        std::cout.rdbuf(old_cout_buf);

        CHECK(oss.str() == "a -[10]-> b\na + b -[0.5]-> b + c\n");
    }

    SUBCASE("Printing reaction network as DOT graph")
    {
        vessel.add(a >> 10.0 >>= b);
        vessel.add(a + b >> 0.5 >>= b + c);
        pp.generate_network_graph("test_graph");

        std::ifstream dotFile("test_graph.dot");

        if (!dotFile.is_open()) {
            std::cerr << "Error opening the file!" << std::endl;
            CHECK(FALSE);
        }

        std::string content((std::istreambuf_iterator<char>(dotFile)), std::istreambuf_iterator<char>());
        auto expected = "digraph {\n"
                   "    c[shape=box];\n"
                   "    b[shape=box];\n"
                   "    a[shape=box];\n"
                   "    r0[label=10,shape=oval];\n"
                   "    a -> r0;\n"
                   "    r0 -> b;\n"
                   "    r1[label=0.5,shape=oval];\n"
                   "    a -> r1;\n"
                   "    b -> r1;\n"
                   "    r1 -> b;\n"
                   "    r1 -> c;\n"
                   "}\n";
        CHECK((content == expected));
        dotFile.close();
    }
}