//
// Created by keiris on 22/02/25.
//
#include <iostream>
#include <cmath>
#include "gnuplot-iostream.h"

#include <iostream>
#include <vector>
#include <utility>
#include "gnuplot-iostream.h"
#include <cmath>

// Exemple de classe Vehicle (cf. implémentation précédente)
class Vehicle {
public:
    double m;   // Masse [kg]
    double a;   // Distance COG - essieu avant [m]
    double b;   // Distance COG - essieu arrière [m]
    double CA;  // Coefficient de résistance de l'air
    double vx;  // Vitesse longitudinale [m/s]
    double vy;  // Vitesse latérale [m/s]
    double r;   // Taux de lacet (vitesse angulaire) [rad/s]
    double I;   // Moment d'inertie effectif pour le lacet

    Vehicle(double mass, double a_front, double b_rear, double airRes = 0.0)
      : m(mass), a(a_front), b(b_rear), CA(airRes), vx(0.0), vy(0.0), r(0.0)
    {
        I = m * std::pow(0.5 * (a + b), 2);
    }

    // Loi de Newton pour la translation
    void computeTranslation(const double Fx, const double Fy, double& ax, double& ay) {
        ax = Fx / m;
        ay = Fy / m;
    }

    // Loi de Newton pour la rotation (lacet)
    double computeYawAcceleration(const double torque) {
        return torque / I;
    }

    // Mise à jour des états par intégration d'Euler
    void update(double dt, double Fx, double Fy, double torque) {
        double ax, ay;
        computeTranslation(Fx, Fy, ax, ay);
        double r_dot = computeYawAcceleration(torque);
        vx += ax * dt;
        vy += ay * dt;
        r  += r_dot * dt;
    }
};

int main() {
    // Initialisation du véhicule (m=1700 kg, a=1.5 m, b=1.5 m, CA=0.5)
    Vehicle myVehicle(1700.0, 1.5, 1.5, 0.5);

    // Paramètres de simulation
    double Fx = 500.0;     // Force longitudinale (N)
    double Fy = 200.0;     // Force latérale (N)
    double torque = -150.0; // Moment de lacet (Nm)
    double dt = 0.1;       // Pas de temps (s)
    int steps = 100;       // Simulation sur 10 secondes

    // Vecteurs pour stocker les données : paire (temps, valeur)
    std::vector<std::pair<double, double>> vx_data, vy_data, r_data;

    // Simulation : enregistrement des états à chaque pas de temps
    for (int i = 0; i <= steps; ++i) {
        double t = i * dt;
        vx_data.push_back({t, myVehicle.vx});
        vy_data.push_back({t, myVehicle.vy});
        r_data.push_back({t, myVehicle.r});
        myVehicle.update(dt, Fx, Fy, torque);
    }

    // Création d'un objet Gnuplot
    Gnuplot gp;

    // Pour vx :
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output 'vx.png'\n";
    gp << "set title 'Vitesse Longitudinale (vx)'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'vx (m/s)'\n";
    gp << "plot '-' with lines lw 2 title 'vx'\n";
    gp.send1d(vx_data);
    gp << "unset output\n";
    gp.flush();

    // Pour vy :
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output 'vy.png'\n";
    gp << "set title 'Vitesse Latérale (vy)'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'vy (m/s)'\n";
    gp << "plot '-' with lines lw 2 title 'vy'\n";
    gp.send1d(vy_data);
    gp << "unset output\n";
    gp.flush();

    // Pour r :
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output 'r.png'\n";
    gp << "set title 'Taux de Lacet (r)'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'r (rad/s)'\n";
    gp << "plot '-' with lines lw 2 title 'r'\n";
    gp.send1d(r_data);
    gp << "unset output\n";
    gp.flush();


    return 0;
}
