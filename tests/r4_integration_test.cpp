//
// Created by keiris on 03/03/25.
//

// rk4_integration_test.cpp

#include <gtest/gtest.h>
#include <cmath>

// Fonction dérivée f(x,t) = dx/dt = x
double f(double x, double /* t */) {
    return x;
}

/**
 * @brief Réalise une étape d'intégration de type Runge-Kutta d'ordre 4 (RK4) pour une équation différentielle ordinaire (EDO) scalaire.
 *
 * Cette fonction effectue une seule étape d'intégration RK4 pour une EDO de la forme :
 *     dx/dt = f(x, t)
 * avec une condition initiale donnée x à l'instant t, et un pas de temps dt.
 *
 * La méthode RK4 calcule quatre "pentes" (k1, k2, k3, k4) qui sont des approximations des dérivées à différents points dans l'intervalle dt :
 *   - k1 représente la pente au début de l'intervalle (à t).
 *   - k2 est la pente à mi-chemin dans l'intervalle, en utilisant k1 pour estimer x(t + dt/2).
 *   - k3 est une autre estimation de la pente à mi-chemin, mais en utilisant k2.
 *   - k4 représente la pente à la fin de l'intervalle (à t + dt), en utilisant k3 pour estimer x(t + dt).
 *
 * La solution numérique est ensuite mise à jour en prenant une moyenne pondérée de ces pentes :
 *     x(t + dt) ≈ x(t) + (dt/6) * (k1 + 2*k2 + 2*k3 + k4)
 *
 * Cette méthode est d'ordre 4, ce qui signifie que l'erreur locale par étape est proportionnelle à dt^5 et l'erreur globale est proportionnelle à dt^4.
 *
 * @param f  Pointeur vers la fonction qui calcule f(x, t) (la dérivée de x).
 * @param x  La valeur actuelle de x (état à l'instant t).
 * @param t  L'instant courant.
 * @param dt Le pas de temps pour l'intégration.
 * @return La nouvelle valeur de x après une étape d'intégration RK4.
 */double RK4Step(double (*f)(double, double), double x, double t, double dt) {
    const double k1 = f(x, t);
    const double k2 = f(x + 0.5 * dt * k1, t + 0.5 * dt);
    const double k3 = f(x + 0.5 * dt * k2, t + 0.5 * dt);
    const double k4 = f(x + dt * k3, t + dt);
    return x + (dt / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
}

// Test de convergence de l'intégrateur RK4
TEST(RK4IntegratorTest, ConvergenceTest) {
    constexpr double x0 = 1.0;       // condition initiale
    constexpr double T = 1.0;        // temps final
    constexpr int N = 1000;          // nombre d'étapes
    constexpr double dt = T / N;     // pas de temps

    double x_num = x0;
    double t = 0.0;

    // Boucle d'intégration
    for (int i = 0; i < N; ++i) {
        x_num = RK4Step(f, x_num, t, dt);
        t += dt;
    }

    // Solution exacte: x(T) = x0 * exp(T)
    const double x_exact = x0 * std::exp(T);
    const double error = std::fabs(x_num - x_exact);

    // Afficher l'erreur pour le débogage (optionnel)
    std::cout << "x_num = " << x_num << ", x_exact = " << x_exact
              << ", erreur = " << error << std::endl;

    // On s'attend à ce que l'erreur soit inférieure à une tolérance (ici 1e-6)
    EXPECT_NEAR(x_num, x_exact, 1e-6);
}

// Point d'entrée pour GoogleTest
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
