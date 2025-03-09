#ifndef VEHICLE_H
#define VEHICLE_H

#include <cmath>
#include <vector>
#include <string>
#include <iostream>

#include "calculationhelper.h"

void plotTest();

struct vehicleData {
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

    float delta; // Angle de braquage (radiant)
};

#include <plotting.h> // Moved the inclusion here to prevent inclusion looping

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
    Vehicle() : mass(0), dist_cog_front_axle(0), dist_cog_rear_axle(0),
        airResCoeff(0), I(0), Cx(0), Cy(0), vx(0), vy(0), lacet(0), x(0),
        y(0), psi(0), slip(0), slip_tau(0), s_desired(0),
        mu_front(0), mu_rear(0), g(0), count(0)
    {
    }

    // TODO : Remove this constructor
    Vehicle(float mass, float a_front, float b_rear,
        float airRes, float cx, float cy,
        float slip, float slip_tau, float s_desired,
        float mu_front, float mu_rear, float g)
        : mass(mass), dist_cog_front_axle(a_front), dist_cog_rear_axle(b_rear), airResCoeff(airRes), Cx(cx), Cy(cy),
          vx(1), vy(1), lacet(0.0), x(0.0), y(0.0), psi(0.0), slip(slip), slip_tau(slip_tau), s_desired(s_desired),
          mu_front(mu_front), mu_rear(mu_rear), g(g), count(0) {
        I = mass * std::pow(0.5 * (dist_cog_front_axle + dist_cog_rear_axle), 2);
    }

    Vehicle(const float mass, const float dist_cog_front_axle, const float dist_cog_rear_axle,
            const float airRes, const float cx, const float cy,
            const float vx, const float vy, const float lacet,
            const float x, const float y, const float psi,
            const float slip, const float slip_tau, const float s_desired,
            const float mu_front, const float mu_rear, const float g)
        : mass(mass), dist_cog_front_axle(dist_cog_front_axle), dist_cog_rear_axle(dist_cog_rear_axle),
            airResCoeff(airRes), Cx(cx), Cy(cy), vx(vx), vy(vy),
            lacet(lacet), x(x), y(y), psi(psi),
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

    /**
     * TODO Full Complete Documentation.
     * @param nbIterations
     * @param data
     * @param step
     */
    void getNextIterations(size_t nbIterations, vehicleData* data, float step);

    /**
     * TODO Full Complete Documentation.
     * @param data
     */
    void setData(const vehicleData &data);

    /**
     * TODO Full Complete Documentation.
     * @param data
     */
    void getData(vehicleData &data) const;

private:

    void computeDerivatives(const float s[7], float dsdt[7], float delta);
};



#endif //VEHICLE_H
