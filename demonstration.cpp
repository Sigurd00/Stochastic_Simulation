//
// Created by jskad on 12-06-2024.
//

#include "demonstration.h"
#include "vessel.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <future>

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
        auto v = Vessel{"COVID19 SEIHR " + std::to_string(N)};
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

    VesselOptimized seihr_optimized(uint32_t N)
    {
        auto v = VesselOptimized{"COVID19 SEIHR: " + std::to_string(N)};
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
        auto vessel = example_one();
        vessel.simulate(2000);
        std::cout << "A = " << vessel.get("A") << "\n";
        std::cout << "B = " << vessel.get("B") << "\n";
        std::cout << "C = " << vessel.get("C") << "\n";
    }

    double calculate_hospitalisation_peak(coro::generator<TrajectoryPoint> simulation) {
        auto peak = double{0};

        for (const auto& point : simulation) {
            if (point.reactants.get("H") > peak){
                peak = point.reactants.get("H");
            }
        }

        return peak;
    }

    void demonstrate_lazy_trajectory_support(){
        auto seihrDK = seihr(5822763);
        auto seihrNJ = seihr(589755);
        const auto peakDK = calculate_hospitalisation_peak(seihrDK.simulate(100));
        const auto peakNJ = calculate_hospitalisation_peak(seihrNJ.simulate(100));

        std::cout << "PEAK DK = " << peakDK << std::endl;
        std::cout << "PEAK NJ = " << peakNJ << std::endl;
    }

    void visualize_example_one() {
        auto vessel = example_one();
        auto simulation = vessel.simulate(2000);

        const std::vector<std::string> reactants = {"A", "B", "C"};
        std::vector<std::vector<double>> data(reactants.size() + 1);

        // Extract data from simulation
        for (const auto& point : simulation) {
            data[0].push_back(point.time);
            for (size_t i = 0; i < reactants.size(); ++i) {
                data[i + 1].push_back(point.reactants.get(reactants[i]));
            }
        }

        generate_graph(data, reactants, vessel.name);
        open_gnuplot(vessel.name);
    }

    void visualize_seihr() {
        auto vessel = seihr(1000);
        auto end_time = 100;
        auto simulation = vessel.simulate(end_time);

        const std::vector<std::string> reactants = {"S", "E", "I", "H", "R"};
        std::vector<std::vector<double>> data(reactants.size() + 1);

        // Extract data from simulation
        for (const auto& point : simulation) {
            if (point.time > end_time)
                data[0].push_back(end_time);
            else {
                data[0].push_back(point.time);
            }
            for (size_t i = 0; i < reactants.size(); ++i) {
                data[i + 1].push_back(point.reactants.get(reactants[i]));
            }
        }

        generate_graph(data, reactants, vessel.name);
        open_gnuplot(vessel.name);
    }

    void visualize_circadian_rhythm() {
        auto vessel = circadian_rhythm();
        auto simulation = vessel.simulate(100);

        const std::vector<std::string> reactants = {"C", "A", "R"};
        std::vector<std::vector<double>> data(reactants.size() + 1);

        // Extract data from simulation
        for (const auto& point : simulation) {
            data[0].push_back(point.time);
            for (size_t i = 0; i < reactants.size(); ++i) {
                data[i + 1].push_back(point.reactants.get(reactants[i]));
            }
        }

        generate_graph(data, reactants, vessel.name);
        open_gnuplot(vessel.name);
    }

    void generate_graph(const std::vector<std::vector<double>>& data, const std::vector<std::string>& reactants, std::string title) {
        std::replace(title.begin(), title.end(), ' ', '_');
        // Save data in file
        std::ofstream datafile(title + "_data.txt");
        if (!datafile.is_open()) {
            std::cerr << "Error opening data file." << std::endl;
        }

        // Write header
        datafile << "# Time";
        for (const auto& reactant : reactants) {
            datafile << " " << reactant;
        }
        datafile << "\n";

        // Write data
        for (size_t i = 0; i < data[0].size(); ++i) {
            for (const auto& vec : data) {
                datafile << vec[i] << " ";
            }
            datafile << "\n";
        }
        datafile.close();

        //Write script file
        std::ofstream scriptfile(title + ".gp");
        if (!scriptfile.is_open()) {
            std::cerr << "Error creating gnuplot script file." << std::endl;
            return;
        }
        scriptfile << "set title \""+ title +"\"\n";
        scriptfile << "set xlabel \"Time\"\n";
        scriptfile << "set ylabel \"Count\"\n";
        scriptfile << "set xrange [0:" + std::to_string(std::floor(data[0].back())) +"]\n"; //from 0 to last time
        scriptfile << "plot ";
        for (size_t i = 0; i < reactants.size(); ++i) {
            scriptfile << "\"" + title +"_data.txt\" using 1:" << (i + 2) << " with lines title \"" << reactants[i] << "\"";
            if (i < reactants.size() - 1) {
                scriptfile << ", ";
            }
        }
        scriptfile << "\n";
        scriptfile << "pause -1\n";
        scriptfile << "exit\n";
        scriptfile.close();
    }

    void open_gnuplot(std::string title) {
        std::replace(title.begin(), title.end(), ' ', '_');
        std::string command = "gnuplot -persist " + title +".gp";
        if (system(command.c_str()) != 0) {
            std::cerr << "Error executing gnuplot command." << std::endl;
        }
    }


}