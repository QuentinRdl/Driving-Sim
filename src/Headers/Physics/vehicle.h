#ifndef VEHICLE_H
#define VEHICLE_H

#include <cmath>

#include "calculationhelper.h"


class Vehicle {
public:
    float mass;   // Masse [kg]
    float dist_cog_front_axle;   // Distance COG - essieu avant [m]
    float dist_cog_rear_axle;   // Distance COG - essieu arrière [m]
    float airResCoeff;  // Coefficient de résistance de l'air
    float I;   // Moment d'inertie effectif pour le lacet
    float Cx;  // Rigidité longitudinale [N]
    float Cy;  // Rigidité latérale [N/rad]

    float vx;  // Vitesse longitudinale [m/s]
    float vy;  // Vitesse latérale [m/s]
    float lacet;   // Taux de lacet (vitesse angulaire) [rad/s]

    // Pour gérer la position du Véhicule
    float x;
    float y;
    float psi; // Orientation du véhicule [rad]

    // Données pour étape 3 :
    float slip; // Slip dynamique pour les roues avant
    float slip_tau; // Constante de temps pour la dynamique du slip
    float s_desired; // Valeur de slip désirée

    // Données pour étape 4 :
    float mu_front; // Coefficient de friction pour l'essieu avant
    float mu_rear; // Coefficient de friction pour l'essieu arriere
    float g; // Acceleration due a la gravite


    size_t count; // TODO : Remove !!!

    Vehicle(const float mass, const float dist_cog_front_axle, const float dist_cog_rear_axle,
            const float airRes, const float cx, const float cy,
            const float slip, const float slip_tau, const float s_desired,
            const float mu_front, const float mu_rear, const float g)
        : mass(mass), dist_cog_front_axle(dist_cog_front_axle), dist_cog_rear_axle(dist_cog_rear_axle),
            airResCoeff(airRes), Cx(cx), Cy(cy), vx(1), vy(1),
            lacet(0.0), x(600.0f), y(400.0f), psi(0.0),
            slip(slip), slip_tau(slip_tau), s_desired(s_desired),
            mu_front(mu_front), mu_rear(mu_rear), g(g)
    {
        I = mass * std::powf(0.5f * (dist_cog_front_axle + dist_cog_rear_axle), 2.0f);
        count = 0; // TODO REMOVE.
    }

    /**
     * TODO Full Complete Documentation.
     * @param dt
     * @param delta
     */
    void updateBicycleRK4(float dt, float delta);

private:

    void computeDerivatives(const float s[7], float dsdt[7], float delta);
};



#endif //VEHICLE_H
