//
// Created by keiris on 1/18/25.
//

#include <vector>

#ifndef VEHICULEDYNAMICS_H
#define VEHICULEDYNAMICS_H


class VehicleDynamics {
public:
    // Constructor to initialize parameters
    VehicleDynamics(double mass, double distFront, double distRear,
                    double tireStiffLong, double tireStiffLat, double airResCoeff);

    // Method to compute state derivatives
    void computeStateDerivatives(const std::vector<double>& state,
                                  const std::vector<double>& inputs,
                                  std::vector<double>& derivatives) const;

    // Method to compute outputs
    void computeOutputs(const std::vector<double>& state,
                        const std::vector<double>& inputs,
                        std::vector<double>& outputs) const;

private:
    double m;  // Vehicle mass [kg]
    double a;  // Distance from front axle to COG [m]
    double b;  // Distance from rear axle to COG [m]
    double Cx; // Longitudinal tire stiffness [N]
    double Cy; // Lateral tire stiffness [N/rad]
    double CA; // Air resistance coefficient [1/m]
};




#endif //VEHICULEDYNAMICS_H
