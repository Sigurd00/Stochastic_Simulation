//
// Created by jskad on 10-06-2024.
//
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <stdexcept>

#ifndef STOCHASTIC_SIMULATION_VESSEL_H
#define STOCHASTIC_SIMULATION_VESSEL_H

#endif //STOCHASTIC_SIMULATION_VESSEL_H

namespace stochastic {
    template<typename K, typename V>
    class vessel
    {
    public:
        explicit vessel(std::string  name) : _name(std::move(name)) {}

        const V& add(const K& key, const V& value) {
            if (reactants.find(key) != reactants.end()) { //Failure case B from requirement 3
                throw std::runtime_error("Reactant already exists");
            }
            reactants[key] = value;
            return value;
        }

        const V& get(const K& key) const {
            if (!exists(key)) { //Failure case A from requirement 3
                throw std::runtime_error("Reactant not found");
            }
            return reactants.at(key);
        }

        bool exists(const K& key) const {
            return reactants.find(key) != reactants.end();
        }
    private:
        std::string const _name;
        std::unordered_map<K, V> reactants;
    };
}
