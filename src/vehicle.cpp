#include "vehicle.h"
#include "plotting.h"
#include <cassert>

/**
 * Met à jour l'état du véhicule en utilisant la méthode de Runge-Kutta d'ordre 4 (RK4) pour le modèle Bicycle.
 * @param dt the delta of time.
 * @param delta the steering angle.
 * @param currentState the current state of the vehicle
 */
std::array<float, 7> Vehicle::updateBicycleRK4(const float dt, const float delta, const std::array<float, 7>& currentState) const {
    const std::array<float, 7> state = currentState;
    // État initial (en SI) extrait des membres
    std::array<float, 7> newState{};
    float k1[7], k2[7], k3[7], k4[7], temp[7];

    computeDerivatives(state.data(), k1, delta);

    for (int i = 0; i < 7; ++i)
        temp[i] = state[i] + 0.5f * dt * k1[i];
    computeDerivatives(temp, k2, delta);

    for (int i = 0; i < 7; ++i)
        temp[i] = state[i] + 0.5f * dt * k2[i];
    computeDerivatives(temp, k3, delta);

    for (int i = 0; i < 7; ++i)
        temp[i] = state[i] + dt * k3[i];
    computeDerivatives(temp, k4, delta);

    for (int i = 0; i < 7; ++i)
        newState[i] = state[i] + dt / 6.0f * (k1[i] + 2.0f * k2[i] + 2.0f * k3[i] + k4[i]);

    return newState;
}

/**
 * Met à jour l'état du véhicule en utilisant la méthode RK4 avec l'état courant interne.
 * * @param dt the delta of time.
 * @param delta the steering angle.
 */
std::array<float, 7> Vehicle::updateBicycleRK4(const float dt, const float delta) const {
    return updateBicycleRK4(dt, delta, { slip, vx, vy, lacet, psi, x, y });
}

/**
 * Calcule les dérivées de l'état du véhicule pour l'intégration numérique.
 * @param state the state of the vehicle
 * @param dsdt the derivative of the state
 * @param delta the steering angle
 */
void Vehicle::computeDerivatives(const float state[7], float dsdt[7], const float delta) const {
    const float slip_val = state[0];
    const float vx_val   = state[1];
    const float vy_val   = state[2];
    const float r_val    = state[3];
    const float psi_val  = state[4];
    // s[5] = x, s[6] = y

    dsdt[0] = (s_desired - slip_val) / slip_tau;

    float alpha_F = 0.0f, alpha_R = 0.0f;
    if (vx_val > 0.01f) {
        alpha_F = delta - (vy_val + dist_cog_front_axle * r_val) / vx_val;
        alpha_R = -(vy_val - dist_cog_rear_axle * r_val) / vx_val;
    }

    const float F_x_front = 2.0f * Cx * slip_val;
    constexpr float F_x_rear = 0.0f;

    const float Fz_front = mass * g * (dist_cog_rear_axle / (dist_cog_front_axle + dist_cog_rear_axle));
    const float Fz_rear  = mass * g * (dist_cog_front_axle / (dist_cog_front_axle + dist_cog_rear_axle));
    const float F_y_max_front = mu_front * Fz_front;
    const float F_y_max_rear  = mu_rear * Fz_rear;

    const float F_y_front_linear = 2.0f * Cy * alpha_F;
    const float F_y_rear_linear  = 2.0f * Cy * alpha_R;

    const float ratio_front = F_y_front_linear / F_y_max_front;
    const float ratio_rear  = F_y_rear_linear  / F_y_max_rear;

    const float F_y_front = F_y_max_front * tanhf(ratio_front);
    const float F_y_rear  = F_y_max_rear  * tanhf(ratio_rear);

    const float ax = vy_val * r_val + (1.0f / mass) *
                     (F_x_front * cosf(delta) - F_y_front * sinf(delta) + F_x_rear - airResCoeff * vx_val * vx_val);

    float ay   = -vx_val * r_val + (1.0f / mass) * (F_x_front * sinf(delta) + F_y_front * cosf(delta) + F_y_rear);
    float r_dot = 1.0f / I * (dist_cog_front_axle * (F_x_front * sinf(delta) + F_y_front * cosf(delta)) - dist_cog_rear_axle * F_y_rear);

    constexpr float c_lat = 1000.0f;
    ay = ay - (c_lat / mass) * vy_val;

    constexpr float c_yaw = 2000.0f;
    const float torque = dist_cog_front_axle * (F_x_front * sinf(delta) + F_y_front * cosf(delta))
                         - dist_cog_rear_axle * F_y_rear;
    r_dot = (1.0f / I) * (torque - c_yaw * r_val);

    dsdt[1] = ax;
    dsdt[2] = ay;
    dsdt[3] = r_dot;
    dsdt[4] = r_val; // d(psi)/dt = r

    const float v_global_x = vx_val * cosf(psi_val) - vy_val * sinf(psi_val);
    const float v_global_y = vx_val * sinf(psi_val) + vy_val * cosf(psi_val);
    dsdt[5] = v_global_x;
    dsdt[6] = v_global_y;
}

