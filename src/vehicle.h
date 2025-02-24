//
// Created by rgld_ on 24/02/25.
//

#ifndef VEHICLE_H
#define VEHICLE_H

#include <cmath>


/**
 *
 * TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * TODO Temporary implementation of QRadlo of the vehicle physic model
 * TODO CHANGE THIS
 * TODO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 */
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
    double Cx;  // Rigidité longitudinale [N]
    double Cy;  // Rigidité latérale [N/rad]

    // Pour gérer la position du Véhicule
    double x;
    double y;
    double psi;

    // Constructeur étape 1
    Vehicle(double mass, double a_front, double b_rear, double airRes = 0.0)
      : m(mass), a(a_front), b(b_rear), CA(airRes), vx(0.0), vy(0.0), r(0.0)
    {
        I = m * std::pow(0.5 * (a + b), 2);
    }

    // Constructeur étape 2
    Vehicle(double mass, double a_front, double b_rear, double airRes, double cx, double cy, double x, double y)
  : m(mass), a(a_front), b(b_rear), CA(airRes), Cx(cx), Cy(cy), vx(0.0), vy(0.0), r(0.0), x(x), y(y), psi(0.0)
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

    void updateBicycle(double dt, double delta, double slip)
    {
        // Calculer les angles de glissement pour les pneus avant (alpha_F) et arrière (alpha_R)
        double alpha_F = 0.0, alpha_R = 0.0;
        if (vx > 0.01) { // évite la division par zéro
            alpha_F = delta - (vy + a * r) / vx;
            alpha_R = -(vy - b * r) / vx;
        }

        // Calcul des forces sur les pneus (hypothèse : les forces sont identiques sur les deux roues de l'essieu)
        double F_x_front = 2.0 * Cx * slip;  // Force longitudinale sur l'essieu avant (drive wheels)
        double F_x_rear  = 0.0;                // Pas de force longitudinale à l'arrière
        double F_y_front = 2.0 * Cy * alpha_F; // Force latérale sur l'essieu avant
        double F_y_rear  = 2.0 * Cy * alpha_R;  // Force latérale sur l'essieu arrière

        // Calcul des accélérations selon le modèle "bicycle"
        double ax = vy * r + 1.0/m * (F_x_front * cos(delta) - F_y_front * sin(delta) + F_x_rear - CA * vx * vx);
        double ay = -vx * r + 1.0/m * (F_x_front * sin(delta) + F_y_front * cos(delta) + F_y_rear);
        double r_dot = 1.0 / I * (a * (F_x_front * sin(delta) + F_y_front * cos(delta)) - b * F_y_rear);

        // Mise à jour des états par intégration d'Euler
        vx += ax * dt;
        vy += ay * dt;
        r += r_dot * dt;

        psi += r * dt; // Integration du taux de lacet pour obtenir l'angle de direction

        // Transformation des vitesses locales en vitesses globales :
        double v_global_x = vx * cos(psi) - vy * sin(psi);
        double v_global_y = vx * sin(psi) + vy * cos(psi);

        // Mise a jour des positions globales par intégration :
        x += v_global_x * dt;
        y += v_global_y * dt;

    }


};



#endif //VEHICLE_H
