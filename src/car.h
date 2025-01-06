// Car.h
#ifndef CAR_H
#define CAR_H

#include <string>
#include <bits/stdc++.h> // For vector usage

class Car {

  struct {
    double x, y;
  } Vector2D = {0, 0};

public:

    Car(int identifier, double mass, double weight, double horsePower, double torque double tireGripCoefficient, bool isTraction);

    int tick; // TODO Will be moved to another class later
    const float dT;

    // Data
    int identifier;
    double mass;
    double weight;
    double horsePower;
    double torque; // Couple
    std::vector<struct Vector2D> velocity;
    int frontWheelAngle;
    std::vector<struct Vector2D> acceleration;
    double tireGripCoefficient; // Value of grip of the tires (fixed value)
    double actualTireGripCoefficient; // How much the car actually grips (will change with terrain etc)

    double frontRightWheelRotationSpeed;
    double frontLeftWheelRotationSpeed;
    double backRightWheelRotationSpeed;
    double backLeftWheelRotationSpeed;

    std::vector<double> centrifugalForce;

    // Information
    bool isTraction;

    // Methods
    void accelerate(int percentageThrottle);
    void decelerate(int percentageThrottle);
    void turn(int wheelAngle);

};

#endif // CAR_H