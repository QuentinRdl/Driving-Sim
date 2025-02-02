#include <iostream>
#include <vector>
#include <cmath>
#include "gnuplot-iostream.h"

// Tire Model Constants
const double B = 10.0;  // stiffness factor
const double C = 1.9;   // shape factor
const double D = 1.0;   // peak factor
const double E = 0.97;  // curvature factor

// Calculate longitudinal force based on Pacejka "Magic Formula"
double calculate_longitudinal_force(double slip) {
    return D * std::sin(C * std::atan(B * slip - E * (B * slip - std::atan(B * slip))));
}

int main() {
    std::vector<std::pair<double, double>> data_points;

    // Generate data points for plotting
    for (double slip = -0.3; slip <= 0.3; slip += 0.01) {
        double force = calculate_longitudinal_force(slip);
        data_points.emplace_back(slip, force);
    }

    // Plot using gnuplot-iostream
    Gnuplot gp;
    gp << "set xlabel 'Slip Ratio'\n";
    gp << "set ylabel 'Longitudinal Force'\n";
    gp << "set title 'Tire Model Curve'\n";
    gp << "plot '-' with lines title 'Force vs Slip'\n";
    gp.send1d(data_points);

    return 0;
}
