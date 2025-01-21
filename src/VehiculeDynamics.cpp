//
// Created by keiris on 1/20/25.
//

#include "VehiculeDynamics.h"
#include <cmath>

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

// Structure to hold input-output data
struct IOData {
    Eigen::MatrixXd inputs;
    Eigen::MatrixXd outputs;
    double sampleTime;
};

// Function to load data TODO : (make a placeholder for the actual data-loading logic)
IOData loadVehicleData(const std::string& type) {
    IOData data;
    if (type == "high") {
        // Simulated high tire stiffness data
        data.inputs = Eigen::MatrixXd::Random(100, 5);  // Replace with real data
        data.outputs = Eigen::MatrixXd::Random(100, 3); // Replace with real data
    } else if (type == "low") {
        // Simulated low tire stiffness data
        data.inputs = Eigen::MatrixXd::Random(100, 5);  // Replace with real data
        data.outputs = Eigen::MatrixXd::Random(100, 3); // Replace with real data
    }
    data.sampleTime = 0.1;
    return data;
}

// Function to plot inputs
void plotInputs(const IOData& data, const std::string& title) {
    for (int i = 0; i < data.inputs.cols(); ++i) {
        std::vector<double> time(data.inputs.rows());
        std::vector<double> values(data.inputs.rows());

        for (int j = 0; j < data.inputs.rows(); ++j) {
            time[j] = j * data.sampleTime;
            values[j] = data.inputs(j, i);
        }

        plt::subplot(data.inputs.cols(), 1, i + 1);
        plt::plot(time, values);
        plt::title("Input #" + std::to_string(i + 1));
        plt::tight_layout();
    }
    plt::xlabel("Time (s)");
    plt::show();
}

// Function to plot outputs
void plotOutputs(const IOData& data, const std::string& title) {
    for (int i = 0; i < data.outputs.cols(); ++i) {
        std::vector<double> time(data.outputs.rows());
        std::vector<double> values(data.outputs.rows());

        for (int j = 0; j < data.outputs.rows(); ++j) {
            time[j] = j * data.sampleTime;
            values[j] = data.outputs(j, i);
        }

        plt::subplot(data.outputs.cols(), 1, i + 1);
        plt::plot(time, values);
        plt::title("Output #" + std::to_string(i + 1));
        plt::tight_layout();
    }
    plt::xlabel("Time (s)");
    plt::show();
}


// Main function
// TODO : Rename maine
/*
int maine() {
    matplotlibcpp::subplot(1, 1, 1);
    matplotlibcpp::show();
    return 0;
    // Load high tire stiffness data
    IOData highStiffnessData = loadVehicleData("high");

    // Plot inputs and outputs for high tire stiffness data
    std::cout << "Plotting inputs for high tire stiffness data..." << std::endl;
    plotInputs(highStiffnessData, "High Tire Stiffness Inputs");

    std::cout << "Plotting outputs for high tire stiffness data..." << std::endl;
    plotOutputs(highStiffnessData, "High Tire Stiffness Outputs");

    // Load low tire stiffness data
    IOData lowStiffnessData = loadVehicleData("low");

    // Plot inputs and outputs for low tire stiffness data
    std::cout << "Plotting inputs for low tire stiffness data..." << std::endl;
    plotInputs(lowStiffnessData, "Low Tire Stiffness Inputs");

    std::cout << "Plotting outputs for low tire stiffness data..." << std::endl;
    plotOutputs(lowStiffnessData, "Low Tire Stiffness Outputs");

    // Placeholder for model simulation, parameter estimation, and comparison logic
    // Implement as needed based on your specific requirements

    return 0;
}
*/



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
