//
// Created by keiris on 1/6/25.
//

#include "car.h"
#include "VehiculeDynamics.h"

#include <thread>

#include "utils.h"
/*
int main() {
    Driving_Sim::Car car1(4.5, 2.5);
    car1.setInputs(1.5, 0);
    car1.setInitialConditions(50, 20, 20, 0);
    double t1 = 5;
    std::string file1 = "car1.txt";

    Driving_Sim::Car car2(2.5, 3.5);
    car2.setInputs(-1, 0.05);
    car2.setInitialConditions(0, 0, 40, 0.5);
    double t2 = 150;
    std::string file2 = "car2.txt";

    std::thread th1(&Driving_Sim::Car::simulateTrajectory, std::ref(car1), std::ref(t1));
    th1.join();
    std::thread th2(&Driving_Sim::Car::simulateTrajectory, std::ref(car2), std::ref(t2));
    th2.join();

    std::thread th3(&Driving_Sim::Car::writeToFile, std::ref(car1), std::ref(file1));
    th3.join();
    std::thread th4(&Driving_Sim::Car::writeToFile, std::ref(car2), std::ref(file2));
    th4.join();
    // Driving_Sim::utils::registerKeys();

    return EXIT_SUCCESS;
}


int main() {
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

