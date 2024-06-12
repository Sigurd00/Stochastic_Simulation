//
// Created by jskad on 12-06-2024.
//

#include "demonstration.h"
#include "vessel.h"
#include <vector>
#include <matplotlibcpp.h>
namespace plt = matplotlibcpp;

namespace stochastic{
    Vessel example_one() {
        auto vessel = stochastic::Vessel{"Basic example"};
        const auto A = vessel.add("A", 100);
        const auto B = vessel.add("B", 0);
        const auto C = vessel.add("C", 1);
        vessel.add((A + C) >> 0.001 >>= (B + C));
        return vessel;
    }

    Vessel circadian_rhythm() {
        const auto alphaA = 50;
        const auto alpha_A = 500;
        const auto alphaR = 0.01;
        const auto alpha_R = 50;
        const auto betaA = 50;
        const auto betaR = 5;
        const auto gammaA = 1;
        const auto gammaR = 1;
        const auto gammaC = 2;
        const auto deltaA = 1;
        const auto deltaR = 0.2;
        const auto deltaMA = 10;
        const auto deltaMR = 0.5;
        const auto thetaA = 50;
        const auto thetaR = 100;
        auto v = stochastic::Vessel{"Circadian Rhythm"};
        const auto env = v.environment();
        const auto DA = v.add("DA", 1);
        const auto D_A = v.add("D_A", 0);
        const auto DR = v.add("DR", 1);
        const auto D_R = v.add("D_R", 0);
        const auto MA = v.add("MA", 0);
        const auto MR = v.add("MR", 0);
        const auto A = v.add("A", 0);
        const auto R = v.add("R", 0);
        const auto C = v.add("C", 0);
        v.add((A + DA) >> gammaA >>= D_A);
        v.add(D_A >> thetaA >>= DA + A);
        v.add((A + DR) >> gammaR >>= D_R);
        v.add(D_R >> thetaR >>= DR + A);
        v.add(D_A >> alpha_A >>= MA + D_A);
        v.add(DA >> alphaA >>= MA + DA);
        v.add(D_R >> alpha_R >>= MR + D_R);
        v.add(DR >> alphaR >>= MR + DR);
        v.add(MA >> betaA >>= MA + A);
        v.add(MR >> betaR >>= MR + R);
        v.add((A + R) >> gammaC >>= C);
        v.add(C >> deltaA >>= R);
        v.add(A >> deltaA >>= env);
        v.add(R >> deltaR >>= env);
        v.add(MA >> deltaMA >>= env);
        v.add(MR >> deltaMR >>= env);
        return v;
    }

    Vessel seihr(uint32_t N)
    {
        auto v = Vessel{"COVID19 SEIHR: " + std::to_string(N)};
        const auto eps = 0.0009; // initial fraction of infectious
        const auto I0 = size_t(std::round(eps * N)); // initial infectious
        const auto E0 = size_t(std::round(eps * N * 15)); // initial exposed
        const auto S0 = N - I0 - E0; // initial susceptible
        const auto R0 = 2.4; // initial basic reproductive number
        const auto alpha = 1.0 / 5.1; // incubation rate (E -> I) ~5.1 days
        const auto gamma = 1.0 / 3.1; // recovery rate (I -> R) ~3.1 days
        const auto beta = R0 * gamma; // infection/generation rate (S+I -> E+I)
        const auto P_H = 0.9e-3; // probability of hospitalization
        const auto kappa = gamma * P_H * (1.0 - P_H); // hospitalization rate (I -> H)
        const auto tau = 1.0 / 10.12; // removal rate in hospital (H -> R) ~10.12 days
        const auto S = v.add("S", S0); // susceptible
        const auto E = v.add("E", E0); // exposed
        const auto I = v.add("I", I0); // infectious
        const auto H = v.add("H", 0); // hospitalized
        const auto R = v.add("R", 0); // removed/immune (recovered + dead)
        v.add((S + I) >> beta / N >>= E + I); // susceptible becomes exposed by infectious
        v.add(E >> alpha >>= I); // exposed becomes infectious
        v.add(I >> gamma >>= R); // infectious becomes removed
        v.add(I >> kappa >>= H); // infectious becomes hospitalized
        v.add(H >> tau >>= R); // hospitalized becomes removed
        return v;
    }
    void simulate_example_one() {
        Vessel vessel = example_one();
        vessel.simulate(2000);
        std::cout << "A = " << vessel.get("A") << "\n";
        std::cout << "B = " << vessel.get("B") << "\n";
        std::cout << "C = " << vessel.get("C") << "\n";
    }

