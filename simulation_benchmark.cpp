//
// Created by jskad on 14-06-2024.
//

#include <iostream>
#include "demonstration.h"
#include <benchmark/benchmark.h>

constexpr auto nr_simulations = 100;
constexpr auto simulation_end_time = 100;
constexpr auto population_count = 10000;

using namespace stochastic;
// Requirement 10
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

BENCHMARK(simulate_unoptimized)->Iterations(100);
BENCHMARK(simulate_parallel_unoptimized)->Iterations(100);
BENCHMARK_MAIN();

// Running the benchmarks with release build yielded the following results

/*
Run on (20 X 3610 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x10)
  L1 Instruction 32 KiB (x10)
  L2 Unified 1280 KiB (x10)
  L3 Unified 25600 KiB (x1)
---------------------------------------------------------------------------------------
Benchmark                                             Time             CPU   Iterations
---------------------------------------------------------------------------------------
simulate_unoptimized/iterations:100          1482601753 ns    261406250 ns          100
simulate_parallel_unoptimized/iterations:100  114774980 ns       312500 ns          100

 Hence a 13x speedup is seen when running the tasks in parallel with a population of 1000

---------------------------------------------------------------------------------------
*/