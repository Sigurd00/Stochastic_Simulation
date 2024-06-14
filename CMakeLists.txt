cmake_minimum_required(VERSION 3.28)
project(Stochastic_Simulation)

set(CMAKE_CXX_STANDARD 20)

include(libcoro.cmake)
include(benchmark.cmake)
add_subdirectory(benchmark)
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
        demonstration.cpp
        demonstration.h
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

# Add the test executable
add_executable(Stochastic_Simulation_Benchmarks
        vessel.cpp
        vessel.h
        reaction.cpp
        reaction.h
        demonstration.cpp
        demonstration.h
        simulation_benchmark.cpp
)

target_link_libraries(Stochastic_Simulation PRIVATE libcoro)
target_link_libraries(Stochastic_Simulation_Benchmarks PRIVATE libcoro
        benchmark::benchmark)
target_link_libraries(Stochastic_Simulation_Tests PRIVATE libcoro)
# Link the test executable with Doctest
target_include_directories(Stochastic_Simulation_Tests PRIVATE ${CMAKE_SOURCE_DIR})

# Enable testing
enable_testing()

# Add a test for CTest to run
add_test(NAME StochasticSimulationTests COMMAND Stochastic_Simulation_Tests)

