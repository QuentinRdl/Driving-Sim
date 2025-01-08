//
// Created by keiris on 1/6/25.
//

#include "car.h"

#include <thread>

#include "utils.h"

int main() {
    Driving_Sim::Car car1(4.5, 2.5);
    car1.setInputs(1.5, 0);
    car1.setInitialConditions(50, 20, 20, 0);
    double t1 = 0.1;
    std::string file1 = "car1.txt";

    Driving_Sim::Car car2(2.5, 3.5);
    car2.setInputs(-1, 0.05);
    car2.setInitialConditions(0, 0, 40, 0.5);
    double t2 = 0.15;
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