/**
 * Calcule les itérations suivantes de la simulation de la dynamique du véhicule et remplit le tableau de données.
 * @param nbIterations the number of iterations to compute
 * @param data the data array to fill
 * @param dt the delta of time between two iterations (in seconds, often related to fps in the interface used)
 * @param startIndex the starting index in the data array
 */
void Vehicle::getNextIterations(const size_t nbIterations, vehicleData* data, const float dt, const size_t startIndex) const {
    assert(nbIterations > 0);
    const float delta = data[startIndex].delta;
    std::array<float, 7> currentState = { slip, vx, vy, lacet, psi, x, y };

    for (size_t i = 0; i < nbIterations; ++i) {
        currentState = updateBicycleRK4(dt, delta, currentState);
        const size_t idx = startIndex + i;
        data[idx].mass = mass;
        data[idx].dist_cog_front_axle = dist_cog_front_axle;
        data[idx].dist_cog_rear_axle = dist_cog_rear_axle;
        data[idx].airResCoeff = airResCoeff;
        data[idx].I = I;
        data[idx].Cx = Cx;
        data[idx].Cy = Cy;
        data[idx].vx = currentState[1];
        data[idx].vy = currentState[2];
        data[idx].lacet = currentState[3];
        data[idx].psi = currentState[4];
        data[idx].x = currentState[5];
        data[idx].y = currentState[6];
        data[idx].slip = currentState[0];
        data[idx].slip_tau = slip_tau;
        data[idx].s_desired = s_desired;
        data[idx].mu_front = mu_front;
        data[idx].mu_rear = mu_rear;
        data[idx].g = g;
    }
}
/**
 * Met à jour les attributs internes du véhicule à partir d'une structure de données.
 *
 * @param data La structure contenant les valeurs à assigner aux attributs du véhicule.
 */
void Vehicle::setData(const vehicleData &data) {
    mass = data.mass;
    dist_cog_front_axle = data.dist_cog_front_axle;
    dist_cog_rear_axle = data.dist_cog_rear_axle;
    airResCoeff = data.airResCoeff;
    I = data.I;
    Cx = data.Cx;
    Cy = data.Cy;
    vx = data.vx;
    vy = data.vy;
    lacet = data.lacet;
    x = data.x;
    y = data.y;
    psi = data.psi;
    slip = data.slip;
    slip_tau = data.slip_tau;
    s_desired = data.s_desired;
    mu_front = data.mu_front;
    mu_rear = data.mu_rear;
    g = data.g;
}

/**
 * Récupère les attributs du véhicule pour les stocker dans la structure de données passée en paramètres.
 *
 * @param data La structure qui sera remplie avec les valeurs actuelles du véhicule.
 */
void Vehicle::getData(vehicleData &data) const {
    data.mass = mass;
    data.dist_cog_front_axle = dist_cog_front_axle;
    data.dist_cog_rear_axle = dist_cog_rear_axle;
    data.airResCoeff = airResCoeff;
    data.I = I;
    data.Cx = Cx;
    data.Cy = Cy;
    data.vx = vx;
    data.vy = vy;
    data.lacet = lacet;
    data.x = x;
    data.y = y;
    data.psi = psi;
    data.slip = slip;
    data.slip_tau = slip_tau;
    data.s_desired = s_desired;
    data.mu_front = mu_front;
    data.mu_rear = mu_rear;
    data.g = g;
    // data[i].delta = // L'angle de braquage reste constant durant un round de simulation
}

