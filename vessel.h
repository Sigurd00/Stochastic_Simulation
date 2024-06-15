//
// Created by jskad on 10-06-2024.
//
#pragma once

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "reaction.h"
#include <random>
#include <algorithm>
#include <coro/coro.hpp>
#include <set>
#include <ranges>
#ifndef STOCHASTIC_SIMULATION_VESSEL_H
#define STOCHASTIC_SIMULATION_VESSEL_H

#endif //STOCHASTIC_SIMULATION_VESSEL_H

namespace stochastic {
    // Requirement 3
    template<typename K, typename V>
    class Symbol_table {
    public:
        const K &add(const K &key, const V &value) {
            if (symbols.find(key) != symbols.end()) { //Failure case B from requirement 3
                throw std::runtime_error("Symbol already exists");
            }
            symbols[key] = value;
            return key;
        }

        const V &get(const K &key) const {
            if (!exists(key)) { //Failure case A from requirement 3
                throw std::runtime_error("Symbol not found");
            }
            return symbols.at(key);
        }

        void update(const K &key, const V &new_value) {
            if (!exists(key)) {
                throw std::runtime_error("Symbol not found");
            }
            symbols[key] = new_value;
        }

        bool exists(const K &key) const {
            return symbols.find(key) != symbols.end();
        }

        std::vector<K> get_all_symbols() const {
            std::vector<K> keys;
            keys.reserve(symbols.size());
            for (const auto &pair: symbols) {
                keys.push_back(pair.first);
            }
            return keys;
        }

    private:
        std::unordered_map<K, V> symbols;
    };

    struct TrajectoryPoint {
        double time{};
        Symbol_table<std::string, double> reactants;
    };

    class Vessel {
    public:
        std::vector<Reaction> reactions;
        std::string const name;

        explicit Vessel(std::string name) : name(std::move(name)) {}

        const std::string &add(const std::string &key, double value) {
            return reactants.add(key, value);
        }

        std::string environment() {
            return reactants.add("env", 0);
        }

        void add(const Reaction &reaction) {
            for (const auto &reactant: reaction.inputs) {
                if (!reactants.exists(reactant)) {
                    throw std::runtime_error("Input reactant not found");
                }
            }

            for (const auto &reactant: reaction.products) {
                if (!reactants.exists(reactant)) {
                    throw std::runtime_error("Product reactant not found");
                }
            }

            reactions.push_back(reaction);
        }

        const double &get(const std::string &key) const {
            return reactants.get(key);
        }

        std::vector<std::string> get_all_reactants() const {
            return reactants.get_all_symbols();
        }

        // Requirement 4
        coro::generator<TrajectoryPoint> simulate(const double &end_time) {
            double current_time = 0;

            while (current_time <= end_time) {
                for (Reaction &reaction: reactions) {
                    compute_delay(reaction);
                }
                auto &reaction = *std::min_element(reactions.begin(), reactions.end(),
                                                   [](const auto a, const auto b) { return a.delay < b.delay; });
                current_time += reaction.delay;

                if (std::none_of(reaction.inputs.begin(), reaction.inputs.end(),
                                 [&reactants = reactants](const auto &reactant) {
                                     return reactants.get(reactant) == 0;
                                 })) {
                    for (const auto &reactant: reaction.inputs) {
                        reactants.update(reactant, reactants.get(reactant) - 1);
                    }
                    for (const auto &reactant: reaction.products) {
                        reactants.update(reactant, reactants.get(reactant) + 1);
                    }
                }
                co_yield {current_time, reactants};
            }
        }

    private:
        void compute_delay(Reaction &reaction) const {
            std::random_device rd;
            std::mt19937 gen(rd());

            double input_product = 1;
            for (const auto &input: reaction.inputs) {
                const auto &input_amount = reactants.get(input);

                if (input_amount == 0) {
                    reaction.delay = std::numeric_limits<double>::infinity();
                    return;
                }
                input_product *= input_amount;
            }
            std::exponential_distribution d(reaction.rate * input_product);

            reaction.delay = d(gen);
        }

        Symbol_table<std::string, double> reactants;
    };
}
