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
    double Cx;  // Rigidité longitudinale [N]
    double Cy;  // Rigidité latérale [N/rad]

    // Pour gérer la position du Véhicule
    double x;
    double y;
    double psi;

    // Données pour étape 3 :
    double slip; // Slip dynamique pour les roues avant
    double slip_tau; // Constante de temps pour la dynamique du slip
    double s_desired; // Valeur de slip désirée


    // Constructeur étape 1
    Vehicle(double mass, double a_front, double b_rear, double airRes = 0.0)
      : m(mass), a(a_front), b(b_rear), CA(airRes), vx(0.0), vy(0.0), r(0.0)
    {
        I = m * std::pow(0.5 * (a + b), 2);
    }

    // Constructeur étape 2
    Vehicle(double mass, double a_front, double b_rear, double airRes, double cx, double cy)
  : m(mass), a(a_front), b(b_rear), CA(airRes), Cx(cx), Cy(cy), vx(0.0), vy(0.0), r(0.0), x(0.0), y(0.0), psi(0.0)
    {
        I = m * std::pow(0.5 * (a + b), 2);
    }

    // Constructeur étape 3
    Vehicle(double mass, double a_front, double b_rear, double airRes, double cx, double cy, double slip, double slip_tau, double s_desired)
  : m(mass), a(a_front), b(b_rear), CA(airRes), Cx(cx), Cy(cy), vx(0.0), vy(0.0), r(0.0), x(0.0), y(0.0), psi(0.0), slip(slip), slip_tau(slip_tau), s_desired(s_desired)
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

    void updateBicycle(double dt, double delta, double slip) {
        // Calculer les angles de glissement pour les pneus avant (alpha_F) et arrière (alpha_R)
        double alpha_F = 0.0, alpha_R = 0.0;
        if (vx > 0.01) {
            // évite la division par zéro
            alpha_F = delta - (vy + a * r) / vx;
            alpha_R = -(vy - b * r) / vx;
        }

        // Calcul des forces sur les pneus (hypothèse : les forces sont identiques sur les deux roues de l'essieu)
        double F_x_front = 2.0 * Cx * slip; // Force longitudinale sur l'essieu avant (drive wheels)
        double F_x_rear = 0.0; // Pas de force longitudinale à l'arrière
        double F_y_front = 2.0 * Cy * alpha_F; // Force latérale sur l'essieu avant
        double F_y_rear = 2.0 * Cy * alpha_R; // Force latérale sur l'essieu arrière

        // Calcul des accélérations selon le modèle "bicycle"
        double ax = vy * r + 1.0 / m * (F_x_front * cos(delta) - F_y_front * sin(delta) + F_x_rear - CA * vx * vx);
        double ay = -vx * r + 1.0 / m * (F_x_front * sin(delta) + F_y_front * cos(delta) + F_y_rear);
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

    void updateBicycleEtape3(double dt, double delta) {
        // Mise a jour dynamique du slip => Eq 1st degree
        if (slip_tau == 0) {
            // We don't want to divide by zero
            std::cerr << "Error: slip_tau cannot be zero" << std::endl;
            return;
        }
        double slip_dot = (s_desired - slip) / slip_tau;
        slip += slip_dot * dt;


        // Calculer les angles de glissement pour les pneus avant (alpha_F) et arrière (alpha_R)
        double alpha_F = 0.0, alpha_R = 0.0;
        if (vx > 0.01) {
            // évite la division par zéro
            alpha_F = delta - (vy + a * r) / vx;
            alpha_R = -(vy - b * r) / vx;
        }

        // Calcul des forces sur les pneus (hypothèse : les forces sont identiques sur les deux roues de l'essieu)
        double F_x_front = 2.0 * Cx * slip; // Force longitudinale sur l'essieu avant (drive wheels)
        double F_x_rear = 0.0; // Pas de force longitudinale à l'arrière
        double F_y_front = 2.0 * Cy * alpha_F; // Force latérale sur l'essieu avant
        double F_y_rear = 2.0 * Cy * alpha_R; // Force latérale sur l'essieu arrière

        // Calcul des accélérations selon le modèle "bicycle"
        double ax = vy * r + 1.0 / m * (F_x_front * cos(delta) - F_y_front * sin(delta) + F_x_rear - CA * vx * vx);
        double ay = -vx * r + 1.0 / m * (F_x_front * sin(delta) + F_y_front * cos(delta) + F_y_rear);
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
void plot_etape_2(
    std::vector<std::pair<double,double>> vx_data, std::vector<std::pair<double,double>>vy_data,
    std::vector<std::pair<double,double>>r_data, std::vector<std::pair<double,double>> traj_data)
    {
    // Création d'un objet Gnuplot pour générer les fichiers images
    Gnuplot gp;
    // Plot de vx
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output 'Images/vx_bicycle.png'\n";
    gp << "set title 'Vitesse Longitudinale (vx) - Modèle Bicycle'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'vx (m/s)'\n";
    gp << "plot '-' with lines lw 2 title 'vx'\n";
    gp.send1d(vx_data);
    gp << "unset output\n";
    gp.flush();


    // Plot de vy
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output 'Images/vy_bicycle.png'\n";
    gp << "set title 'Vitesse Latérale (vy) - Modèle Bicycle'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'vy (m/s)'\n";
    gp << "plot '-' with lines lw 2 title 'vy'\n";
    gp.send1d(vy_data);
    gp << "unset output\n";
    gp.flush();

    // Plot de r (taux de lacet)
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output 'Images/r_bicycle.png'\n";
    gp << "set title 'Taux de Lacet (r) - Modèle Bicycle'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'r (rad/s)'\n";
    gp << "plot '-' with lines lw 2 title 'r'\n";
    gp.send1d(r_data);
    gp << "unset output\n";
    gp.flush();

    // Plot de la trajectoire (x en fonction de y)
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output 'Images/trajectory.png'\n";
    gp << "set title 'Trajectoire du Véhicule'\n";
    gp << "set xlabel 'Position X (m)'\n";
    gp << "set ylabel 'Position Y (m)'\n";
    gp << "plot '-' with lines lw 2 title 'Trajectoire'\n";
    gp.send1d(traj_data);
    gp << "unset output\n";
    gp.flush();

}

void etape1() {
    // Initialisation du véhicule (m=1700 kg, a=1.5 m, b=1.5 m, CA=0.5)
    Vehicle myVehicle(1700.0, 1.5, 1.5, 0.5);

    // Paramètres de simulation
    double Fx = 500.0;     // Force longitudinale (N)
    double Fy = 200.0;     // Force latérale (N)
    double torque = -150.0; // Moment de lacet (Nm)
    double dt = 0.1;       // Pas de temps (s)
    int steps = 10000;       // Simulation sur 10 secondes

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
}


void etape2() {
    // Initialisation du véhicule avec modèle Bicycle
    // Paramètres : Masse = 1700 kg, a = 1.5 m, b = 1.5 m, CA = 0.5, Cx = 150000 N, Cy = 40000 N/rad
    Vehicle myVehicle(1700.0, 1.5, 1.5, 0.5, 150000.0, 40000.0);

    double dt = 0.2;
    int steps = 1000;
    // Choix d'un angle de braquage (delta) et d'un slip constant pour la simulation
    double delta = 0.05; // en radians
    double slip  = 0.1;  // valeur de glissement

    // Vecteurs pour stocker les données (temps, valeur)
    std::vector<std::pair<double,double>> vx_data, vy_data, r_data;
    std::vector<std::pair<double,double>> traj_data; // Pour stocker les données relatives à la trajectoire du véhicule


    for (int i = 0; i <= steps; ++i) {
        if (i == 500) {
            delta = -delta;
        }
        double t = i * dt;
        vx_data.push_back({t, myVehicle.vx});
        vy_data.push_back({t, myVehicle.vy});
        r_data.push_back({t, myVehicle.r});
        traj_data.push_back({myVehicle.x, myVehicle.y});

        // Mise à jour de la dynamique avec le modèle Bicycle
        myVehicle.updateBicycle(dt, delta, slip);
    }

    plot_etape_2(vx_data, vy_data, r_data, traj_data);
}

void etape3() {
    // Initialisation du véhicule avec modèle Bicycle
    // Paramètres : Masse = 1700 kg, a = 1.5 m, b = 1.5 m, CA = 0.5, Cx = 150000 N, Cy = 40000 N/rad
    double initSlip = 0;
    double initSlip_tau = 0.5;
    double initS_desired = 0.1; // Valeur cible de slip

    Vehicle myVehicle(1700.0, 1.5, 1.5, 0.5, 150000.0, 40000.0, initSlip, initSlip_tau, initS_desired);

    double dt = 0.2;
    int steps = 1000;
    // Choix d'un angle de braquage (delta) et d'un slip constant pour la simulation
    double delta = 0.05; // en radians
    // double slip  = 0.1;  // valeur de glissement

    // Vecteurs pour stocker les données (temps, valeur)
    std::vector<std::pair<double,double>> vx_data, vy_data, r_data, slip_data;
    std::vector<std::pair<double,double>> traj_data; // Pour stocker les données relatives à la trajectoire du véhicule


    for (int i = 0; i <= steps; ++i) {
        if (i == 500) {
            delta = -delta;
        }
        double t = i * dt;
        vx_data.push_back({t, myVehicle.vx});
        vy_data.push_back({t, myVehicle.vy});
        r_data.push_back({t, myVehicle.r});
        traj_data.push_back({myVehicle.x, myVehicle.y});
        // slip_data.push_back({t, myVehicle.slip});

        // Mise à jour de la dynamique avec le modèle Bicycle
        myVehicle.updateBicycleEtape3(dt, delta);
    }

    plot_etape_2(vx_data, vy_data, r_data, traj_data);
}

int main() {
    etape3();
    return 0;
}
