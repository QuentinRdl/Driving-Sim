#include "car.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

namespace Driving_Sim {

    Car::Car(double f, double r): dT(0.01) {
        frontDistance = f;
        rearDistance = r;
    }

    Car::Car(double f, double r, double m, double mu): dT(0.01), mass(m), tireFrictionCoefficient(mu) {
        frontDistance = f;
        rearDistance = r;
        // TODO : Check maxTractionForce calculations
        maxTractionForce = mass * GRAVITY * tireFrictionCoefficient; // Weight * adherence coefficient
    }

    void Car::setInitialConditions(double x_i, double y_i, double v_i, double psi_i) {
       positionX.push_back(x_i);
       positionY.push_back(y_i);
       speed.push_back(v_i);
       heading.push_back(psi_i);
       time.push_back(0.0);
    }

    void Car::setInputs(double a, double steer) {
        acceleration_Input = a;
        steerAngle_Input = steer;
    }

    void Car::simulateTrajectory(double t) {
         int numSamples = t/dT;
         for(int i = 0; i < numSamples; i++) {
           // We will use Euler's method to solve (or simulate) differential equation ? // TODO : Check info
           double timeUpdate = time[i] + dT;
           time.push_back(timeUpdate);

           double speedUpdated = speed[i] + dT*(acceleration_Input);
           speed.push_back(speedUpdated);
           double slipAngle = atan(tan(steerAngle_Input) * rearDistance/(rearDistance+frontDistance));

           double headingUpdated = heading[i] + dT * ((speed[i]/rearDistance) * sin (slipAngle));
           heading.push_back(headingUpdated);

           double posXUpdated = positionX[i] + dT * (speed[i] * cos(heading[i] + slipAngle));
           positionX.push_back(posXUpdated);


           double posYUpdated = positionY[i] + dT * (speed[i] * sin(heading[i] + slipAngle));
           positionY.push_back(posYUpdated);

           std::cout << speed[i] << std::endl;
         }
    }

    void Car::simulateAdvancedTrajectory(double t) {
        int numSamples = t / dT;
        for (int i = 0; i < numSamples; i++) {
            // Update time
            double timeUpdate = time[i] + dT;
            time.push_back(timeUpdate);

            // Update speed
            double speedUpdated = speed[i] + dT * (acceleration_Input);
            speed.push_back(speedUpdated);

            // Calculate Slip Angles
            double slipAngleFront = atan(tan(steerAngle_Input) * rearDistance / (rearDistance + frontDistance));
            double slipAngleRear = atan(tan(steerAngle_Input) * frontDistance / (rearDistance + frontDistance));
            // double slipAngle = atan(tan(steerAngle_Input) * rearDistance / (rearDistance + frontDistance));
            double slipAngle = slipAngleFront;

            // Calculate Normal Forces
            double Fz_front = mass * GRAVITY * rearDistance / (rearDistance + frontDistance);
            double Fz_rear = mass * GRAVITY * frontDistance / (rearDistance + frontDistance);


            // Approximation of cornering stiffness based on normal force
            double k_front = 15.0;  // Empirical constant for front tires
            double k_rear = 14.0;   // Empirical constant for rear tires

            // Calculate cornering stiffness
            double C_alpha_front = k_front * Fz_front;  // Front cornering stiffness
            double C_alpha_rear = k_rear * Fz_rear;     // Rear cornering stiffness

            // Calculate Lateral Forces
            double Fy_front = C_alpha_front * slipAngleFront;
            double Fy_rear = C_alpha_rear * slipAngleRear;

            // Check for Grip Limits Understeering, Oversteering
            // TODO : Find MaxFy_rear and front
            double maxFy_front, maxFy_rear = 123;
            if (Fy_front > maxFy_front) {
                printf("The car is currently understeering");
                // Fy_front = maxFy_front;
                // Fy_front *= k_understeer;
            }

            if (Fy_rear > maxFy_rear) {
                printf("The car is currently oversteering");
                // Fy_rear = maxFy_rear;
                // Fy_rear *= k_oversteer;
            }
            double headingUpdated = heading[i] + dT * ((speed[i] / rearDistance) * sin(slipAngle));
            heading.push_back(headingUpdated);

            double posXUpdated = positionX[i] + dT * (speed[i] * cos(heading[i] + slipAngle));
            positionX.push_back(posXUpdated);


            double posYUpdated = positionY[i] + dT * (speed[i] * sin(heading[i] + slipAngle));
            positionY.push_back(posYUpdated);

            std::cout << speed[i] << std::endl;
        }
    }

    void Car::writeToFile(const std::string &fileName) const {
       std::ofstream oFile;
       oFile.open(fileName);
       oFile << "t(s)" << "\t" << "X(m)" << "\t" << "Y(m)" << "\t" << "V(m/s)" << "\t" << "Psi(rad)" << std::endl;
       for(int i = 0; i < time.size(); i++) {
           oFile << std::setprecision(5) << time[i] << "\t" << positionX[i] << "\t" << positionY[i] << "\t" << speed[i] << "\t" << heading[i] << std::endl;
       }
       oFile.close();
    }

}
