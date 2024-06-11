//
// Created by jskad on 10-06-2024.
//

#ifndef STOCHASTIC_SIMULATION_REACTION_H
#define STOCHASTIC_SIMULATION_REACTION_H

#endif //STOCHASTIC_SIMULATION_REACTION_H
#include "string"
#include "vector"
#include <iostream>

namespace stochastic {

    class partialReaction{
    public:
        partialReaction(std::vector<std::string> inputs, double rate) : inputs(std::move(inputs)), rate(rate) {}

        std::vector<std::string> inputs;
        double rate;
    };

    class reaction{
    public:
        reaction(partialReaction partial, std::vector<std::string> products) : inputs(std::move(partial.inputs)), products(std::move(products)), rate(partial.rate) {}

        std::vector<std::string> inputs;
        std::vector<std::string> products;
        double rate;

        friend std::ostream& operator<<(std::ostream& os, const reaction& r); //Give << access to our privates *wink wink*
    };

    // Overload +, >> and >>=
    inline std::vector<std::string> operator+(const std::string& left, const std::string& right) {
        return {left, right};
    }

    inline std::vector<std::string> operator+(std::vector<std::string> left, const std::string& right) {
        left.push_back(right);
        return left;
    }

    inline partialReaction operator>>(const std::vector<std::string>& inputs, double rate) {
        return {inputs, rate};
    }

    inline partialReaction operator>>(const std::string& input, double rate) {
        return {std::vector<std::string>{input}, rate};
    }

    inline reaction operator>>=(const partialReaction& partial, const std::vector<std::string>& products) {
        return {partial, products};
    }

    inline reaction operator>>=(const partialReaction& partial, const std::string& product) {
        return {partial, std::vector<std::string>{product}};
    }

    inline std::ostream& operator<<(std::ostream& os, const reaction& r) {
        for (size_t i = 0; i < r.inputs.size(); ++i) {
            os << r.inputs[i];
            if (i != r.inputs.size() - 1) os << " + ";
        }

        os << " -[" << r.rate << "]-> ";

        for (size_t i = 0; i < r.products.size(); ++i) {
            os << r.products[i];
            if (i != r.products.size() - 1) os << " + ";
        }
         return os;
    }
}
