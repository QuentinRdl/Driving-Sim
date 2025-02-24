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
    double weight;   // Masse [kg]
    double distance_cog_front_axle;   // Distance COG - essieu avant [m]
    double distance_cog_rear_axle;   // Distance COG - essieu arrière [m]
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

    // Constructeur étape 2
    Vehicle(const double mass,
        const double distance_cog_front_axle, const double distance_cog_rear_axle,
        const double airRes,
        const double cx, const double cy,
        const double x, const double y, const double r)
    : weight(mass), distance_cog_front_axle(distance_cog_front_axle), distance_cog_rear_axle(distance_cog_rear_axle), CA(airRes), vx(0.0), vy(0.0), r(r), Cx(cx), Cy(cy), x(x), y(y), psi(0.0)
    {
        I = weight * std::pow(0.5 * (distance_cog_front_axle + distance_cog_rear_axle), 2);
    }

    // Loi de Newton pour la translation
    void computeTranslation(const double Fx, const double Fy, double& ax, double& ay) const {
        ax = Fx / weight;
        ay = Fy / weight;
    }

    // Loi de Newton pour la rotation (lacet)
    double computeYawAcceleration(const double torque) const {
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

    void updateBicycle(const double dt, const double delta, const double slip)
    {
        // Calculer les angles de glissement pour les pneus avant (alpha_F) et arrière (alpha_R)
        double alpha_F = 0.0, alpha_R = 0.0;
        if (vx > 0.01) { // évite la division par zéro
            alpha_F = delta - (vy + distance_cog_front_axle * r) / vx;
            alpha_R = -(vy - distance_cog_rear_axle * r) / vx;
        }

        // Calcul des forces sur les pneus (hypothèse : les forces sont identiques sur les deux roues de l'essieu)
        const double F_x_front = 2.0 * Cx * slip;    // Force longitudinale sur l'essieu avant (drive wheels)
        constexpr double F_x_rear  = 0.0;            // Pas de force longitudinale à l'arrière
        const double F_y_front = 2.0 * Cy * alpha_F; // Force latérale sur l'essieu avant
        const double F_y_rear  = 2.0 * Cy * alpha_R; // Force latérale sur l'essieu arrière

        // Calcul des accélérations selon le modèle "bicycle"
        const double ax = vy * r + 1.0/weight * (F_x_front * cos(delta) - F_y_front * sin(delta) + F_x_rear - CA * vx * vx);
        const double ay = -vx * r + 1.0/weight * (F_x_front * sin(delta) + F_y_front * cos(delta) + F_y_rear);
        const double r_dot = 1.0 / I * (distance_cog_front_axle * (F_x_front * sin(delta) + F_y_front * cos(delta)) - distance_cog_rear_axle * F_y_rear);

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
