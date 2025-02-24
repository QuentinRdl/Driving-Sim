//
// Created by keiris on 1/20/25.
//

#include "VehiculeDynamics.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <cstdio>

#include "../gnuplot-iostream.h"

using namespace std;


// Structure pour stocker les paramètres du véhicule
struct VehicleParams {
    double m;    // Masse du véhicule [kg]
    double a;    // Distance du COG à l'essieu avant [m]
    double b;    // Distance du COG à l'essieu arrière [m]
    double Cx;   // Raideur longitudinale [N]
    double Cy;   // Raideur latérale [N/rad]
    double CA;   // Coefficient de résistance de l'air [1/m]
};

// Fonction pour calculer les dérivées de l'état (compute_dx)
vector<double> compute_dx(const vector<double>& x, const vector<double>& u, const VehicleParams& p) {
    // x[0]: v_x, x[1]: v_y, x[2]: r
    // u[0]: s_FL, u[1]: s_FR, u[2]: s_RL, u[3]: s_RR, u[4]: delta
    vector<double> dx(3, 0.0);

    double v_x = x[0];
    double v_y = x[1];
    double r   = x[2];
    double delta = u[4];

    // Calcul des fonctions trigonométriques pour delta
    double cos_delta = cos(delta);
    double sin_delta = sin(delta);

    // Équation pour la dérivée de la vitesse longitudinale
    dx[0] = v_y * r
            + (1.0 / p.m) * (
                p.Cx * (u[0] + u[1]) * cos_delta
                - 2.0 * p.Cy * (u[4] - (v_y + p.a * r) / v_x) * sin_delta
                + p.Cx * (u[2] + u[3])
                - p.CA * v_x * v_x
            );

    // Équation pour la dérivée de la vitesse latérale
    dx[1] = -v_x * r
            + (1.0 / p.m) * (
                p.Cx * (u[0] + u[1]) * sin_delta
                + 2.0 * p.Cy * (u[4] - (v_y + p.a * r) / v_x) * cos_delta
                + 2.0 * p.Cy * (p.b * r - v_y) / v_x
            );

    // Pour la dérivée du taux de lacet, le dénominateur est : m * ((a+b)/2)^2
    double denom = p.m * pow((p.a + p.b) / 2.0, 2);
    dx[2] = (1.0 / denom) * (
                p.a * ( p.Cx * (u[0] + u[1]) * sin_delta
                        + 2.0 * p.Cy * (u[4] - (v_y + p.a * r) / v_x) * cos_delta )
                - 2.0 * p.b * p.Cy * (p.b * r - v_y) / v_x
            );

    return dx;
}

// Fonction pour calculer les sorties du système (compute_y)
vector<double> compute_y(const vector<double>& x, const vector<double>& u, const VehicleParams& p) {
    vector<double> y(3, 0.0);

    double v_x = x[0];
    double v_y = x[1];
    double r   = x[2];
    double delta = u[4];

    double sin_delta = sin(delta);
    double cos_delta = cos(delta);

    // Sortie 1 : vitesse longitudinale
    y[0] = v_x;
    // Sortie 2 : accélération latérale
    y[1] = (1.0 / p.m) * (
                p.Cx * (u[0] + u[1]) * sin_delta
                + 2.0 * p.Cy * (u[4] - (v_y + p.a * r) / v_x) * cos_delta
                + 2.0 * p.Cy * (p.b * r - v_y) / v_x
           );
    // Sortie 3 : taux de lacet
    y[2] = r;

    return y;
}

int main() {
    // Définition des paramètres du véhicule (valeurs issues de l'exemple)
    VehicleParams p = {
        1700,      // m
        1.5,       // a
        1.5,       // b
        150000,    // Cx
        40000,     // Cy
        0.5        // CA
    };

    // Conditions initiales (par exemple, x1 > 0 pour la validité du modèle)
    vector<double> x = {1.0, 0.0, 0.0};  // [v_x, v_y, r]

    // Simulation : définir le pas de temps et la durée finale
    double dt = 0.1;      // Pas de temps (secondes)
    double t_final = 10;  // Durée totale de la simulation (secondes)

    std::ofstream dataFile("data.txt");

    // Boucle de simulation avec intégration par la méthode d'Euler
    for (double t = 0; t < t_final; t += dt) {
        // Définir les entrées u à chaque itération.
        // Pour cet exemple, nous utilisons des valeurs constantes.
        // u = [s_FL, s_FR, s_RL, s_RR, delta]
        vector<double> u = {0.01, 0.01, 0.0, 0.0, 0.05};  // Exemple d'entrées

        // Calculer le vecteur dérivé (dx/dt) à partir de l'état actuel
        vector<double> dx = compute_dx(x, u, p);

        // Intégration Euler : mise à jour de l'état
        for (size_t i = 0; i < x.size(); i++) {
            x[i] += dt * dx[i];
        }

        // Calculer la sortie à partir de l'état mis à jour
        vector<double> y = compute_y(x, u, p);

        // Afficher les résultats pour chaque instant t
        cout << "t = " << t
             << " | v_x = " << y[0]
             << " | a_y = " << y[1]
             << " | r = " << y[2]
             << endl;

        // Write to file
        dataFile << t << " " << y[0] << " " << y[1] << " " << y[2] << "\n";

    }

        dataFile.close();

        // Puis, pour afficher le plot avec Gnuplot, utilisez un pipe pour envoyer les commandes :
        FILE* gp = popen("gnuplot -persistent", "w");
        if (gp) {
            fprintf(gp, "set title 'Simulation de dynamique du véhicule'\n");
            fprintf(gp, "set xlabel 'Temps (s)'\n");
            fprintf(gp, "set ylabel 'Vitesse Longitudinale (m/s)'\n");
            // Pour tracer, par exemple, la vitesse longitudinale en fonction du temps :
            fprintf(gp, "plot 'data.txt' using 1:2 with lines title 'v_x'\n");
            pclose(gp);
        }
    return 0;
}