/**
 * Exécute une simulation itérative de la dynamique du véhicule et génère les graphiques résultants.
 */
void Vehicle::plotTestIterative() {

    // Initialisation du véhicule avec modèle Bicycle
    // Paramètres : Masse = 1700 kg, a = 1.5 m, b = 1.5 m, CA = 0.5, Cx = 150000 N, Cy = 40000 N/rad
    // vehicleData *data = {};
    float initSlip = 0;
    float initSlip_tau = 0.5;
    float initS_desired = 0.1; // Valeur cible de slip

    Vehicle myVehicle(1700.0, 1.5, 1.5, 20, 150000.0, 40000.0, initSlip, initSlip_tau, initS_desired, 0.9, 0.9, 9.81);

    float dt = 0.02;
    int steps = 10000;
    // Choix d'un angle de braquage (delta) et d'un slip constant pour la simulation
    //double delta = 0.05; // en radians
    float delta = 0.05; // en radians
    vehicleData data[steps];
    data[0].delta = delta;
    myVehicle.getNextIterations(steps/2, data, dt, 0);
    data[steps/2].delta = -delta;
    myVehicle.getNextIterations(steps/2, data, dt, steps/2);

    // Print the results
    for (size_t i = 0; i < static_cast<size_t>(steps); ++i) {
        std::cout << "Iteration " << i << ": x = " << data[i].x << ", y = " << data[i].y << ", vx = " << data[i].vx << ", vy = " << data[i].vy << std::endl;
    }

    Plotting p;
    p.plotStepFromArray(data, steps, "../Plots/Iterative");
}

/**
 * Exécute une simulation itérative (mais sans utiliser la méthode itérative de notre classe, on utilise une boucle
 * classique ici) de la dynamique du véhicule et génère les graphiques résultants.
 */
void Vehicle::plot() {
    // Initialisation du véhicule avec modèle Bicycle
    // Paramètres : Masse = 1700 kg, a = 1.5 m, b = 1.5 m, CA = 0.5, Cx = 150000 N, Cy = 40000 N/rad
    // vehicleData *data = {};
    float initSlip = 0;
    float initSlip_tau = 0.5;
    float initS_desired = 0.1; // Valeur cible de slip

    Vehicle myVehicle(1700.0, 1.5, 1.5, 20, 150000.0, 40000.0,
                      initSlip, initSlip_tau, initS_desired, 0.7, 0.7, 9.81);

    float dt = 0.02;
    int steps = 30000;
    // Choix d'un angle de braquage (delta) et d'un slip constant pour la simulation
    //double delta = 0.05; // en radians
    float delta = 0.05; // en radians
    // double slip  = 0.1;  // valeur de glissement

    // Vecteurs pour stocker les données (temps, valeur)
    std::vector<std::pair<float, float>> vx_data, vy_data, r_data, slip_data;
    std::vector<std::pair<float, float>> traj_data; // Pour stocker les données relatives à la trajectoire du véhicule

    int change = steps / 2;
    // On crée une copie locale de l'état initial : [slip, vx, vy, lacet, psi, x, y]
    std::array<float, 7> currentState = { myVehicle.slip, myVehicle.vx, myVehicle.vy,
                                          myVehicle.lacet, myVehicle.psi, myVehicle.x, myVehicle.y };

    for (int i = 0; i <= steps; ++i) {
        if (i == change) {
            delta = -delta; // Pour creer un changement de direction
        }
        float t = i * dt;
        vx_data.emplace_back(t, currentState[1]);
        vy_data.emplace_back(t, currentState[2]);
        r_data.emplace_back(t, currentState[3]);
        traj_data.emplace_back(currentState[5], currentState[6]);
        slip_data.emplace_back(t, currentState[0]);

        // Mise à jour de la dynamique avec le modèle Bicycle
        // Ici, updateBicycleRK4 retourne le nouvel état sans modifier l'objet myVehicle
        currentState = myVehicle.updateBicycleRK4(dt, delta);
    }

    Plotting p;
    p.plot_etape(vx_data, vy_data, r_data, traj_data, slip_data, "../Plots");
}
