//
// Created by jskad on 14-06-2024.
//

#include <iostream>
#include "demonstration.h"
#include <future>
#include <benchmark/benchmark.h>

constexpr auto nr_simulations = 100;
constexpr auto simulation_end_time = 100;
constexpr auto population_count = 1000;

using namespace stochastic;

static void simulate_unoptimized(benchmark::State& state) {
    for (auto _ : state) {
        auto vessels = std::vector<Vessel>{nr_simulations, seihr(population_count)};
        double sum = 0;
        for (auto& vessel : vessels) {
            sum += calculate_hospitalisation_peak(vessel.simulate(simulation_end_time));
        }
        auto average = sum / nr_simulations;
    }
}

static void simulate_optimized(benchmark::State& state) {
    for (auto _ : state) {
        auto vessels = std::vector<Vessel>{nr_simulations, seihr_optimized(population_count)};
        double sum = 0;
        for (auto& vessel : vessels) {
            sum += calculate_hospitalisation_peak(vessel.simulate(simulation_end_time));
        }
        auto average = sum / nr_simulations;
    }
}

static void simulate_parallel_unoptimized(benchmark::State& state) {
    for (auto _ : state) {
        auto vessels = std::vector<Vessel>{nr_simulations, seihr(population_count)};
        auto futures = parallelize_execution_of_multiple_sims<double>(vessels, simulation_end_time, calculate_hospitalisation_peak);
        double sum = 0;
        for (auto& future : futures) {
            sum += future.get();
        }
        auto average = sum / nr_simulations;
    }
}

static void simulate_parallel_optimized(benchmark::State& state) {
    for (auto _ : state) {
        auto vessels = std::vector<Vessel>{nr_simulations, seihr_optimized(population_count)};
        auto futures = parallelize_execution_of_multiple_sims<double>(vessels, simulation_end_time, calculate_hospitalisation_peak);
        double sum = 0;
        for (auto& future : futures) {
            sum += future.get();
        }
        auto average = sum / nr_simulations;
    }
}

BENCHMARK(simulate_unoptimized);
BENCHMARK(simulate_optimized);
BENCHMARK(simulate_parallel_unoptimized);
BENCHMARK(simulate_parallel_optimized);
BENCHMARK_MAIN();
