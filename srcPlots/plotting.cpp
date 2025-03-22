//
// Created by keiris on 09/03/25.
//

#include "plotting.h"

#include <utility>
#include "vehicle.h"

void Plotting::plot_etape(
    const std::vector<std::pair<float, float>>& vx_data, const std::vector<std::pair<float, float>>&vy_data,
    const std::vector<std::pair<float, float>>&r_data, const std::vector<std::pair<float, float>>& traj_data,
    const std::vector<std::pair<float, float>>& slip_data, const std::string& path)
    {
    // On s'assure que chaque vecteur n'est pas vide
    assert(!vx_data.empty());
    assert(!vy_data.empty());
    assert(!r_data.empty());
    assert(!traj_data.empty());
    assert(!slip_data.empty());

    // On s'assure que le path exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "Error: Path does not exist: " << path << std::endl;
        return;
    }
    // Création d'un objet Gnuplot pour générer les fichiers images
    Gnuplot gp;
    // Plot de vx
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output '" << path << "/vx_bicycle.png'\n";
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
    gp << "set output '" << path << "/vy_bicycle.png'\n";
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
    gp << "set output '" << path << "/r_bicycle.png'\n";
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
    gp << "set output '" << path << "/trajectory.png'\n";
    gp << "set title 'Trajectoire du Véhicule'\n";
    gp << "set xlabel 'Position X (m)'\n";
    gp << "set ylabel 'Position Y (m)'\n";
    gp << "plot '-' with lines lw 2 title 'Trajectoire'\n";
    gp.send1d(traj_data);
    gp << "unset output\n";
    gp.flush();

    if (slip_data.empty() == true) {
        std::cout << path << " : Plotting slip data is not possible, as the data is empty" << std::endl;
        return;
    }

    // Plot de slip_data (si possible)
    gp << "reset\n";
    gp << "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n";
    gp << "set output '" << path << "/slip_data.png'\n";
    gp << "set title 'Donnés sur le slip angle'\n";
    gp << "set xlabel 'Temps (s)'\n";
    gp << "set ylabel 'Slip (rad)'\n";
    gp << "plot '-' with lines lw 2 title 'vy'\n";
    gp.send1d(slip_data);
    gp << "unset output\n";
    gp.flush();
}

// Converts the array of vehicleData into the vector or pair we inputs
void Plotting::convertToArray(
    std::vector<std::pair<float, float>>& vx_data, std::vector<std::pair<float, float>>& vy_data,
    std::vector<std::pair<float, float>>& r_data, std::vector<std::pair<float, float>>& traj_data,
    std::vector<std::pair<float, float>>& slip_data, vehicleData *data, size_t size) {
    // We iterate through our array of vehicleData
    for (size_t i = 0; i < size; ++i) {
        // Access each element using data[i]
        std::cout << data[i].vx << std::endl;
        vx_data.emplace_back(static_cast<float>(i), data[i].vx);
        vy_data.emplace_back(static_cast<float>(i), data[i].vy);
        r_data.emplace_back(static_cast<float>(i), data[i].lacet);
        traj_data.emplace_back(data[i].x, data[i].y);
        slip_data.emplace_back(static_cast<float>(i), data[i].slip);
        std::cout << "convertToArray => Iteration number: " << i << std::endl;
    }
}

void Plotting::plotStepFromArray(vehicleData *data, size_t size, std::string path) {
    // On s'assure que le path exists
    if (!std::filesystem::exists(path)) {
        std::cerr << "Error: Path does not exist: " << path << std::endl;
        return;
    }

    // Vecteurs pour stocker les données (temps, valeur)
    std::vector<std::pair<float, float>> vx_data, vy_data, r_data, slip_data, traj_data;
    convertToArray(vx_data, vy_data, r_data, traj_data, slip_data, data, size);

    // Appel a la fonction de plot
    plot_etape(vx_data, vy_data, r_data, traj_data, slip_data, std::move(path));
}

int main() {
    Vehicle::plotTestIterative();
    Vehicle::plot();
}
