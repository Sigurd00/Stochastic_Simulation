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

#ifndef STOCHASTIC_SIMULATION_VESSEL_H
#define STOCHASTIC_SIMULATION_VESSEL_H

#endif //STOCHASTIC_SIMULATION_VESSEL_H

namespace stochastic {
    template<typename K, typename V>
    class symbol_table {
    public:
        const K& add(const K& key, const V& value) {
            if (symbols.find(key) != symbols.end()) { //Failure case B from requirement 3
                throw std::runtime_error("Reactant already exists");
            }
            symbols[key] = value;
            return key;
        }
        const V& get(const K& key) const {
            if (!exists(key)) { //Failure case A from requirement 3
                throw std::runtime_error("Reactant not found");
            }
            return symbols.at(key);
        }

        bool exists(const K& key) const {
            return symbols.find(key) != symbols.end();
        }

        std::vector<K> get_all_symbols() const {
            std::vector<K> keys;
            keys.reserve(symbols.size());
            for (const auto& pair : symbols) {
                keys.push_back(pair.first);
            }
            return keys;
        }

    private:
        std::unordered_map<K, V> symbols;
    };

    class vessel
    {
    public:
        std::vector<reaction> reactions;
        explicit vessel(std::string  name) : _name(std::move(name)) {}

        const std::string& add(const std::string& key, double value) {
            return reactants.add(key, value);
        }

        void add(const reaction& reaction) {
            for(const auto& reactant : reaction.inputs) {
                if (!reactants.exists(reactant)) {
                    throw std::runtime_error("Input reactant not found");
                }
            }

            for(const auto& reactant : reaction.products) {
                if (!reactants.exists(reactant)) {
                    throw std::runtime_error("Product reactant not found");
                }
            }

            reactions.push_back(reaction);
        }

        const double& get(const std::string& key) const{
            return reactants.get(key);
        }

        std::vector<std::string> get_all_reactants() const {
            return reactants.get_all_symbols();
        }

    private:
        std::string const _name;
        symbol_table<std::string, double> reactants;
    };
}
