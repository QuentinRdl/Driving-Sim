#include "surface.h"

#include <iostream>
#include <utility>

namespace std {
    surface::surface(std::string name, const double frictionCoeff) : name_(std::move(name)), frictionCoeff_(frictionCoeff) {}

    std::string surface::getName() const {
        return name_;
    }

    double surface::getFrictionCoeff() const {
        return frictionCoeff_;
    }

    void surface::display() const {
        std::cout << "Surface: " << name_ << ", frictionCoeff: " << frictionCoeff_ << std::endl;
    }

} // std