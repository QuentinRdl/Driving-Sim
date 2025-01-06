//
// Created by keiris on 1/6/25.
//

#include "car.h"

int main() {
    Driving_Sim::Car car1(4.5, 2.5);
    car1.setInputs(1.5, 0);
    car1.setInitialConditions(50, 20, 20, 0);
    double t1 = 0.03;
    std::string file1 = "car1.txt";

    Driving_Sim::Car car2(4.5, 2.5);
    car2.setInputs(-1, 0.05);
    car2.setInitialConditions(0,0, 40, 0.05);
    double t2 = 0.04;
    std::string file2 = "car2.txt";

    car1.simulateTrajectory(t1);
    car2.simulateTrajectory(t2);
    // Car::simulateTrajectory,ref(car1),ref(t1);
    return 0;
}