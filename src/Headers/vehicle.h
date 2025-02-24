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
    double mass;   // Masse [kg]
    double dist_cog_front_axle;   // Distance COG - essieu avant [m]
    double dist_cog_rear_axle;   // Distance COG - essieu arrière [m]
    double airRes;  // Coefficient de résistance de l'air
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

    /**
     * @param mass the mass of the vehicle
     * @param distance_cog_front_axle the distance between the center of gravity and the front axle
     * @param distance_cog_rear_axle the distance between the center of gravity and the rear axle
     * @param airRes the air resistance coefficient
     * @param cx rigidity in the longitudinal direction (in N)
     * @param cy rigidity in the lateral direction (in N/rad)
     * @param x the x position of the vehicle
     * @param y the y position of the vehicle
     * @param r the yaw rate of the vehicle
     */
    Vehicle(const double mass,
            const double distance_cog_front_axle, const double distance_cog_rear_axle,
            const double airRes,
            const double cx, const double cy,
            const double x, const double y, const double r)
    : mass(mass), dist_cog_front_axle(distance_cog_front_axle), dist_cog_rear_axle(distance_cog_rear_axle),
    airRes(airRes), vx(0.0), vy(0.0), r(r), Cx(cx), Cy(cy), x(x), y(y), psi(0.0) {
        I = mass * std::pow(0.5 * (distance_cog_front_axle + distance_cog_rear_axle), 2);
    }


    void updateBicycle(const double dt, const double delta, const double slip)
    {
        // Calculer les angles de glissement pour les pneus avant (alpha_F) et arrière (alpha_R)
        double alpha_F = 0.0, alpha_R = 0.0;
        if (vx > 0.01) { // évite la division par zéro
            alpha_F = delta - (vy + dist_cog_front_axle * r) / vx;
            alpha_R = -(vy - dist_cog_rear_axle * r) / vx;
        }

        // Calcul des forces sur les pneus (hypothèse : les forces sont identiques sur les deux roues de l'essieu)
        const double F_x_front = 2.0 * Cx * slip;    // Force longitudinale sur l'essieu avant (drive wheels)
        constexpr double F_x_rear  = 0.0;            // Pas de force longitudinale à l'arrière
        const double F_y_front = 2.0 * Cy * alpha_F; // Force latérale sur l'essieu avant
        const double F_y_rear  = 2.0 * Cy * alpha_R; // Force latérale sur l'essieu arrière

        // Calcul des accélérations selon le modèle "bicycle"
        const double ax = vy * r + 1.0/mass * (F_x_front * cos(delta) - F_y_front * sin(delta) + F_x_rear - airRes * vx * vx);
        const double ay = -vx * r + 1.0/mass * (F_x_front * sin(delta) + F_y_front * cos(delta) + F_y_rear);
        const double r_dot = 1.0 / I * (dist_cog_front_axle * (F_x_front * sin(delta) + F_y_front * cos(delta)) - dist_cog_rear_axle * F_y_rear);

        // Mise à jour des états par intégration d'Euler
        vx += ax * dt;
        vy += ay * dt;
        r += r_dot * dt;

        psi += r * dt; // Integration du taux de lacet pour obtenir l'angle de direction

        // Transformation des vitesses locales en vitesses globales :
        const double v_global_x = vx * cos(psi) - vy * sin(psi);
        const double v_global_y = vx * sin(psi) + vy * cos(psi);

        // Mise à jour des positions globales par intégration :
        x += v_global_x * dt;
        y += v_global_y * dt;
    }

};



#endif //VEHICLE_H
