#include "car.h"

Car::Car(double f, double r): dT(0.01) {
  setConstants(f, r);
}

void Car::setConstants(double f, double r) {
  frontDistance = f;
  rearDistance = r;
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


       double posYUpdated = positionY[i] + dT * (speed[i] * cos(heading[i] + slipAngle));
       positionY.push_back(posYUpdated);

       std::cout << "\nSimulate trajectory : speed = " << speed[i] << "\n";
     }

}
void Car::writeToFile(std::string fileName) {
   std::ofstream oFile;
   oFile.open(fileName);
   oFile << "t(s)" << "\t" << "X(m)" << "\t" << "Y(m)" << "\t" << "V(m/s)" << "\t" << "Psi(rad)" << std::endl;
   for(int i = 0; i < time.size(); i++) {
       oFile << std::setprecision(5) << time[i] << "\t" << positionX[i] << "\t" << positionY[i] << "\t" << speed[i] << "\t" << heading[i] << std::endl;
   }
   oFile.close();

}
