
#include <iostream>
#include "demonstration.h"
#include <future>

constexpr auto nr_simulations = 100;
constexpr auto simulation_end_time = 100;
constexpr auto population_count = 50000;
using namespace stochastic;

int main() {
    //stochastic::visualize_example_one();
    //stochastic::visualize_seihr();
    //visualize_circadian_rhythm();

    return 0;

}

void simulate_unoptimized() {
    auto vessels = std::vector<Vessel>{nr_simulations, seihr(population_count)};
    double sum = 0;

    for (auto& vessel : vessels) {
        sum += calculate_hospitalisation_peak(vessel.simulate(simulation_end_time));
    }

    std::cout << "Average peak across " << nr_simulations << " = " << sum / nr_simulations << std::endl;
}

void simulate_optimized() {
    auto vessels = std::vector<VesselOptimized>{nr_simulations, seihr(population_count)};
    double sum = 0;

    for (auto& vessel : vessels) {
        sum += calculate_hospitalisation_peak(vessel.simulate(simulation_end_time));
    }
}

void simulate_parallel_unoptimized() {
    auto vessels = std::vector<Vessel>{nr_simulations, seihr(population_count)};

    auto futures = parallelize_execution_of_multiple_sims<double>(vessels, simulation_end_time, calculate_hospitalisation_peak);

    double sum = 0;
    for (auto& future : futures) {
        sum += future.get();
    }

    std::cout << "Average peak across " << nr_simulations << " = " << sum / nr_simulations << std::endl;
}

void simulate_parallel_optimized() {
    auto vessels = std::vector<VesselOptimized>{nr_simulations, seihr(population_count)};

    auto futures = parallelize_execution_of_multiple_sims<double>(vessels, simulation_end_time, calculate_hospitalisation_peak);

    double sum = 0;
    for (auto& future : futures) {
        sum += future.get();
    }

    std::cout << "Average peak across " << nr_simulations << " = " << sum / nr_simulations << std::endl;
}