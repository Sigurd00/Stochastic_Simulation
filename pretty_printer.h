//
// Created by jskad on 11-06-2024.
//
#ifndef STOCHASTIC_SIMULATION_PRETTY_PRINTER_H
#define STOCHASTIC_SIMULATION_PRETTY_PRINTER_H

#endif //STOCHASTIC_SIMULATION_PRETTY_PRINTER_H

#include "vessel.h"
#include <string>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cstdlib>

namespace stochastic{
    struct Edge {
            std::string from;
            std::string to;
            std::string label;
    };

    class pretty_printer {
    public:
        explicit pretty_printer(const Vessel& vessel) : vessel(vessel) {}
        void generate_network_graph(const std::string& filename) {
            std::ofstream dotFile(filename + ".dot");
            if (!dotFile.is_open()) {
                throw std::runtime_error("Could not open file");
            }

            dotFile << "digraph {\n";

            // Add reactants as nodes
            for(const auto& reactant: vessel.get_all_reactants()) {
                dotFile << "    " << reactant << "[shape=box];\n";
            }

            int i = 0;
            for(const auto& reaction : vessel.reactions){
                dotFile << "    r" << i << "[label=" << reaction.rate << ",shape=oval];\n";
                for(const auto& input : reaction.inputs){
                    dotFile << "    " << input << " -> r" << i << ";\n";
                }
                for(const auto& product : reaction.products){
                    dotFile << "    r" << i << " -> " << product << ";\n";
                }
                i++;
            }

            dotFile << "}\n";
            dotFile.close();
            char command[100];
            sprintf(command, "dot -Tpng %s.dot -o %s.png", filename.c_str(), filename.c_str());
            system(command);
        }

        void print_reactions() {
            for (const auto& reaction : vessel.reactions) {
                std::cout << reaction << "\n";
            }
        }
    private:
        const Vessel& vessel;
    };


}
