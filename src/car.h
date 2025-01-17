// Car.h
#ifndef CAR_H
#define CAR_H
#define GRAVITY 9.81

#include <string>
#include <vector>


namespace Driving_Sim {
    class Car {

        struct {
            double x, y;
        } Vector2D = {0, 0};

    public:

        // Car(int identifier, double mass, double weight, double horsePower, double torque, double tireGripCoefficient, bool isTraction);
        Car(double f, double r);
        Car(double f, double r, double m, double mu);

        // Methods
        void setConstants(double f, double r);
        void setInitialConditions(double x_i, double y_i, double v_i, double psi_i);
        void setInputs(double a, double steer);
        void simulateTrajectory(double t);
        void simulateAdvancedTrajectory(double t);
        void writeToFile(const std::string &fileName) const;

        std::vector<double> positionX;
        std::vector<double> positionY;
        std::vector<double> speed;
        std::vector<double> heading;
        std::vector<double> time;

        double mass;
        double tireFrictionCoefficient;
        double maxTractionForce;

        // int tick; // TODO Will be moved to another class later

        // Data
        int identifier;
        // double mass;
        double weight;
        double horsePower;
        double torque; // Couple


        // std::vector<struct Vector2D> velocity;
        int frontWheelAngle;
        // std::vector<struct Vector2D> acceleration;
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

    private:

        const float dT;
        double frontDistance;
        double rearDistance;
        double slip;
        double acceleration;
        double acceleration_Input;
        double steerAngle_Input;
    };
}

#endif // CAR_H