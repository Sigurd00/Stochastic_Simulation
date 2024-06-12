//
// Created by jskad on 12-06-2024.
//

#ifndef STOCHASTIC_SIMULATION_DEMONSTRATION_H
#define STOCHASTIC_SIMULATION_DEMONSTRATION_H

#endif //STOCHASTIC_SIMULATION_DEMONSTRATION_H
#include "vessel.h"

namespace stochastic {

    Vessel example_one();

    Vessel circadian_rhythm();

    Vessel seihr(uint32_t N);

    void simulate_example_one();

    void demonstrate_lazy_trajectory_support();
    void visualize_example_one();
    void visualize_seihr();
    void visualize_circadian_rhytm();
}