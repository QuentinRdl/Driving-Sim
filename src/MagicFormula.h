
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <utility>
#include "gnuplot-iostream.h"

// Structure to hold road surface parameters
struct RoadSurface {
    std::string name;
    double B;  // Stiffness factor
    double C;  // Shape factor
    double D;  // Peak factor
    double E;  // Curvature factor
};

// Function to calculate longitudinal force using the constant-parameter Magic Formula
double computeLongitudinalForce(double Fz, double slip, const RoadSurface &surface) {
    // Pacejka Magic Formula: Fx = Fz * D * sin(C * atan(B * slip - E*(B*slip - atan(B*slip))))
    return Fz * surface.D * std::sin( surface.C * std::atan( surface.B * slip - surface.E * (surface.B * slip - std::atan(surface.B * slip) ) ) );
}

int main() {
    // Vehicle parameters
    double mv = 2000.0; // Vehicle mass in kg
    double g = 9.81;    // Gravitational acceleration in m/s^2
    double Fz = (mv * g) / 4.0;  // Vertical load per tire (assumes equal load distribution)

    // Define different road surfaces with constant parameters
    std::vector<RoadSurface> surfaces = {
        {"Dry Tarmac", 10.0, 1.9, 1.0, 0.97},
        {"Wet Tarmac", 12.0, 2.3, 0.82, 1.0},
        {"Snow",       5.0, 2.0, 0.3, 1.0},
        {"Ice",        4.0, 2.0, 0.1, 1.0}
    };

    // Generate a series of slip values (from -1 to 1)
    std::vector<double> slip_values;
    for (double slip = -1.0; slip <= 1.0; slip += 0.01) {
        slip_values.push_back(slip);
    }

    // Compute tire longitudinal force for each surface and each slip value
    std::vector< std::vector< std::pair<double, double> > > plot_data;
    for (const auto &surface : surfaces) {
        std::vector<std::pair<double, double>> data;
        for (double slip : slip_values) {
            double Fx = computeLongitudinalForce(Fz, slip, surface);
            data.emplace_back(slip, Fx);
        }
        plot_data.push_back(data);
    }

    // Plot using gnuplot-iostream
    Gnuplot gp;
    gp << "set xlabel 'Wheel slip [-]'\n";
    gp << "set ylabel 'Tire longitudinal force, Fx [N]'\n";
    gp << "set title 'Tire longitudinal force - constant coefficients'\n";
    gp << "set grid\n";

    // Build plot command to include all surfaces with different titles
    gp << "plot ";
    for (size_t i = 0; i < surfaces.size(); ++i) {
        gp << "'-' with lines lw 2 title '" << surfaces[i].name << "'";
        if (i != surfaces.size() - 1)
            gp << ", ";
    }
    gp << "\n";

    // Send each data series to gnuplot
    for (const auto &data : plot_data) {
        gp.send1d(data);
    }

    // Keep the plot window open until user input
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    std::cin.get();

    return 0;
}
