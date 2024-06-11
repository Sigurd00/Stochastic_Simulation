cmake_minimum_required(VERSION 3.28)
project(Stochastic_Simulation)

set(CMAKE_CXX_STANDARD 20)

# Add the directory where doctest.h is located
include_directories(${CMAKE_SOURCE_DIR})

# Add the main executable
add_executable(Stochastic_Simulation
        main.cpp
        vessel.cpp
        vessel.h
        reaction.cpp
        reaction.h
        pretty_printer.cpp
        pretty_printer.h
)

# Add the test executable
add_executable(Stochastic_Simulation_Tests
        vessel.cpp
        vessel.h
        reaction.cpp
        reaction.h
        vessel_tests.cpp
        reaction_tests.cpp
)

# Link the test executable with Doctest
target_include_directories(Stochastic_Simulation_Tests PRIVATE ${CMAKE_SOURCE_DIR})

# Enable testing
enable_testing()

# Add a test for CTest to run
add_test(NAME StochasticSimulationTests COMMAND Stochastic_Simulation_Tests)
