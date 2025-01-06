#include "surface.h"

#include <iostream>
#include <utility>

namespace Driving_Sim {
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


/**
 *
 *Temp Testing.
 *
 * @return 0
 */
/*int main() {

    const Driving_Sim::surface asphalte("Asphalte", 0.85);
    const Driving_Sim::surface sable("Sable", 0.6);
    const Driving_Sim::surface verglas("Verglas", 0.1);


    for (const Driving_Sim::surface &surface_ : {asphalte, sable, verglas}) {
        surface_.display();

        constexpr double mass = 1500.0;
        constexpr double g = 9.81;
        const double traction_max = surface_.getFrictionCoeff() * mass * g;

        // P_g = m*g (N)
        std::cout << "Force de traction max sur " << surface_.getName() << ": " << traction_max << "N" << std::endl << std::endl << std::endl;
    }


    return 0;
}*/