    void demonstrate_lazy_trajectory_support(){
        Vessel seihrDK = seihr(5822763);
        Vessel seihrNJ = seihr(589755);
        auto simulationDK = seihrDK.simulate(100);
        auto simulationNJ = seihrNJ.simulate(100);

        double peakDK = 0;
        double peakNJ = 0;

        for (const auto& point : simulationDK) {
            if (point.reactants.get("H") > peakDK){
                peakDK = point.reactants.get("H");
                std::cout << peakDK << std::endl;
            }
        }
        std::cout << "ACTUAL PEAK DK = " << peakDK << std::endl;
        for (const auto& point : simulationNJ) {
            if (point.reactants.get("H") > peakNJ){
                peakNJ = point.reactants.get("H");
                std::cout << peakNJ << std::endl;
            }
        }

        std::cout << "ACTUAL PEAK NJ = " << peakNJ << std::endl;
    }

    void visualize_example_one() {
        Vessel vessel = example_one();
        auto simulation = vessel.simulate(2000);

        plt::figure_size(800, 600);
        plt::xlabel("Time");
        plt::ylabel("Count");
        std::vector<double> time = {};
        std::vector<double> A_values = {};
        std::vector<double> B_values = {};
        std::vector<double> C_values = {};

        for (const auto& point : simulation) {
            time.push_back(point.time);
            A_values.push_back(point.reactants.get("A"));
            B_values.push_back(point.reactants.get("B"));
            C_values.push_back(point.reactants.get("C"));
        }

        plt::plot(time, A_values, {{"label", "A"}});
        plt::plot(time, B_values, {{"label", "B"}});
        plt::plot(time, C_values, {{"label", "C"}});
        plt::legend();
        plt::save("example_one.png");
    }

    void visualize_seihr() {
        Vessel vessel = seihr(1000);
        auto simulation = vessel.simulate(100);

        plt::figure_size(800, 600);
        plt::xlabel("Time");
        plt::ylabel("Count");
        std::vector<double> time = {};
        std::vector<double> S_values = {};
        std::vector<double> E_values = {};
        std::vector<double> I_values = {};
        std::vector<double> H_values = {};
        std::vector<double> R_values = {};

        for (const auto& point : simulation) {
            time.push_back(point.time);
            S_values.push_back(point.reactants.get("S"));
            E_values.push_back(point.reactants.get("E"));
            I_values.push_back(point.reactants.get("I"));
            H_values.push_back(point.reactants.get("H") * 1000);
            R_values.push_back(point.reactants.get("R"));
        }

        plt::plot(time, S_values, {{"label", "S"}});
        plt::plot(time, E_values, {{"label", "E"}});
        plt::plot(time, I_values, {{"label", "I"}});
        plt::plot(time, H_values, {{"label", "H * 1000"}});
        plt::plot(time, R_values, {{"label", "R"}});
        plt::legend();
        plt::save("seihr.png");
    }

    void visualize_circadian_rhytm() {
        Vessel vessel = circadian_rhythm();
        auto simulation = vessel.simulate(100);

        plt::figure_size(800, 600);
        plt::xlabel("Time");
        plt::ylabel("Count");
        std::vector<double> time = {};
        std::vector<double> C_values = {};
        std::vector<double> A_values = {};
        std::vector<double> R_values = {};


        for (const auto& point : simulation) {
            time.push_back(point.time);
            C_values.push_back(point.reactants.get("C"));
            A_values.push_back(point.reactants.get("A"));
            R_values.push_back(point.reactants.get("R"));

        }

        plt::plot(time, C_values, {{"label", "S"}});
        plt::plot(time, A_values, {{"label", "E"}});
        plt::plot(time, R_values, {{"label", "I"}});
        plt::legend();
        plt::save("circadian_rhytm.png");
    }
}