//
// Created by keiris on 1/20/25.
//

#include "VehiculeDynamics.h"
#include <cmath>

void VehicleDynamics::computeStateDerivatives(const std::vector<double> &state,
                                              const std::vector<double> &inputs,
                                              std::vector<double> &derivatives) const {
    // State variables
    double vx = state[0];
    double vy = state[1];
    double r = state[2];

    // Input variables
    double sFL = inputs[0];
    double sFR = inputs[1];
    double sRL = inputs[2];
    double sRR = inputs[3];
    double delta = inputs[4];

    // Compute derivatives
    derivatives[0] = vy * r + (1 / m) * (Cx * (sFL + sFR) * cos(delta)
                                         - 2 * Cy * (delta - (vy + a * r) / vx) * sin(delta)
                                         + Cx * (sRL + sRR) - CA * pow(vx, 2));
    derivatives[1] = -vx * r + (1 / m) * (Cx * (sFL + sFR) * sin(delta)
                                          + 2 * Cy * (delta - (vy + a * r) / vx) * cos(delta)
                                          + 2 * Cy * (b * r - vy) / vx);
    derivatives[2] = (1 / (pow((a + b) / 2, 2) * m)) * (a * (Cx * (sFL + sFR) * sin(delta)
                                                             + 2 * Cy * (delta - (vy + a * r) / vx) * cos(delta))
                                                        - 2 * b * Cy * (b * r - vy) / vx);
}

void VehicleDynamics::computeOutputs(const std::vector<double> &state,
                                     const std::vector<double> &inputs,
                                     std::vector<double> &outputs) const {
    double vx = state[0];
    double vy = state[1];
    double r = state[2];
    double delta = inputs[4];

    outputs[0] = vx;
    outputs[1] = (1 / m) * (Cx * (inputs[0] + inputs[1]) * sin(delta)
                            + 2 * Cy * (delta - (vy + a * r) / vx) * cos(delta)
                            + 2 * Cy * (b * r - vy) / vx);
    outputs[2] = r;
}

void simulate(VehicleDynamics &vehicle, double dt, double totalTime) {
    std::vector<double> state = {0.0, 0.0, 0.0}; // Initial state
    std::vector<double> inputs = {0.0, 0.0, 0.0, 0.0, 0.1}; // Example inputs
    std::vector<double> derivatives(3);
    std::vector<double> outputs(3);

    for (double t = 0; t < totalTime; t += dt) {
        vehicle.computeStateDerivatives(state, inputs, derivatives);

        // Update state using Euler integration
        for (size_t i = 0; i < state.size(); ++i) {
            state[i] += derivatives[i] * dt;
        }

        vehicle.computeOutputs(state, inputs, outputs);
        // TODO : Print or log the outputs
    }
}
