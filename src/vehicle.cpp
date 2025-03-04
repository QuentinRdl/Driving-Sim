#include "vehicle.h"

/**
 * TODO complete documentation / @gubgub
 * @param dt
 * @param delta
 */
void Vehicle::updateBicycleRK4(const float dt, float delta) {
    // État vectoriel : [slip, vx, vy, r, psi, x, y]
    float state[7] = {slip, vx, vy, lacet, psi, x, y};
    float k1[7], k2[7], k3[7], k4[7], temp[7];


    // Calcul de k1
    computeDerivatives(state, k1, delta);
    // Calcul de k2
    for (int i = 0; i < 7; i++) {
        temp[i] = state[i] + 0.5f * dt * k1[i];
    }
    computeDerivatives(temp, k2, delta);
    // Calcul de k3
    for (int i = 0; i < 7; i++) {
        temp[i] = state[i] + 0.5f * dt * k2[i];
    }
    computeDerivatives(temp, k3, delta);
    // Calcul de k4
    for (int i = 0; i < 7; i++) {
        temp[i] = state[i] + dt * k3[i];
    }
    computeDerivatives(temp, k4, delta);

    // Combinaison pour obtenir le nouvel état
    for (int i = 0; i < 7; i++) {
        state[i] = state[i] + dt / 6.0f * (k1[i] + 2.0f * k2[i] + 2.0f * k3[i] + k4[i]);
    }
    // Mise à jour des variables membres
    slip = state[0];
    vx = state[1];
    vy = state[2];
    lacet = state[3];
    psi = state[4];
    x = state[5];
    y = state[6];
}

/**
 * TODO complete documentation / @gubgub
 * @param s
 * @param dsdt
 * @param delta
 */
void Vehicle::computeDerivatives(const float s[7], float dsdt[7], float delta) {
    // Extraction des états
    const float slip_val = s[0];
    const float vx_val = s[1];
    const float vy_val = s[2];
    const float r_val = s[3];
    const float psi_val = s[4];
    // s[5] = x, s[6] = y

    // Dynamique du slip
    dsdt[0] = (s_desired - slip_val) / slip_tau;

    // Calcul des angles de glissement (éviter division par zéro)
    float alpha_F = 0.0, alpha_R = 0.0;
    if (vx_val > 0.01f) {
        alpha_F = delta - (vy_val + dist_cog_front_axle * r_val) / vx_val;
        alpha_R = -(vy_val - dist_cog_rear_axle * r_val) / vx_val;
    }

    // Forces longitudinales
    const float F_x_front = 2.0f * Cx * slip_val;
    constexpr float F_x_rear = 0.0;

    // Charges verticales sur chaque essieu
    const float Fz_front = mass * g * (dist_cog_rear_axle / (dist_cog_front_axle + dist_cog_rear_axle));
    const float Fz_rear = mass * g * (dist_cog_front_axle / (dist_cog_front_axle + dist_cog_rear_axle));
    const float F_y_max_front = mu_front * Fz_front;
    const float F_y_max_rear = mu_rear * Fz_rear;

    // Forces latérales linéaires
    const float F_y_front_linear = 2.0f * Cy * alpha_F;
    const float F_y_rear_linear = 2.0f * Cy * alpha_R;

    const float ratio_front = F_y_front_linear / F_y_max_front;
    const float ratio_rear = F_y_rear_linear / F_y_max_rear;

    // Force saturée
    // Saturation via tanh pour modéliser la limite des pneus
    const float F_y_front = F_y_max_front * tanhf(ratio_front);
    const float F_y_rear = F_y_max_rear * tanhf(ratio_rear);

    // Affichage console si on est en saturation
    if (std::fabs(ratio_front) > 1.0f) {
        // std::cout << "[SAT FRONT] ratio = " << ratio_front
        // << " => F_y_front_linear=" << F_y_front_linear
        // << " N, F_y_front=" << F_y_front << " N\n";
        count++;
    }
    if (std::fabs(ratio_rear) > 1.0f) {
        // std::cout << "[SAT REAR] ratio = " << ratio_rear
        //         << " => F_y_rear_linear=" << F_y_rear_linear
        //      << " N, F_y_rear=" << F_y_rear << " N\n";
        count++;
    }

    // Accélérations (modèle bicycle)
    const float ax = vy_val * r_val + (1.0f / mass) *
                     (F_x_front * cosf(delta) - F_y_front * sinf(delta) + F_x_rear - airResCoeff * vx_val * vx_val);

    // TODO Call Gub Gub to check why the two following lines are reassigned without using the values few lines in bottom
    float ay = -vx_val * r_val + (1.0f / mass) * (F_x_front * sinf(delta) + F_y_front * cosf(delta) + F_y_rear);
    float r_dot = 1.0f / I * (dist_cog_front_axle * (F_x_front * sinf(delta) + F_y_front * cosf(delta)) - dist_cog_rear_axle * F_y_rear);


    // TODO : Is Working ?
    // On va essayer d'amortir le lateral
    // Parametre d'amortissement lateral (en kg/s) :
    constexpr float c_lat = 1000.0; // TODO : A ajuster en fonction des ocsillations

    // TODO : Is Working ?
    // Calcul de ay (Modele bicycle + amortissement)
    ay = -vx_val * r_val
         + (1.0f / mass) * (F_x_front * sinf(delta) + F_y_front * cosf(delta) + F_y_rear)
         - (c_lat / mass) * vy_val;


    // Paramètre d'amortissement en lacet (en N·m·s/rad) :
    constexpr float c_yaw = 2000.0f; // À ajuster empiriquement

    const float torque = dist_cog_front_axle * (F_x_front * sinf(delta) + F_y_front * cosf(delta))
                         - dist_cog_rear_axle * F_y_rear;

    // Ajout d'un couple d'amortissement = - c_yaw * r_val
    r_dot = (1.0f / I) * ( torque - c_yaw * r_val );



    // FIN TODO : Is Working ?

    dsdt[1] = ax;
    dsdt[2] = ay;
    dsdt[3] = r_dot;
    dsdt[4] = r_val; // d(psi)/dt = r

    // Transformation en vitesses globales
    const float v_global_x = vx_val * cosf(psi_val) - vy_val * sinf(psi_val);
    const float v_global_y = vx_val * sinf(psi_val) + vy_val * cosf(psi_val);
    dsdt[5] = v_global_x; // dx/dt
    dsdt[6] = v_global_y; // dy/dt
}
