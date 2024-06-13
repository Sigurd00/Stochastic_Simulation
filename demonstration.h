//
// Created by jskad on 12-06-2024.
//

#ifndef STOCHASTIC_SIMULATION_DEMONSTRATION_H
#define STOCHASTIC_SIMULATION_DEMONSTRATION_H

#endif //STOCHASTIC_SIMULATION_DEMONSTRATION_H
#include "vessel.h"
#include <future>
namespace stochastic {

    Vessel example_one();

    Vessel circadian_rhythm();

    Vessel seihr(uint32_t N);

    void simulate_example_one();

    void demonstrate_lazy_trajectory_support();

    void visualize_example_one();

    void visualize_seihr();

    void visualize_circadian_rhythm();

    void generate_graph(const std::vector<std::vector<double>>& data, const std::vector<std::string>& reactants, std::string title);

    void open_gnuplot(std::string title);

    double calculate_hospitalisation_peak(coro::generator<TrajectoryPoint> simulation);

    template<typename T>
    std::vector<std::future<T>> parallelize_execution_of_multiple_sims(std::vector<Vessel>& vessels, const double& end_time, std::function<T (coro::generator<TrajectoryPoint>)> worker_function) {
        std::vector<std::future<T>> futures;
        futures.reserve(vessels.size());

        for (auto& vessel : vessels) {
            futures.push_back(std::async(worker_function, vessel.simulate(end_time)));
        }

        return futures;
    }
}