//
// Created by jskad on 10-06-2024.
//

#ifndef STOCHASTIC_SIMULATION_REACTION_H
#define STOCHASTIC_SIMULATION_REACTION_H

#endif //STOCHASTIC_SIMULATION_REACTION_H
#include "string"

namespace stochastic {
    class reaction{
    public:
        reaction(std::string left, double rate) : _left(std::move(left)), _rate(rate) {}
        reaction(std::string left, std::string right, double rate) : _left(std::move(left)), _right(std::move(right)), _rate(rate) {}

        std::string _left;
        std::string _right;
        double _rate;
    };

    // Have to overload +, >> and >>=

    inline reaction operator>>(const std::string& left, double rate) {
        return {left, rate};
    }

    inline reaction operator>>=(const reaction& reaction, const std::string& right) {
        return {reaction._left, right, reaction._rate};
    }
}
