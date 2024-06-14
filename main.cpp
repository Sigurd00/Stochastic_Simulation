
#include <iostream>
#include "demonstration.h"
#include "pretty_printer.h"
using namespace stochastic;

int main() {
    // Demonstration of 2
    auto example_vessel = example_one();
    auto pp_example_one = pretty_printer(example_vessel);
    pp_example_one.print_reactions();
    pp_example_one.generate_network_graph("example_one");

    // A + C -[0.001]-> B + C

    auto seihr_vessel = seihr(1000);
    auto seihr_pp = pretty_printer(seihr_vessel);
    seihr_pp.print_reactions();
    seihr_pp.generate_network_graph("seihr");

    /*
    S + I -[0.000774194]-> E + I
    E -[0.196078]-> I
    I -[0.322581]-> R
    I -[0.000290061]-> H
    H -[0.0988142]-> R
     */

    auto circadian_vessel = circadian_rhythm();
    auto circadian_rhythm_pp = pretty_printer(circadian_vessel);
    circadian_rhythm_pp.print_reactions();
    circadian_rhythm_pp.generate_network_graph("circadian_rhytm");

    /*
    A + DA -[1]-> D_A
    D_A -[50]-> DA + A
    A + DR -[1]-> D_R
    D_R -[100]-> DR + A
    D_A -[500]-> MA + D_A
    DA -[50]-> MA + DA
    D_R -[50]-> MR + D_R
    DR -[0.01]-> MR + DR
    MA -[50]-> MA + A
    MR -[5]-> MR + R
    A + R -[2]-> C
    C -[1]-> R
    A -[1]-> env
    R -[0.2]-> env
    MA -[10]-> env
    MR -[0.5]-> env
     */

    // Demonstration of requirement 5-6
    //visualize_example_one();
    visualize_seihr();
    //visualize_circadian_rhythm();

    // Demonstration of 7
    //demonstrate_lazy_trajectory_support();
    // Peak for NJ = 127 and Peak for DK = 1195

    // Demonstration of 8
    auto vessels = std::vector<Vessel>{100, seihr(500'000)};
    auto futures = parallelize_execution_of_multiple_sims<double>(vessels, 100, calculate_hospitalisation_peak);
    double sum = 0;
    for (auto& future : futures) {
        sum += future.get();
    }
    std::cout << "Average peak across 100 simulations = " << sum / 100 << std::endl;
    // Average peak across  100 simulations = 110.21

    return 0;
}